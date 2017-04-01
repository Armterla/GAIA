#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_msys_base.h>
#include <gaia_msys_heapcrt.h>
#include <gaia_msys_heapesg.h>
#include <gaia_algo_string.h>
#include <gaia_ctn_vector.h>
#include <gaia_ctn_string.h>
#include <gaia_ctn_charsstring.h>
#include <gaia_ctn_map.h>
#include <gaia_doc_pdf.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#include <hpdf.h>

namespace GAIA
{
	namespace DOC
	{
		static const GAIA::CH* PDF_SUPPORT_IMAGE_RAW = "raw";
		static const GAIA::CH* PDF_SUPPORT_IMAGE_PNG = "png";
		static const GAIA::CH* PDF_SUPPORT_IMAGE_JPEG = "jpeg";
		static GAIA::GVOID hpdf_error_handler(HPDF_STATUS hpdf_err, HPDF_STATUS hpdf_detail, GAIA::GVOID* pUserPointer)
		{
			GAST(pUserPointer != GNIL);
			if(pUserPointer == GNIL)
				return;
			GAIA::DOC::PDFWriter* pWriter = (GAIA::DOC::PDFWriter*)pUserPointer;
			pWriter->OnError(hpdf_err, hpdf_detail);
		}
		static GAIA::GVOID* hpdf_alloc_func(HPDF_UINT uSize)
		{
			GAST(uSize > 0);
		#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
			return g_gaia_globalmsys.memory_alloc(uSize);
		#else
			return ::malloc(uSize);
		#endif
		}
		static GAIA::GVOID hpdf_free_func(GAIA::GVOID* p)
		{
			GAST(p != GNIL);
		#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
			g_gaia_globalmsys.memory_release(p);
		#else
			return ::free(p);
		#endif
		}
		GINL HPDF_Font HPDF_GetFontEx(HPDF_Doc doc, const GAIA::CH* pszFont)
		{
			const GAIA::CH* pszFinded = GAIA::ALGO::gstristr(pszFont, " Encoding:");
			if(pszFinded == GNIL)
				return HPDF_GetFont(doc, pszFont, GNIL);
			GAIA::CH szFontName[GAIA::MAXPL];
			GAIA::CH szEncodingName[GAIA::MAXPL];
			GAIA::ALGO::gstrcpy(szFontName, pszFont, pszFinded - pszFont);
			GAIA::ALGO::gstrcpy(szEncodingName, pszFinded + sizeof(" Encoding:") - 1);
			return HPDF_GetFont(doc, szFontName, szEncodingName);
		}
		class PDFWriterContext : public GAIA::Base
		{
		public:
			PDFWriterContext()
			{
				this->reset();
			}
			GAIA::GVOID reset()
			{
				doc = GNIL;
				page = GNIL;
				tCreateTime.localtime();
				tModTime.localtime();
				listCallBack.clear();
				listPage.clear();
				rFillAlpha = R(1.0);
				rStrokeAlpha = R(1.0);
				blendmode = GAIA::DOC::PDF_BLEND_MODE_NORMAL;
				bBeginText = GAIA::False;
				strFont = "Helvetica";
				rFontSize = R(10.0);
				rLineSpace = R(0.0);
				rWordSpace = R(0.0);
				rCharSpace = R(0.0);
				listFont.clear();
				listImage.clear();
				strCreatorPassword.clear();
				strReaderPassword.clear();
				uCompressModeFlag = PDF_COMPRESS_TEXT | PDF_COMPRESS_IMAGE | PDF_COMPRESS_META;
				uPermissionFlag = PDF_PERMISSION_READ | PDF_PERMISSION_COPY | PDF_PERMISSION_EDITALL | PDF_PERMISSION_PRINT;
				strEncodingName.clear();
			}
		public:
			HPDF_Doc doc;
			HPDF_ExtGState gstate;
			HPDF_Page page;
			GAIA::TIME::Time tCreateTime;
			GAIA::TIME::Time tModTime;
			GAIA::CTN::Vector<GAIA::DOC::PDFWriter::CallBack*> listCallBack;
			GAIA::CTN::Vector<HPDF_Page> listPage;
			GAIA::REAL rFillAlpha;
			GAIA::REAL rStrokeAlpha;
			GAIA::DOC::PDF_BLEND_MODE blendmode;
			GAIA::BL bBeginText;
			GAIA::CTN::AString strFont;
			GAIA::REAL rFontSize;
			GAIA::REAL rLineSpace;
			GAIA::REAL rWordSpace;
			GAIA::REAL rCharSpace;
			GAIA::CTN::Map<GAIA::CTN::ACharsString, HPDF_Font> listFont;
			GAIA::CTN::Map<GAIA::CTN::ACharsString, HPDF_Image> listImage;
			GAIA::CTN::AString strCreatorPassword;
			GAIA::CTN::AString strReaderPassword;
			GAIA::U32 uCompressModeFlag;
			GAIA::U32 uPermissionFlag;
			GAIA::CTN::AString strEncodingName;
		};
		PDFWriter::PDFWriter()
		{
			m_ctx = gnew PDFWriterContext;
			m_ctx->doc = HPDF_NewEx(hpdf_error_handler, hpdf_alloc_func, hpdf_free_func, 0, this);
			m_ctx->gstate = HPDF_CreateExtGState(m_ctx->doc);
		}
		PDFWriter::~PDFWriter()
		{
			HPDF_Free(m_ctx->doc);
			gdel m_ctx;
		}
		GAIA::GVOID PDFWriter::RegistCallBack(GAIA::DOC::PDFWriter::CallBack& cb)
		{
			if(this->IsRegistedCallBack(cb))
				GTHROW(Illegal);
			m_ctx->listCallBack.push_back(&cb);
		}
		GAIA::GVOID PDFWriter::UnregistCallBack(GAIA::DOC::PDFWriter::CallBack& cb)
		{
			if(!this->IsRegistedCallBack(cb))
				GTHROW(Illegal);
			m_ctx->listCallBack.remove(&cb);
		}
		GAIA::BL PDFWriter::IsRegistedCallBack(GAIA::DOC::PDFWriter::CallBack& cb) const
		{
			for(GAIA::NUM x = 0; x < m_ctx->listCallBack.size(); ++x)
			{
				if(m_ctx->listCallBack[x] == &cb)
					return GAIA::True;
			}
			return GAIA::False;
		}
		GAIA::GVOID PDFWriter::UnregistCallBackAll()
		{
			m_ctx->listCallBack.clear();
		}
		GAIA::GVOID PDFWriter::SetCreator(const GAIA::CH* pszCreator)
		{
			GAST(pszCreator != GNIL);
			if(pszCreator == GNIL)
				GTHROW(InvalidParam);
			if(HPDF_SetInfoAttr(m_ctx->doc, HPDF_INFO_CREATOR, pszCreator) != HPDF_OK)
				GTHROW(Illegal);
		}
		const GAIA::CH* PDFWriter::GetCreator() const
		{
			return HPDF_GetInfoAttr(m_ctx->doc, HPDF_INFO_CREATOR);
		}
		GAIA::GVOID PDFWriter::SetAuthor(const GAIA::CH* pszAuthor)
		{
			GAST(pszAuthor != GNIL);
			if(pszAuthor == GNIL)
				GTHROW(InvalidParam);
			if(HPDF_SetInfoAttr(m_ctx->doc, HPDF_INFO_AUTHOR, pszAuthor) != HPDF_OK)
				GTHROW(Illegal);
		}
		const GAIA::CH* PDFWriter::GetAuthor() const
		{
			return HPDF_GetInfoAttr(m_ctx->doc, HPDF_INFO_AUTHOR);
		}
		GAIA::GVOID PDFWriter::SetTitle(const GAIA::CH* pszTitle)
		{
			GAST(pszTitle != GNIL);
			if(pszTitle == GNIL)
				GTHROW(InvalidParam);
			if(HPDF_SetInfoAttr(m_ctx->doc, HPDF_INFO_TITLE, pszTitle) != HPDF_OK)
				GTHROW(Illegal);
		}
		const GAIA::CH* PDFWriter::GetTitle() const
		{
			return HPDF_GetInfoAttr(m_ctx->doc, HPDF_INFO_TITLE);
		}
		GAIA::GVOID PDFWriter::SetSubject(const GAIA::CH* pszSubject)
		{
			GAST(pszSubject != GNIL);
			if(pszSubject == GNIL)
				GTHROW(InvalidParam);
			if(HPDF_SetInfoAttr(m_ctx->doc, HPDF_INFO_SUBJECT, pszSubject) != HPDF_OK)
				GTHROW(Illegal);
		}
		const GAIA::CH* PDFWriter::GetSubject() const
		{
			return HPDF_GetInfoAttr(m_ctx->doc, HPDF_INFO_SUBJECT);
		}
		GAIA::GVOID PDFWriter::SetKeywords(const GAIA::CH* pszKeywords)
		{
			GAST(pszKeywords != GNIL);
			if(pszKeywords == GNIL)
				GTHROW(InvalidParam);
			if(HPDF_SetInfoAttr(m_ctx->doc, HPDF_INFO_KEYWORDS, pszKeywords) != HPDF_OK)
				GTHROW(Illegal);
		}
		const GAIA::CH* PDFWriter::GetKeywords() const
		{
			return HPDF_GetInfoAttr(m_ctx->doc, HPDF_INFO_KEYWORDS);
		}
		GAIA::GVOID PDFWriter::SetCreateTime(const GAIA::TIME::Time& t)
		{
			HPDF_Date date;
			date.year = t.y;
			date.month = t.mo;
			date.day = t.d;
			date.hour = t.h;
			date.minutes = t.mi;
			date.seconds = t.sec;
			date.ind = ' ';
			date.off_hour = 0;
			date.off_minutes = 0;
			if(HPDF_SetInfoDateAttr(m_ctx->doc, HPDF_INFO_CREATION_DATE, date) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->tCreateTime = t;
		}
		const GAIA::TIME::Time& PDFWriter::GetCreateTime() const
		{
			return m_ctx->tCreateTime;
		}
		GAIA::GVOID PDFWriter::SetModTime(const GAIA::TIME::Time& t)
		{
			HPDF_Date date;
			date.year = t.y;
			date.month = t.mo;
			date.day = t.d;
			date.hour = t.h;
			date.minutes = t.mi;
			date.seconds = t.sec;
			date.ind = ' ';
			date.off_hour = 0;
			date.off_minutes = 0;
			if(HPDF_SetInfoDateAttr(m_ctx->doc, HPDF_INFO_MOD_DATE, date) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->tModTime = t;
		}
		const GAIA::TIME::Time& PDFWriter::GetModTime() const
		{
			return m_ctx->tModTime;
		}
		GAIA::GVOID PDFWriter::SetEncoding(const GAIA::CH* pszEncodingName)
		{
			if(GAIA::ALGO::gstremp(pszEncodingName))
				GTHROW(InvalidParam);
			if(m_ctx->strEncodingName.comparei(pszEncodingName) == 0)
				return;
			HPDF_STATUS resEncoding = HPDF_OK;
			HPDF_STATUS resFont = HPDF_OK;
			if(GAIA::ALGO::gstricmp(pszEncodingName, "CHS") == 0)
			{
				resFont = HPDF_UseCNSFonts(m_ctx->doc);
				resEncoding = HPDF_UseCNSEncodings(m_ctx->doc);
			}
			else if(GAIA::ALGO::gstricmp(pszEncodingName, "CHT") == 0)
			{
				resFont = HPDF_UseCNTFonts(m_ctx->doc);
				resEncoding = HPDF_UseCNTEncodings(m_ctx->doc);
			}
			else if(GAIA::ALGO::gstricmp(pszEncodingName, "KR") == 0)
			{
				resFont = HPDF_UseKRFonts(m_ctx->doc);
				resEncoding = HPDF_UseKREncodings(m_ctx->doc);
			}
			else if(GAIA::ALGO::gstricmp(pszEncodingName, "JP") == 0)
			{
				resFont = HPDF_UseJPFonts(m_ctx->doc);
				resEncoding = HPDF_UseJPEncodings(m_ctx->doc);
			}
			else if(GAIA::ALGO::gstricmp(pszEncodingName, "UTF") == 0)
				resEncoding = HPDF_UseUTFEncodings(m_ctx->doc);
			else
				GTHROW(InvalidParam);
			if(resEncoding != HPDF_OK || resFont != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->strEncodingName = pszEncodingName;
		}
		const GAIA::CH* PDFWriter::GetEncoding() const
		{
			return m_ctx->strEncodingName.fptr();
		}
		GAIA::GVOID PDFWriter::InsertPage(GAIA::NUM sPageIndex)
		{
			GAST(sPageIndex == GINVALID || sPageIndex >= 0 && sPageIndex <= m_ctx->listPage.size());
			if(sPageIndex == GINVALID || sPageIndex == m_ctx->listPage.size())
			{
				HPDF_Page page = HPDF_AddPage(m_ctx->doc);
				if(page == GNIL)
					GTHROW(Illegal);
				m_ctx->listPage.push_back(page);
				m_ctx->page = page;
			}
			else
			{
				if(sPageIndex < 0 || sPageIndex > m_ctx->listPage.size())
					GTHROW(InvalidParam);
				HPDF_Page pageold = m_ctx->listPage[sPageIndex];
				HPDF_Page page = HPDF_InsertPage(m_ctx->doc, pageold);
				if(page == GNIL)
					GTHROW(Illegal);
				m_ctx->listPage.insert(page, sPageIndex);
				m_ctx->page = page;
			}
			if(HPDF_Page_SetExtGState(m_ctx->page, m_ctx->gstate) != HPDF_OK)
				GTHROW(Illegal);

			// Init page.
			{
				// Font name and size.
				HPDF_Font f = GNIL;
				HPDF_Font* pFinded = m_ctx->listFont.find(m_ctx->strFont);
				if(pFinded != GNIL)
					f = *pFinded;
				if(f == GNIL)
				{
					f = HPDF_GetFontEx(m_ctx->doc, m_ctx->strFont);
					m_ctx->listFont.insert(m_ctx->strFont, f);
				}
				if(f == GNIL)
					GTHROW(Illegal);
				if(HPDF_Page_SetFontAndSize(m_ctx->page, f, m_ctx->rFontSize) != HPDF_OK)
					GTHROW(Illegal);

				// Text space.
				if(HPDF_Page_SetTextLeading(m_ctx->page, m_ctx->rLineSpace) != HPDF_OK)
					GTHROW(Illegal);
				if(HPDF_Page_SetWordSpace(m_ctx->page, m_ctx->rWordSpace) != HPDF_OK)
					GTHROW(Illegal);
				if(HPDF_Page_SetCharSpace(m_ctx->page, m_ctx->rCharSpace) != HPDF_OK)
					GTHROW(Illegal);

				// Line state.

				// Color.

				// Alpha.

				// Blend state.
			}
		}
		GAIA::NUM PDFWriter::GetPageCount() const
		{
			return m_ctx->listPage.size();
		}
		GAIA::GVOID PDFWriter::SelectPage(GAIA::NUM sPageIndex)
		{
			GAST(sPageIndex >= 0 && sPageIndex < m_ctx->listPage.size());
			if(sPageIndex < 0 || sPageIndex >= m_ctx->listPage.size())
				GTHROW(InvalidParam);
			m_ctx->page = m_ctx->listPage[sPageIndex];
		}
		GAIA::GVOID PDFWriter::SetPageWidth(GAIA::REAL rWidth)
		{
			GAST(rWidth >= R(0.0));
			if(HPDF_Page_SetWidth(m_ctx->page, rWidth) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::REAL PDFWriter::GetPageWidth() const
		{
			return HPDF_Page_GetWidth(m_ctx->page);
		}
		GAIA::GVOID PDFWriter::SetPageHeight(GAIA::REAL rHeight)
		{
			GAST(rHeight >= R(0.0));
			if(HPDF_Page_SetHeight(m_ctx->page, rHeight) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::REAL PDFWriter::GetPageHeight() const
		{
			return HPDF_Page_GetHeight(m_ctx->page);
		}
		GAIA::GVOID PDFWriter::SetFillRGB(GAIA::REAL r, GAIA::REAL g, GAIA::REAL b)
		{
			GAST(r >= R(0.0));
			GAST(g >= R(0.0));
			GAST(b >= R(0.0));
			if(HPDF_Page_SetRGBFill(m_ctx->page, r, g, b) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::GetFillRGB(GAIA::REAL& r, GAIA::REAL& g, GAIA::REAL& b) const
		{
			HPDF_RGBColor res = HPDF_Page_GetRGBFill(m_ctx->page);
			r = res.r;
			g = res.g;
			b = res.b;
		}
		GAIA::GVOID PDFWriter::SetFillCMYK(GAIA::REAL c, GAIA::REAL m, GAIA::REAL y, GAIA::REAL k)
		{
			GAST(c >= R(0.0));
			GAST(m >= R(0.0));
			GAST(y >= R(0.0));
			GAST(k >= R(0.0));
			if(HPDF_Page_SetCMYKFill(m_ctx->page, c, m, y, k) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::GetFillCMYK(GAIA::REAL& c, GAIA::REAL& m, GAIA::REAL& y, GAIA::REAL& k) const
		{
			HPDF_CMYKColor res = HPDF_Page_GetCMYKFill(m_ctx->page);
			c = res.c;
			m = res.m;
			y = res.y;
			k = res.k;
		}
		GAIA::GVOID PDFWriter::SetStrokeRGB(GAIA::REAL r, GAIA::REAL g, GAIA::REAL b)
		{
			GAST(r >= R(0.0));
			GAST(g >= R(0.0));
			GAST(b >= R(0.0));
			if(HPDF_Page_SetRGBStroke(m_ctx->page, r, g, b) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::GetStrokeRGB(GAIA::REAL& r, GAIA::REAL& g, GAIA::REAL& b) const
		{
			HPDF_RGBColor res = HPDF_Page_GetRGBStroke(m_ctx->page);
			r = res.r;
			g = res.g;
			b = res.b;
		}
		GAIA::GVOID PDFWriter::SetStrokeCMYK(GAIA::REAL c, GAIA::REAL m, GAIA::REAL y, GAIA::REAL k)
		{
			GAST(c >= R(0.0));
			GAST(m >= R(0.0));
			GAST(y >= R(0.0));
			GAST(k >= R(0.0));
			if(HPDF_Page_SetCMYKStroke(m_ctx->page, c, m, y, k) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::GetStrokeCMYK(GAIA::REAL& c, GAIA::REAL& m, GAIA::REAL& y, GAIA::REAL& k) const
		{
			HPDF_CMYKColor res = HPDF_Page_GetCMYKStroke(m_ctx->page);
			c = res.c;
			m = res.m;
			y = res.y;
			k = res.k;
		}
		GAIA::GVOID PDFWriter::SetFillAlpha(GAIA::REAL a)
		{
			GAST(a >= R(0.0));
			if(HPDF_ExtGState_SetAlphaFill(m_ctx->gstate, a) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->rFillAlpha = a;
		}
		GAIA::REAL PDFWriter::GetFillAlpha() const
		{
			return m_ctx->rFillAlpha;
		}
		GAIA::GVOID PDFWriter::SetStrokeAlpha(GAIA::REAL a)
		{
			GAST(a >= R(0.0));
			if(HPDF_ExtGState_SetAlphaStroke(m_ctx->gstate, a) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->rStrokeAlpha = a;
		}
		GAIA::REAL PDFWriter::GetStrokeAlpha() const
		{
			return m_ctx->rStrokeAlpha;
		}
		GAIA::GVOID PDFWriter::SetBlendMode(GAIA::DOC::PDF_BLEND_MODE m)
		{
			HPDF_BlendMode prac = HPDF_BM_NORMAL;
			if(m == GAIA::DOC::PDF_BLEND_MODE_NORMAL)
				prac = HPDF_BM_NORMAL;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_MULTIPLY)
				prac = HPDF_BM_MULTIPLY;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_SCREEN)
				prac = HPDF_BM_SCREEN;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_OVERLAY)
				prac = HPDF_BM_OVERLAY;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_DARKEN)
				prac = HPDF_BM_DARKEN;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_LIGHTEN)
				prac = HPDF_BM_LIGHTEN;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_COLORDODGE)
				prac = HPDF_BM_COLOR_DODGE;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_COLORBUM)
				prac = HPDF_BM_COLOR_BUM;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_HARDLIGHT)
				prac = HPDF_BM_HARD_LIGHT;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_SOFTLIGHT)
				prac = HPDF_BM_SOFT_LIGHT;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_DIFFERENCE)
				prac = HPDF_BM_DIFFERENCE;
			else if(m == GAIA::DOC::PDF_BLEND_MODE_EXCLUSHON)
				prac = HPDF_BM_EXCLUSHON;
			else
				GTHROW(InvalidParam);
			if(HPDF_ExtGState_SetBlendMode(m_ctx->gstate, prac) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->blendmode = m;
		}
		GAIA::DOC::PDF_BLEND_MODE PDFWriter::GetBlendMode() const
		{
			return m_ctx->blendmode;
		}
		GAIA::GVOID PDFWriter::StateSave()
		{
			if(HPDF_Page_GSave(m_ctx->page) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::StateRestore()
		{
			if(HPDF_Page_GRestore(m_ctx->page) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::SetLineWidth(GAIA::REAL rLineWidth)
		{
			GAST(rLineWidth >= R(0.0));
			if(HPDF_Page_SetLineWidth(m_ctx->page, rLineWidth) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::REAL PDFWriter::GetLineWidth() const
		{
			return HPDF_Page_GetLineWidth(m_ctx->page);
		}
		GAIA::GVOID PDFWriter::SetLineCap(GAIA::DOC::PDF_LINE_CAP m)
		{
			HPDF_STATUS res = HPDF_OK;
			if(m == PDF_LINE_CAP_HARD)
				res = HPDF_Page_SetLineCap(m_ctx->page, HPDF_BUTT_END);
			else if(m == PDF_LINE_CAP_ROUND)
				res = HPDF_Page_SetLineCap(m_ctx->page, HPDF_ROUND_END);
			else if(m == PDF_LINE_CAP_SCUARE)
				res = HPDF_Page_SetLineCap(m_ctx->page, HPDF_PROJECTING_SCUARE_END);
			else
				GTHROW(InvalidParam);
			if(res != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::DOC::PDF_LINE_CAP PDFWriter::GetLineCap() const
		{
			HPDF_LineCap cap = HPDF_Page_GetLineCap(m_ctx->page);
			if(cap == HPDF_BUTT_END)
				return PDF_LINE_CAP_HARD;
			else if(cap == HPDF_ROUND_END)
				return PDF_LINE_CAP_ROUND;
			else if(cap == HPDF_PROJECTING_SCUARE_END)
				return PDF_LINE_CAP_SCUARE;
			GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::SetLineJoin(GAIA::DOC::PDF_LINE_JOIN m)
		{
			HPDF_STATUS res = HPDF_OK;
			if(m == PDF_LINE_JOIN_MITER)
				res = HPDF_Page_SetLineJoin(m_ctx->page, HPDF_MITER_JOIN);
			else if(m == PDF_LINE_JOIN_ROUND)
				res = HPDF_Page_SetLineJoin(m_ctx->page, HPDF_ROUND_JOIN);
			else if(m == PDF_LINE_JOIN_BEVEL)
				res = HPDF_Page_SetLineJoin(m_ctx->page, HPDF_BEVEL_JOIN);
			else
				GTHROW(InvalidParam);
			if(res != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::DOC::PDF_LINE_JOIN PDFWriter::GetLineJoin() const
		{
			HPDF_LineJoin join = HPDF_Page_GetLineJoin(m_ctx->page);
			if(join == HPDF_MITER_JOIN)
				return PDF_LINE_JOIN_MITER;
			else if(join == HPDF_ROUND_JOIN)
				return PDF_LINE_JOIN_ROUND;
			else if(join == HPDF_BEVEL_JOIN)
				return PDF_LINE_JOIN_BEVEL;
			else
				GTHROW(InvalidParam);
		}
		GAIA::GVOID PDFWriter::SetDash(const GAIA::U16* pPattern, GAIA::U32 uCount, GAIA::U32 uPhase)
		{
			if(HPDF_Page_SetDash(m_ctx->page, pPattern, uCount, uPhase) != HPDF_OK)
				GTHROW(Illegal);

		}
		GAIA::GVOID PDFWriter::GetDash(GAIA::U16* pPattern, GAIA::U32& uCount, GAIA::U32& uPhase) const
		{
			if(pPattern == GNIL)
				GTHROW(InvalidParam);
			HPDF_DashMode m = HPDF_Page_GetDash(m_ctx->page);
			uCount = m.num_ptn;
			uPhase = m.phase;
			for(GAIA::U32 x = 0; x < m.num_ptn; ++x)
				pPattern[x] = m.ptn[x];
		}
		GAIA::GVOID PDFWriter::MoveTo(GAIA::REAL x, GAIA::REAL y)
		{
			if(HPDF_Page_MoveTo(m_ctx->page, x, y) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::Pos(GAIA::REAL& x, GAIA::REAL& y) const
		{
			HPDF_Point p = HPDF_Page_GetCurrentPos(m_ctx->page);
			x = p.x;
			y = p.y;
		}
		GAIA::GVOID PDFWriter::Fill()
		{
			if(HPDF_Page_Fill(m_ctx->page) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::Stroke()
		{
			if(HPDF_Page_Stroke(m_ctx->page) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::FillStroke()
		{
			if(HPDF_Page_FillStroke(m_ctx->page) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::Rectangle(GAIA::REAL x, GAIA::REAL y, GAIA::REAL rWidth, GAIA::REAL rHeight)
		{
			GAST(rWidth >= R(0.0));
			GAST(rHeight >= R(0.0));
			if(HPDF_Page_Rectangle(m_ctx->page, x, y, rWidth, rHeight) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::Circle(GAIA::REAL x, GAIA::REAL y, GAIA::REAL rRadius)
		{
			GAST(rRadius >= R(0.0));
			if(HPDF_Page_Circle(m_ctx->page, x, y, rRadius) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::Ellipse(GAIA::REAL x, GAIA::REAL y, GAIA::REAL xr, GAIA::REAL yr)
		{
			GAST(xr >= R(0.0));
			GAST(yr >= R(0.0));
			if(HPDF_Page_Ellipse(m_ctx->page, x, y, xr, yr) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::Arc(GAIA::REAL x, GAIA::REAL y, GAIA::REAL rRadius, GAIA::REAL rAngle1, GAIA::REAL rAngle2)
		{
			GAST(rRadius >= R(0.0));
			if(HPDF_Page_Arc(m_ctx->page, x, y, rRadius, rAngle1, rAngle2) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::LineTo(GAIA::REAL x, GAIA::REAL y)
		{
			if(HPDF_Page_LineTo(m_ctx->page, x, y) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::CurveTo(GAIA::REAL x, GAIA::REAL y, GAIA::REAL xtan, GAIA::REAL ytan)
		{
			if(HPDF_Page_CurveTo2(m_ctx->page, xtan, ytan, x, y) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::CurveTo(GAIA::REAL x, GAIA::REAL y, GAIA::REAL xtan1, GAIA::REAL ytan1, GAIA::REAL xtan2, GAIA::REAL ytan2)
		{
			if(HPDF_Page_CurveTo(m_ctx->page, xtan1, ytan1, xtan2, ytan2, x, y) != HPDF_OK)
				GTHROW(Illegal);
		}
		const GAIA::CH* PDFWriter::PrepareFont(const GAIA::CH* pszFontFileName, GAIA::BL bEmbedToDoc)
		{
			GAST(!GAIA::ALGO::gstremp(pszFontFileName));
			if(GAIA::ALGO::gstremp(pszFontFileName))
				GTHROW(InvalidParam);
			const GAIA::CH* pszFontName = HPDF_LoadTTFontFromFile(m_ctx->doc, pszFontFileName, bEmbedToDoc);
			if(GAIA::ALGO::gstremp(pszFontName))
				return GNIL;
			return pszFontName;
		}
		GAIA::GVOID PDFWriter::BeginText()
		{
			if(this->IsBeginText())
				GTHROW(Illegal);
			if(HPDF_Page_BeginText(m_ctx->page) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->bBeginText = GAIA::True;
		}
		GAIA::GVOID PDFWriter::EndText()
		{
			if(!this->IsBeginText())
				GTHROW(Illegal);
			if(HPDF_Page_EndText(m_ctx->page) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->bBeginText = GAIA::False;
		}
		GAIA::BL PDFWriter::IsBeginText() const
		{
			return m_ctx->bBeginText;
		}
		GAIA::GVOID PDFWriter::SetFont(const GAIA::CH* pszFont)
		{
			GAST(!GAIA::ALGO::gstremp(pszFont));
			if(GAIA::ALGO::gstremp(pszFont))
				GTHROW(InvalidParam);
			HPDF_Font f = GNIL;
			HPDF_Font* pFinded = m_ctx->listFont.find(pszFont);
			if(pFinded != GNIL)
				f = *pFinded;
			if(f == GNIL)
			{
				f = HPDF_GetFontEx(m_ctx->doc, pszFont);
				m_ctx->listFont.insert(pszFont, f);
			}
			if(f == GNIL)
				GTHROW(Illegal);
			if(HPDF_Page_SetFontAndSize(m_ctx->page, f, m_ctx->rFontSize) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->strFont = pszFont;
		}
		const GAIA::CH* PDFWriter::GetFont() const
		{
			return m_ctx->strFont.fptr();
		}
		GAIA::GVOID PDFWriter::SetFontSize(GAIA::REAL rFontSize)
		{
			GAST(rFontSize >= R(0.0));
			if(rFontSize < R(0.0))
				GTHROW(InvalidParam);
			HPDF_Font f = GNIL;
			HPDF_Font* pFinded = m_ctx->listFont.find(m_ctx->strFont.fptr());
			if(pFinded != GNIL)
				f = *pFinded;
			if(f == GNIL)
			{
				f = HPDF_GetFontEx(m_ctx->doc, m_ctx->strFont.fptr());
				m_ctx->listFont.insert(m_ctx->strFont.fptr(), f);
			}
			if(f == GNIL)
				GTHROW(Illegal);
			if(HPDF_Page_SetFontAndSize(m_ctx->page, f, rFontSize) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->rFontSize = rFontSize;
		}
		GAIA::REAL PDFWriter::GetFontSize() const
		{
			return m_ctx->rFontSize;
		}
		GAIA::GVOID PDFWriter::SetLineSpace(GAIA::REAL rSpace)
		{
			GAST(rSpace >= R(0.0));
			if(rSpace < R(0.0))
				GTHROW(InvalidParam);
			if(HPDF_Page_SetTextLeading(m_ctx->page, rSpace) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->rLineSpace = rSpace;
		}
		GAIA::REAL PDFWriter::GetLineSpace() const
		{
			return m_ctx->rLineSpace;
		}
		GAIA::GVOID PDFWriter::SetWordSpace(GAIA::REAL rSpace)
		{
			GAST(rSpace >= R(0.0));
			if(rSpace < R(0.0))
				GTHROW(InvalidParam);
			if(HPDF_Page_SetWordSpace(m_ctx->page, rSpace) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->rWordSpace = rSpace;
		}
		GAIA::REAL PDFWriter::GetWordSpace() const
		{
			return m_ctx->rWordSpace;
		}
		GAIA::GVOID PDFWriter::SetCharSpace(GAIA::REAL rSpace)
		{
			GAST(rSpace >= R(0.0));
			if(rSpace < R(0.0))
				GTHROW(InvalidParam);
			if(HPDF_Page_SetCharSpace(m_ctx->page, rSpace) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->rCharSpace = rSpace;
		}
		GAIA::REAL PDFWriter::GetCharSpace() const
		{
			return m_ctx->rCharSpace;
		}
		GAIA::GVOID PDFWriter::SetTextVOffset(GAIA::REAL rOffset)
		{
			if(HPDF_Page_SetTextRaise(m_ctx->page, rOffset) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::REAL PDFWriter::GetTextVOffset() const
		{
			return HPDF_Page_GetTextRaise(m_ctx->page);
		}
		GAIA::GVOID PDFWriter::SetTextMatrix(GAIA::REAL x, GAIA::REAL y, GAIA::REAL a, GAIA::REAL b, GAIA::REAL c, GAIA::REAL d)
		{
			if(HPDF_Page_SetTextMatrix(m_ctx->page, a, b, c, d, x, y) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::SetTextRenderMode(GAIA::DOC::PDF_TEXT_RENDER_MODE m)
		{
			HPDF_STATUS res = HPDF_OK;
			if(m == GAIA::DOC::PDF_TEXT_RENDER_MODE_FILL)
				res = HPDF_Page_SetTextRenderingMode(m_ctx->page, HPDF_FILL);
			else if(m == GAIA::DOC::PDF_TEXT_RENDER_MODE_STROKE)
				res = HPDF_Page_SetTextRenderingMode(m_ctx->page, HPDF_STROKE);
			else if(m == GAIA::DOC::PDF_TEXT_RENDER_MODE_FILLSTROKE)
				res = HPDF_Page_SetTextRenderingMode(m_ctx->page, HPDF_FILL_THEN_STROKE);
			else if(m == GAIA::DOC::PDF_TEXT_RENDER_MODE_INVISIBLE)
				res = HPDF_Page_SetTextRenderingMode(m_ctx->page, HPDF_INVISIBLE);
			else if(m == GAIA::DOC::PDF_TEXT_RENDER_MODE_FILLCLIP)
				res = HPDF_Page_SetTextRenderingMode(m_ctx->page, HPDF_FILL_CLIPPING);
			else if(m == GAIA::DOC::PDF_TEXT_RENDER_MODE_STROKECLIP)
				res = HPDF_Page_SetTextRenderingMode(m_ctx->page, HPDF_STROKE_CLIPPING);
			else if(m == GAIA::DOC::PDF_TEXT_RENDER_MODE_FILLSTROKECLIP)
				res = HPDF_Page_SetTextRenderingMode(m_ctx->page, HPDF_FILL_STROKE_CLIPPING);
			else if(m == GAIA::DOC::PDF_TEXT_RENDER_MODE_CLIP)
				res = HPDF_Page_SetTextRenderingMode(m_ctx->page, HPDF_CLIPPING);
			else
				GTHROW(InvalidParam);
			if(res != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::DOC::PDF_TEXT_RENDER_MODE PDFWriter::GetTextRenderMode() const
		{
			HPDF_TextRenderingMode pracmode = HPDF_Page_GetTextRenderingMode(m_ctx->page);
			if(pracmode == HPDF_FILL)
				return GAIA::DOC::PDF_TEXT_RENDER_MODE_FILL;
			else if(pracmode == HPDF_STROKE)
				return GAIA::DOC::PDF_TEXT_RENDER_MODE_STROKE;
			else if(pracmode == HPDF_FILL_THEN_STROKE)
				return GAIA::DOC::PDF_TEXT_RENDER_MODE_FILLSTROKE;
			else if(pracmode == HPDF_INVISIBLE)
				return GAIA::DOC::PDF_TEXT_RENDER_MODE_INVISIBLE;
			else if(pracmode == HPDF_FILL_CLIPPING)
				return GAIA::DOC::PDF_TEXT_RENDER_MODE_FILLCLIP;
			else if(pracmode == HPDF_STROKE)
				return GAIA::DOC::PDF_TEXT_RENDER_MODE_STROKECLIP;
			else if(pracmode == HPDF_FILL_STROKE_CLIPPING)
				return GAIA::DOC::PDF_TEXT_RENDER_MODE_FILLSTROKECLIP;
			else if(pracmode == HPDF_CLIPPING)
				return GAIA::DOC::PDF_TEXT_RENDER_MODE_CLIP;
			else
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::MoveTextTo(GAIA::REAL x, GAIA::REAL y)
		{
			if(HPDF_Page_MoveTextPos(m_ctx->page, x, y) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::TextPos(GAIA::REAL& x, GAIA::REAL& y) const
		{
			HPDF_Point p = HPDF_Page_GetCurrentTextPos(m_ctx->page);
			x = p.x;
			y = p.y;
		}
		GAIA::GVOID PDFWriter::Text(const GAIA::CH* pszText, GAIA::BL bNewLine)
		{
			GAST(pszText != GNIL);
			if(pszText == GNIL)
				GTHROW(InvalidParam);
			if(pszText[0] == '\0')
				return;
			if(bNewLine)
			{
				if(HPDF_Page_ShowTextNextLine(m_ctx->page, pszText) != HPDF_OK)
					GTHROW(Illegal);
			}
			else
			{
				if(HPDF_Page_ShowText(m_ctx->page, pszText) != HPDF_OK)
					GTHROW(Illegal);
			}
		}
		GAIA::GVOID PDFWriter::Text(GAIA::REAL x, GAIA::REAL y, const GAIA::CH* pszText)
		{
			GAST(pszText != GNIL);
			if(pszText == GNIL)
				GTHROW(InvalidParam);
			if(pszText[0] == '\0')
				return;
			if(HPDF_Page_TextOut(m_ctx->page, x, y, pszText) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::Text(GAIA::REAL rLeft, GAIA::REAL rTop, GAIA::REAL rRight, GAIA::REAL rBottom, const GAIA::CH* pszText, GAIA::DOC::PDF_TEXT_ALIGN align)
		{
			GAST(pszText != GNIL);
			if(pszText == GNIL)
				GTHROW(InvalidParam);
			HPDF_TextAlignment pracalign;
			if(align == PDF_TEXT_ALIGN_LEFT)
				pracalign = HPDF_TALIGN_LEFT;
			else if(align == PDF_TEXT_ALIGN_CENTER)
				pracalign = HPDF_TALIGN_CENTER;
			else if(align == PDF_TEXT_ALIGN_RIGHT)
				pracalign = HPDF_TALIGN_RIGHT;
			else if(align == PDF_TEXT_ALIGN_JUSTIFY)
				pracalign = HPDF_TALIGN_JUSTIFY;
			else
				pracalign = HPDF_TALIGN_LEFT;
			GAIA::U32 uLen;
			if(HPDF_Page_TextRect(m_ctx->page, rLeft, rTop, rRight, rBottom, pszText, pracalign, &uLen) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::REAL PDFWriter::GetTextWidth(const GAIA::CH* pszText)
		{
			GAST(pszText != GNIL);
			if(pszText == GNIL)
				GTHROW(InvalidParam);
			if(pszText[0] == '\0')
				return R(0.0);
			return HPDF_Page_TextWidth(m_ctx->page, pszText);
		}
		GAIA::NUM PDFWriter::MeatureText(const GAIA::CH* pszText, GAIA::REAL rWidth, GAIA::BL bWordWrap, GAIA::REAL* pRealWidth)
		{
			GAST(pszText != GNIL);
			if(pszText == GNIL)
				GTHROW(InvalidParam);
			if(pszText[0] == '\0')
			{
				if(pRealWidth != GNIL)
					*pRealWidth = 0;
				return 0;
			}
			GAST(rWidth > R(0.0));
			if(rWidth <= R(0.0))
				GTHROW(InvalidParam);
			HPDF_REAL rRealWidth;
			HPDF_UINT uMeasureResult = HPDF_Page_MeasureText(m_ctx->page, pszText, rWidth, bWordWrap, &rRealWidth);
			if(pRealWidth != GNIL)
				*pRealWidth = rRealWidth;
			return uMeasureResult;
		}
		GAIA::GVOID PDFWriter::DrawImage(GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::CH* pszFileName, const GAIA::CH* pszImageType)
		{
			GAST(!GAIA::ALGO::gstremp(pszFileName));
			if(GAIA::ALGO::gstremp(pszFileName))
				GTHROW(InvalidParam);
			if(pszImageType != GNIL)
			{
				GAST(pszImageType[0] != '\0');
				if(pszImageType[0] == '\0')
					GTHROW(InvalidParam);
			}
			HPDF_Image img = GNIL;
			HPDF_Image* pFinded = m_ctx->listImage.find(pszFileName);
			if(pFinded != GNIL)
				img = *pFinded;
			if(img == GNIL)
			{
				if(pszImageType == GNIL)
				{
					const GAIA::CH* pszExt = GAIA::ALGO::gstrext(pszFileName);
					if(GAIA::ALGO::gstremp(pszExt))
						return;
					if(GAIA::ALGO::gstricmp(pszExt, PDF_SUPPORT_IMAGE_PNG) == 0)
						pszImageType = PDF_SUPPORT_IMAGE_PNG;
					else if(GAIA::ALGO::gstricmp(pszExt, PDF_SUPPORT_IMAGE_JPEG) == 0)
						pszImageType = PDF_SUPPORT_IMAGE_JPEG;
					else if(GAIA::ALGO::gstricmp(pszExt, PDF_SUPPORT_IMAGE_RAW) == 0)
						pszImageType = PDF_SUPPORT_IMAGE_RAW;
					else
						GTHROW(InvalidParam);
				}
				if(GAIA::ALGO::gstricmp(pszImageType, PDF_SUPPORT_IMAGE_PNG) == 0)
					img = HPDF_LoadPngImageFromFile(m_ctx->doc, pszFileName);
				else if(GAIA::ALGO::gstricmp(pszImageType, PDF_SUPPORT_IMAGE_JPEG) == 0)
					img = HPDF_LoadJpegImageFromFile(m_ctx->doc, pszFileName);
				else if(GAIA::ALGO::gstricmp(pszImageType, PDF_SUPPORT_IMAGE_RAW) == 0)
					GASTFALSE;
				else
					GASTFALSE;
			}
			if(img == GNIL)
				GTHROW(Illegal);
			if(HPDF_Page_DrawImage(m_ctx->page, img, x, y, w, h) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::DrawImage(GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::GVOID* pBuf, GAIA::NUM sLen, const GAIA::CH* pszImageType)
		{
			GAST(pBuf != GNIL);
			if(pBuf == GNIL)
				GTHROW(InvalidParam);
			GAST(sLen > 0);
			if(sLen <= 0)
				GTHROW(InvalidParam);
			GAST(!GAIA::ALGO::gstremp(pszImageType));
			if(GAIA::ALGO::gstremp(pszImageType))
				GTHROW(InvalidParam);
			HPDF_Image img = GNIL;
			if(GAIA::ALGO::gstricmp(pszImageType, PDF_SUPPORT_IMAGE_PNG) == 0)
				img = HPDF_LoadPngImageFromMem(m_ctx->doc, (const HPDF_BYTE*)pBuf, sLen);
			else if(GAIA::ALGO::gstricmp(pszImageType, PDF_SUPPORT_IMAGE_JPEG) == 0)
				img = HPDF_LoadJpegImageFromMem(m_ctx->doc, (const HPDF_BYTE*)pBuf, sLen);
			else if(GAIA::ALGO::gstricmp(pszImageType, PDF_SUPPORT_IMAGE_RAW) == 0)
				GTHROW(InvalidParam);
			else
				GTHROW(InvalidParam);
			if(img == GNIL)
				GTHROW(Illegal);
			if(HPDF_Page_DrawImage(m_ctx->page, img, x, y, w, h) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::DrawImage(GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::GVOID* pBuf, GAIA::NUM sBufWidth, GAIA::NUM sBufHeight, GAIA::NUM sBitCountPerPixel)
		{
			GAST(pBuf != GNIL);
			if(pBuf == GNIL)
				GTHROW(InvalidParam);
			GAST(sBufWidth > 0);
			if(sBufWidth <= 0)
				GTHROW(InvalidParam);
			GAST(sBufHeight > 0);
			if(sBufHeight <= 0)
				GTHROW(InvalidParam);
			GAST(sBitCountPerPixel > 0);
			if(sBitCountPerPixel <= 0)
				GTHROW(InvalidParam);
			HPDF_ColorSpace cs;
			HPDF_UINT bpc; // Bit count per channel.
			if(sBitCountPerPixel == 8)
			{
				cs = HPDF_CS_DEVICE_GRAY;
				bpc = 8;
			}
			else if(sBitCountPerPixel == 24)
			{
				cs = HPDF_CS_DEVICE_RGB;
				bpc = 8;
			}
			else if(sBitCountPerPixel == 32)
			{
				cs = HPDF_CS_DEVICE_CMYK;
				bpc = 8;
			}
			else
				GTHROW(InvalidParam);
			HPDF_Image img = HPDF_LoadRawImageFromMem(m_ctx->doc, (const HPDF_BYTE*)pBuf, sBufWidth, sBufHeight, cs, bpc);
			if(img == GNIL)
				GTHROW(Illegal);
			if(HPDF_Page_DrawImage(m_ctx->page, img, x, y, w, h) != HPDF_OK)
				GTHROW(Illegal);
		}
		GAIA::GVOID* PDFWriter::MakeCatalog(GAIA::GVOID* pParent, const GAIA::CH* pszName, GAIA::NUM sPageIndex, GAIA::GVOID* pEncoder)
		{
			GAST(!GAIA::ALGO::gstremp(pszName));
			if(GAIA::ALGO::gstremp(pszName))
				GTHROW(InvalidParam);
			GAST(sPageIndex >= 0 && sPageIndex < m_ctx->listPage.size());
			if(sPageIndex < 0 || sPageIndex >= m_ctx->listPage.size())
				GTHROW(InvalidParam);
			HPDF_Outline outline = HPDF_CreateOutline(m_ctx->doc, (HPDF_Outline)pParent, pszName, (HPDF_Encoder)pEncoder);
			if(outline == GNIL)
				GTHROW(Illegal);
			HPDF_Page page = m_ctx->listPage[sPageIndex];
			HPDF_Destination dst = HPDF_Page_CreateDestination(page);
			if(HPDF_Destination_SetXYZ(dst, 0, 0, 1) != HPDF_OK)
				GTHROW(Illegal);
			if(HPDF_Outline_SetDestination(outline, dst) != HPDF_OK)
				GTHROW(Illegal);
			return outline;
		}
		GAIA::GVOID PDFWriter::MakeLink(GAIA::NUM sSrcPageIndex, GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, GAIA::NUM sDstPageIndex)
		{
			GAST(sSrcPageIndex >= 0 && sSrcPageIndex < m_ctx->listPage.size());
			if(sSrcPageIndex < 0 || sSrcPageIndex >= m_ctx->listPage.size())
				GTHROW(InvalidParam);
			GAST(w > 0);
			if(w <= 0)
				GTHROW(InvalidParam);
			GAST(h > 0);
			if(h <= 0)
				GTHROW(InvalidParam);
			GAST(sDstPageIndex >= 0 && sDstPageIndex < m_ctx->listPage.size());
			if(sDstPageIndex < 0 || sDstPageIndex >= m_ctx->listPage.size())
				GTHROW(InvalidParam);
			HPDF_Page srcpage = m_ctx->listPage[sSrcPageIndex];
			HPDF_Page dstpage = m_ctx->listPage[sDstPageIndex];
			HPDF_Destination dst = HPDF_Page_CreateDestination(dstpage);
			if(dst == GNIL)
				GTHROW(Illegal);
			HPDF_Rect rc;
			rc.left = x;
			rc.top = y - h;
			rc.right = x + w;
			rc.bottom = y;
			HPDF_Annotation anno = HPDF_Page_CreateLinkAnnot(srcpage, rc, dst);
			if(anno == GNIL)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::MakeURLLink(GAIA::NUM sPageIndex, GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::CH* pszURL)
		{
			GAST(sPageIndex >= 0 && sPageIndex < m_ctx->listPage.size());
			if(sPageIndex < 0 || sPageIndex >= m_ctx->listPage.size())
				GTHROW(InvalidParam);
			GAST(w > 0);
			if(w <= 0)
				GTHROW(InvalidParam);
			GAST(h > 0);
			if(h <= 0)
				GTHROW(InvalidParam);
			GAST(!GAIA::ALGO::gstremp(pszURL));
			if(GAIA::ALGO::gstremp(pszURL))
				GTHROW(InvalidParam);
			HPDF_Page page = m_ctx->listPage[sPageIndex];
			HPDF_Rect rc;
			rc.left = x;
			rc.top = y - h;
			rc.right = x + w;
			rc.bottom = y;
			HPDF_Annotation anno = HPDF_Page_CreateURILinkAnnot(page, rc, pszURL);
			if(anno == GNIL)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::MakeComment(GAIA::NUM sPageIndex, GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::CH* pszText)
		{
			GAST(sPageIndex >= 0 && sPageIndex < m_ctx->listPage.size());
			if(sPageIndex < 0 || sPageIndex >= m_ctx->listPage.size())
				GTHROW(InvalidParam);
			GAST(w > 0);
			if(w <= 0)
				GTHROW(InvalidParam);
			GAST(h > 0);
			if(h <= 0)
				GTHROW(InvalidParam);
			GAST(pszText != GNIL);
			if(pszText == GNIL)
				GTHROW(InvalidParam);
			if(pszText[0] == '\0')
				return;
			HPDF_Page page = m_ctx->listPage[sPageIndex];
			HPDF_Rect rc;
			rc.left = x;
			rc.top = y - h;
			rc.right = x + w;
			rc.bottom = y;
			HPDF_Annotation anno = HPDF_Page_CreateTextAnnot(page, rc, pszText, GNIL);
			if(anno == GNIL)
				GTHROW(Illegal);
		}
		GAIA::GVOID PDFWriter::SetCompressMode(GAIA::U32 uFlag)
		{
			GAIA::U32 uPracFlag = 0;
			if(uFlag & PDF_COMPRESS_TEXT)
				uPracFlag |= HPDF_COMP_TEXT;
			if(uFlag & PDF_COMPRESS_IMAGE)
				uPracFlag |= HPDF_COMP_IMAGE;
			if(uFlag & PDF_COMPRESS_META)
				uPracFlag |= HPDF_COMP_METADATA;
			if(HPDF_SetCompressionMode(m_ctx->doc, uPracFlag) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->uCompressModeFlag = uFlag;
		}
		GAIA::U32 PDFWriter::GetCompressMode() const
		{
			return m_ctx->uCompressModeFlag;
		}
		GAIA::GVOID PDFWriter::SetPermission(GAIA::U32 uFlag)
		{
			GAIA::U32 uPracFlag = 0;
			if(uFlag & PDF_PERMISSION_READ)
				uPracFlag |= HPDF_ENABLE_READ;
			if(uFlag & PDF_PERMISSION_COPY)
				uPracFlag |= HPDF_ENABLE_COPY;
			if(uFlag & PDF_PERMISSION_EDIT)
				uPracFlag |= HPDF_ENABLE_EDIT;
			if(uFlag & PDF_PERMISSION_PRINT)
				uPracFlag |= HPDF_ENABLE_PRINT;
			if(uFlag & PDF_PERMISSION_EDITALL)
				uPracFlag |= HPDF_ENABLE_EDIT_ALL;
			if(HPDF_SetPermission(m_ctx->doc, uPracFlag) != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->uPermissionFlag = uFlag;
		}
		GAIA::U32 PDFWriter::GetPermission() const
		{
			return m_ctx->uPermissionFlag;
		}
		GAIA::GVOID PDFWriter::SetCreatorPassword(const GAIA::CH* pszCreatorPassword)
		{
			GAST(pszCreatorPassword != GNIL);
			if(pszCreatorPassword == GNIL)
				GTHROW(InvalidParam);
			HPDF_STATUS res = HPDF_SetPassword(m_ctx->doc,
							 GAIA::ALGO::gstremp(pszCreatorPassword) ? "" : pszCreatorPassword,
							 m_ctx->strReaderPassword.empty() ? "" : m_ctx->strReaderPassword.fptr());
			if(res != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->strCreatorPassword = pszCreatorPassword;
		}
		const GAIA::CH* PDFWriter::GetCreatorPassword() const
		{
			return m_ctx->strCreatorPassword.fptr();
		}
		GAIA::GVOID PDFWriter::SetReaderPassword(const GAIA::CH* pszReaderPassword)
		{
			GAST(pszReaderPassword != GNIL);
			if(pszReaderPassword == GNIL)
				GTHROW(InvalidParam);
			HPDF_STATUS res = HPDF_SetPassword(m_ctx->doc,
							 m_ctx->strCreatorPassword.empty() ? "" : m_ctx->strCreatorPassword.fptr(),
							 GAIA::ALGO::gstremp(pszReaderPassword) ? "" : pszReaderPassword);
			if(res != HPDF_OK)
				GTHROW(Illegal);
			m_ctx->strReaderPassword = pszReaderPassword;
		}
		const GAIA::CH* PDFWriter::GetReaderPassword() const
		{
			return m_ctx->strReaderPassword.fptr();
		}
		GAIA::BL PDFWriter::Save(const GAIA::CH* pszFileName)
		{
			if(GAIA::ALGO::gstremp(pszFileName))
			{
				if(HPDF_SaveToStream(m_ctx->doc) != HPDF_OK)
					return GAIA::False;
			}
			else
			{
				if(HPDF_SaveToFile(m_ctx->doc, pszFileName) != HPDF_OK)
					return GAIA::False;
			}
			return GAIA::True;
		}
		GAIA::GVOID PDFWriter::OnError(GAIA::U64 uError, GAIA::U64 uDetail)
		{
			for(GAIA::NUM x = 0; x < m_ctx->listCallBack.size(); ++x)
				m_ctx->listCallBack[x]->OnError(uError, uDetail, "");
		}
	}
}
