#ifndef		__GAIA_DOC_PDF_H__
#define		__GAIA_DOC_PDF_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_time.h"

namespace GAIA
{
	namespace DOC
	{
		/*!
			@brief
		*/
		GAIA_ENUM_BEGIN(PDF_TEXT_ALIGN)
			PDF_TEXT_ALIGN_LEFT,
			PDF_TEXT_ALIGN_RIGHT,
			PDF_TEXT_ALIGN_CENTER,
			PDF_TEXT_ALIGN_JUSTIFY,
		GAIA_ENUM_END(PDF_TEXT_ALIGN)

		/*!
			@brief
		*/
		GAIA_ENUM_BEGIN(PDF_LINE_CAP)
			PDF_LINE_CAP_HARD,
			PDF_LINE_CAP_ROUND,
			PDF_LINE_CAP_SCUARE,
		GAIA_ENUM_END(PDF_LINE_CAP)

		/*!
			@brief
		*/
		GAIA_ENUM_BEGIN(PDF_LINE_JOIN)
			PDF_LINE_JOIN_MITER,
			PDF_LINE_JOIN_ROUND,
			PDF_LINE_JOIN_BEVEL,
		GAIA_ENUM_END(PDF_LINE_JOIN)

		/*!
			@brief
		*/
		GAIA_ENUM_BEGIN(PDF_TEXT_RENDER_MODE)
			PDF_TEXT_RENDER_MODE_FILL,
			PDF_TEXT_RENDER_MODE_STROKE,
			PDF_TEXT_RENDER_MODE_FILLSTROKE,
			PDF_TEXT_RENDER_MODE_INVISIBLE,
			PDF_TEXT_RENDER_MODE_FILLCLIP,
			PDF_TEXT_RENDER_MODE_STROKECLIP,
			PDF_TEXT_RENDER_MODE_FILLSTROKECLIP,
			PDF_TEXT_RENDER_MODE_CLIP,
		GAIA_ENUM_END(PDF_TEXT_RENDER_MODE)

		/*!
			@brief
		*/
		GAIA_ENUM_BEGIN(PDF_BLEND_MODE)
			PDF_BLEND_MODE_NORMAL,
			PDF_BLEND_MODE_MULTIPLY,
			PDF_BLEND_MODE_SCREEN,
			PDF_BLEND_MODE_OVERLAY,
			PDF_BLEND_MODE_DARKEN,
			PDF_BLEND_MODE_LIGHTEN,
			PDF_BLEND_MODE_COLORDODGE,
			PDF_BLEND_MODE_COLORBUM,
			PDF_BLEND_MODE_HARDLIGHT,
			PDF_BLEND_MODE_SOFTLIGHT,
			PDF_BLEND_MODE_DIFFERENCE,
			PDF_BLEND_MODE_EXCLUSHON,
		GAIA_ENUM_END(PDF_BLEND_MODE)

		static const GAIA::U32 PDF_COMPRESS_TEXT = 0x0001;
		static const GAIA::U32 PDF_COMPRESS_IMAGE = 0x0002;
		static const GAIA::U32 PDF_COMPRESS_META = 0x0004;

		static const GAIA::U32 PDF_PERMISSION_READ = 0x0001;
		static const GAIA::U32 PDF_PERMISSION_COPY = 0x0002;
		static const GAIA::U32 PDF_PERMISSION_EDIT = 0x0004;
		static const GAIA::U32 PDF_PERMISSION_PRINT = 0x0008;
		static const GAIA::U32 PDF_PERMISSION_EDITALL = 0x0010;

		class PDFWriterContext;
		class PDFWriter : public GAIA::Base
		{
		public:
			/*!
				@brief
			*/
			class CallBack : public GAIA::Base
			{
			public:
				/*!
					@brief
				*/
				virtual GAIA::GVOID OnError(GAIA::U64 uError, GAIA::U64 uDetail, const GAIA::CH* pszError){}

			public:
				/*!
					@brief
				*/
				CallBack(){this->init();}
				/*!
					@brief
				*/
				PDFWriter& GetPDFWriter(){return *m_pPDFWriter;}

			private:
				GAIA::GVOID init(){m_pPDFWriter = GNIL;}
				GAIA::GVOID SetPDFWriter(PDFWriter* pWriter){m_pPDFWriter = pWriter;}

			private:
				PDFWriter* m_pPDFWriter;
			};

		public:
			/*!
				@brief
			*/
			PDFWriter();

			/*!
				@brief
			*/
			~PDFWriter();

			// CallBack management.
			/*!
				@brief
			*/
			GAIA::GVOID RegistCallBack(GAIA::DOC::PDFWriter::CallBack& cb);

			/*!
				@brief
			*/
			GAIA::GVOID UnregistCallBack(GAIA::DOC::PDFWriter::CallBack& cb);

			/*!
				@brief
			*/
			GAIA::BL IsRegistedCallBack(GAIA::DOC::PDFWriter::CallBack& cb) const;

			/*!
				@brief
			*/
			GAIA::GVOID UnregistCallBackAll();

			// Document info management.
			/*!
				@brief
			*/
			GAIA::GVOID SetCreator(const GAIA::CH* pszCreator);

			/*!
				@brief
			*/
			const GAIA::CH* GetCreator() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetAuthor(const GAIA::CH* pszAuthor);

			/*!
				@brief
			*/
			const GAIA::CH* GetAuthor() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetTitle(const GAIA::CH* pszTitle);

			/*!
				@brief
			*/
			const GAIA::CH* GetTitle() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetSubject(const GAIA::CH* pszSubject);

			/*!
				@brief
			*/
			const GAIA::CH* GetSubject() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetKeywords(const GAIA::CH* pszKeywords);

			/*!
				@brief
			*/
			const GAIA::CH* GetKeywords() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetCreateTime(const GAIA::TIME::Time& t);

			/*!
				@brief
			*/
			const GAIA::TIME::Time& GetCreateTime() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetModTime(const GAIA::TIME::Time& t);

			/*!
				@brief
			*/
			const GAIA::TIME::Time& GetModTime() const;

			// Encoding management.
			/*!
				@brief
			*/
			GAIA::GVOID SetEncoding(const GAIA::CH* pszEncodingName);

			/*!
				@brief
			*/
			const GAIA::CH* GetEncoding() const;

			// Page management.
			/*!
				@brief
			*/
			GAIA::GVOID InsertPage(GAIA::NUM sPageIndex = GINVALID);

			/*!
				@brief
			*/
			GAIA::NUM GetPageCount() const;

			/*!
				@brief
			*/
			GAIA::GVOID SelectPage(GAIA::NUM sPageIndex = GINVALID);

			/*!
				@brief
			*/
			GAIA::GVOID SetPageWidth(GAIA::REAL rWidth);

			/*!
				@brief
			*/
			GAIA::REAL GetPageWidth() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetPageHeight(GAIA::REAL rHeight);

			/*!
				@brief
			*/
			GAIA::REAL GetPageHeight() const;

			// Color management.
			/*!
				@brief
			*/
			GAIA::GVOID SetFillRGB(GAIA::REAL r, GAIA::REAL g, GAIA::REAL b);

			/*!
				@brief
			*/
			GAIA::GVOID GetFillRGB(GAIA::REAL& r, GAIA::REAL& g, GAIA::REAL& b) const;

			/*!
				@brief
			*/
			GAIA::GVOID SetFillCMYK(GAIA::REAL c, GAIA::REAL m, GAIA::REAL y, GAIA::REAL k);

			/*!
				@brief
			*/
			GAIA::GVOID GetFillCMYK(GAIA::REAL& c, GAIA::REAL& m, GAIA::REAL& y, GAIA::REAL& k) const;

			/*!
				@brief
			*/
			GAIA::GVOID SetStrokeRGB(GAIA::REAL r, GAIA::REAL g, GAIA::REAL b);

			/*!
				@brief
			*/
			GAIA::GVOID GetStrokeRGB(GAIA::REAL& r, GAIA::REAL& g, GAIA::REAL& b) const;

			/*!
				@brief
			*/
			GAIA::GVOID SetStrokeCMYK(GAIA::REAL c, GAIA::REAL m, GAIA::REAL y, GAIA::REAL k);

			/*!
				@brief
			*/
			GAIA::GVOID GetStrokeCMYK(GAIA::REAL& c, GAIA::REAL& m, GAIA::REAL& y, GAIA::REAL& k) const;

			/*!
				@brief
			*/
			GAIA::GVOID SetFillAlpha(GAIA::REAL a);

			/*!
				@brief
			*/
			GAIA::REAL GetFillAlpha() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetStrokeAlpha(GAIA::REAL a);

			/*!
				@brief
			*/
			GAIA::REAL GetStrokeAlpha() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetBlendMode(GAIA::DOC::PDF_BLEND_MODE m);

			/*!
				@brief
			*/
			GAIA::DOC::PDF_BLEND_MODE GetBlendMode() const;

			// Clip management.

			// State management.
			/*!
				@brief
			*/
			GAIA::GVOID StateSave();

			/*!
				@brief
			*/
			GAIA::GVOID StateRestore();

			// Graphics management.
			/*!
				@brief
			*/
			GAIA::GVOID SetLineWidth(GAIA::REAL rLineWidth);

			/*!
				@brief
			*/
			GAIA::REAL GetLineWidth() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetLineCap(GAIA::DOC::PDF_LINE_CAP m);

			/*!
				@brief
			*/
			GAIA::DOC::PDF_LINE_CAP GetLineCap() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetLineJoin(GAIA::DOC::PDF_LINE_JOIN m);

			/*!
				@brief
			*/
			GAIA::DOC::PDF_LINE_JOIN GetLineJoin() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetDash(const GAIA::U16* pPattern, GAIA::U32 uCount, GAIA::U32 uPhase);

			/*!
				@brief
			*/
			GAIA::GVOID GetDash(GAIA::U16* pPattern, GAIA::U32& uCount, GAIA::U32& uPhase) const;

			/*!
				@brief
			*/
			GAIA::GVOID MoveTo(GAIA::REAL x, GAIA::REAL y);

			/*!
				@brief
			*/
			GAIA::GVOID Pos(GAIA::REAL& x, GAIA::REAL& y) const;

			/*!
				@brief
			*/
			GAIA::GVOID Fill();

			/*!
				@brief
			*/
			GAIA::GVOID Stroke();

			/*!
				@brief
			*/
			GAIA::GVOID FillStroke();

			/*!
				@brief
			*/
			GAIA::GVOID Rectangle(GAIA::REAL x, GAIA::REAL y, GAIA::REAL rWidth, GAIA::REAL rHeight);

			/*!
				@brief
			*/
			GAIA::GVOID Circle(GAIA::REAL x, GAIA::REAL y, GAIA::REAL rRadius);

			/*!
				@brief
			*/
			GAIA::GVOID Ellipse(GAIA::REAL x, GAIA::REAL y, GAIA::REAL xr, GAIA::REAL yr);

			/*!
				@brief
			*/
			GAIA::GVOID Arc(GAIA::REAL x, GAIA::REAL y, GAIA::REAL rRadius, GAIA::REAL rAngle1, GAIA::REAL rAngle2);

			/*!
				@brief
			*/
			GAIA::GVOID LineTo(GAIA::REAL x, GAIA::REAL y);

			/*!
				@brief
			*/
			GAIA::GVOID CurveTo(GAIA::REAL x, GAIA::REAL y, GAIA::REAL xtan, GAIA::REAL ytan);

			/*!
				@brief
			*/
			GAIA::GVOID CurveTo(GAIA::REAL x, GAIA::REAL y, GAIA::REAL xtan1, GAIA::REAL ytan1, GAIA::REAL xtan2, GAIA::REAL ytan2);

			// Font management.
			/*!
				@brief
			*/
			const GAIA::CH* PrepareFont(const GAIA::CH* pszFontFileName, GAIA::BL bEmbedToDoc = GAIA::True);

			// Text management.
			/*!
				@brief
			*/
			GAIA::GVOID BeginText();

			/*!
				@brief
			*/
			GAIA::GVOID EndText();

			/*!
				@brief
			*/
			GAIA::BL IsBeginText() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetFont(const GAIA::CH* pszFont);

			/*!
				@brief
			*/
			const GAIA::CH* GetFont() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetFontSize(GAIA::REAL rFontSize);

			/*!
				@brief
			*/
			GAIA::REAL GetFontSize() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetLineSpace(GAIA::REAL rSpace);

			/*!
				@brief
			*/
			GAIA::REAL GetLineSpace() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetWordSpace(GAIA::REAL rSpace);

			/*!
				@brief
			*/
			GAIA::REAL GetWordSpace() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetCharSpace(GAIA::REAL rSpace);

			/*!
				@brief
			*/
			GAIA::REAL GetCharSpace() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetTextVOffset(GAIA::REAL rOffset);

			/*!
				@brief
			*/
			GAIA::REAL GetTextVOffset() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetTextMatrix(GAIA::REAL x, GAIA::REAL y, GAIA::REAL a, GAIA::REAL b, GAIA::REAL c, GAIA::REAL d);

			/*!
				@brief
			*/
			GAIA::GVOID SetTextRenderMode(GAIA::DOC::PDF_TEXT_RENDER_MODE m);

			/*!
				@brief
			*/
			GAIA::DOC::PDF_TEXT_RENDER_MODE GetTextRenderMode() const;

			/*!
				@brief
			*/
			GAIA::GVOID MoveTextTo(GAIA::REAL x, GAIA::REAL y);

			/*!
				@brief
			*/
			GAIA::GVOID TextPos(GAIA::REAL& x, GAIA::REAL& y) const;

			/*!
				@brief
			*/
			GAIA::GVOID Text(const GAIA::CH* pszText, GAIA::BL bNewLine = GAIA::False);

			/*!
				@brief
			*/
			GAIA::GVOID Text(GAIA::REAL x, GAIA::REAL y, const GAIA::CH* pszText);

			/*!
				@brief
			*/
			GAIA::GVOID Text(GAIA::REAL rLeft, GAIA::REAL rTop, GAIA::REAL rRight, GAIA::REAL rBottom, const GAIA::CH* pszText, GAIA::DOC::PDF_TEXT_ALIGN align = GAIA::DOC::PDF_TEXT_ALIGN_LEFT);

			// Text measure.
			/*!
				@brief
			*/
			GAIA::REAL GetTextWidth(const GAIA::CH* pszText);

			/*!
				@brief
			*/
			GAIA::NUM MeatureText(const GAIA::CH* pszText, GAIA::REAL rWidth, GAIA::BL bWordWrap = GAIA::True, GAIA::REAL* pRealWidth = GNIL);

			// Image management.
			/*!
				@brief
			*/
			GAIA::GVOID DrawImage(GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::CH* pszFileName, const GAIA::CH* pszImageType = GNIL);

			/*!
				@brief
			*/
			GAIA::GVOID DrawImage(GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::GVOID* pBuf, GAIA::NUM sLen, const GAIA::CH* pszImageType);

			/*!
				@brief
			*/
			GAIA::GVOID DrawImage(GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::GVOID* pBuf, GAIA::NUM sBufWidth, GAIA::NUM sBufHeight, GAIA::NUM sBitCountPerPixel);

			// Catalog management.
			/*!
				@brief
			*/
			GAIA::GVOID* MakeCatalog(GAIA::GVOID* pParent, const GAIA::CH* pszName, GAIA::NUM sPageIndex, GAIA::GVOID* pEncoder = GNIL);

			// Link management.
			/*!
				@brief
			*/
			GAIA::GVOID MakeLink(GAIA::NUM sSrcPageIndex, GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, GAIA::NUM sDstPageIndex);

			/*!
				@brief
			*/
			GAIA::GVOID MakeURLLink(GAIA::NUM sPageIndex, GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::CH* pszURL);

			// Comment management.
			/*!
				@brief
			*/
			GAIA::GVOID MakeComment(GAIA::NUM sPageIndex, GAIA::REAL x, GAIA::REAL y, GAIA::REAL w, GAIA::REAL h, const GAIA::CH* pszText);

			// XObject management.

			// Compress management.
			/*!
				@brief
			*/
			GAIA::GVOID SetCompressMode(GAIA::U32 uFlag);

			/*!
				@brief
			*/
			GAIA::U32 GetCompressMode() const;

			// Right management.
			/*!
				@brief
			*/
			GAIA::GVOID SetPermission(GAIA::U32 uFlag);

			/*!
				@brief
			*/
			GAIA::U32 GetPermission() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetCreatorPassword(const GAIA::CH* pszCreatorPassword);

			/*!
				@brief
			*/
			const GAIA::CH* GetCreatorPassword() const;

			/*!
				@brief
			*/
			GAIA::GVOID SetReaderPassword(const GAIA::CH* pszReaderPassword);

			/*!
				@brief
			*/
			const GAIA::CH* GetReaderPassword() const;

			// Save management.
			/*!
				@brief
			*/
			GAIA::BL Save(const GAIA::CH* pszFileName = GNIL);

			// CallBack entries.
			/*!
				@brief
			*/
			GAIA::GVOID OnError(GAIA::U64 uError, GAIA::U64 uDetail);

		private:
			PDFWriterContext* m_ctx;
		};
	}
}

#endif
