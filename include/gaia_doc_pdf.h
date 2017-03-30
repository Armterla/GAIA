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
			@brief PDF text align type.
		*/
		GAIA_ENUM_BEGIN(PDF_TEXT_ALIGN)
			PDF_TEXT_ALIGN_LEFT,
			PDF_TEXT_ALIGN_RIGHT,
			PDF_TEXT_ALIGN_CENTER,
			PDF_TEXT_ALIGN_JUSTIFY,
		GAIA_ENUM_END(PDF_TEXT_ALIGN)

		/*!
			@brief PDF line cap type.
		*/
		GAIA_ENUM_BEGIN(PDF_LINE_CAP)
			PDF_LINE_CAP_HARD,
			PDF_LINE_CAP_ROUND,
			PDF_LINE_CAP_SCUARE,
		GAIA_ENUM_END(PDF_LINE_CAP)

		/*!
			@brief PDF link join type.
		*/
		GAIA_ENUM_BEGIN(PDF_LINE_JOIN)
			PDF_LINE_JOIN_MITER,
			PDF_LINE_JOIN_ROUND,
			PDF_LINE_JOIN_BEVEL,
		GAIA_ENUM_END(PDF_LINE_JOIN)

		/*!
			@brief PDF text render mode.
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
			@brief PDF primitive blend mode.
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
				@brief PDF writer's callback.
			*/
			class CallBack : public GAIA::Base
			{
			public:
				/*!
					@brief If error occurred when write a PDF, this method will be callbacked.
				*/
				virtual GAIA::GVOID OnError(GAIA::U64 uError, GAIA::U64 uDetail, const GAIA::CH* pszError){}

			public:
				
				/*!
					@brief Constructor.
				*/
				CallBack(){this->init();}
				
				/*!
					@brief Get current callback's owner PDFWriter.
				 
					@return Return the owner PDFWriter.
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
				@brief Constructor.
			*/
			PDFWriter();

			/*!
				@brief Destructor.
			*/
			~PDFWriter();

			// CallBack management.
			/*!
				@brief Regist a callback.
			*/
			GAIA::GVOID RegistCallBack(GAIA::DOC::PDFWriter::CallBack& cb);

			/*!
				@brief Unregist a callback.
			*/
			GAIA::GVOID UnregistCallBack(GAIA::DOC::PDFWriter::CallBack& cb);

			/*!
				@brief Check the callback is registed or not.
			*/
			GAIA::BL IsRegistedCallBack(GAIA::DOC::PDFWriter::CallBack& cb) const;

			/*!
				@brief Unregist all callback.
			*/
			GAIA::GVOID UnregistCallBackAll();

			// Document info management.
			/*!
				@brief Set PDF's creator information.
			 
				@param pszCreator [in] Specify the creator's name.
			*/
			GAIA::GVOID SetCreator(const GAIA::CH* pszCreator);

			/*!
				@brief Get PDF's creator information.
			 
				@return Return the creator's name.
			*/
			const GAIA::CH* GetCreator() const;

			/*!
				@brief Set PDF's author information.
			 
				@param pszAuthor [in] Specify the author's name.
			*/
			GAIA::GVOID SetAuthor(const GAIA::CH* pszAuthor);

			/*!
				@brief Get PDF's author information.
			 
				@return Return the author's name.
			*/
			const GAIA::CH* GetAuthor() const;

			/*!
				@brief Set PDF's title.
			 
				@param pszTitle [in] Specify the title name.
			*/
			GAIA::GVOID SetTitle(const GAIA::CH* pszTitle);

			/*!
				@brief Get PDF's title.
			 
				@return Return the PDF's title name.
			*/
			const GAIA::CH* GetTitle() const;

			/*!
				@brief Set PDF's subject information.
			 
				@param pszSubject [in] Specify the subject name.
			*/
			GAIA::GVOID SetSubject(const GAIA::CH* pszSubject);

			/*!
				@brief Get PDF's subject information.
			 
				@return Return the PDF's subject name.
			*/
			const GAIA::CH* GetSubject() const;

			/*!
				@brief Set PDF's keywords information.
			 
				@param pszKeywords [in] Specify the keywords.
			*/
			GAIA::GVOID SetKeywords(const GAIA::CH* pszKeywords);

			/*!
				@brief Get PDF's keywords information.
			 
				@return Return PDF's keywords.
			*/
			const GAIA::CH* GetKeywords() const;

			/*!
				@brief Set PDF's create time.
			*/
			GAIA::GVOID SetCreateTime(const GAIA::TIME::Time& t);

			/*!
				@brief Get PDF's create time.
			*/
			const GAIA::TIME::Time& GetCreateTime() const;

			/*!
				@brief Set PDF's mod time.
			*/
			GAIA::GVOID SetModTime(const GAIA::TIME::Time& t);

			/*!
				@brief Get PDF's mod time.
			*/
			const GAIA::TIME::Time& GetModTime() const;

			// Encoding management.
			/*!
				@brief Set PDF's encoding.
			*/
			GAIA::GVOID SetEncoding(const GAIA::CH* pszEncodingName);

			/*!
				@brief Get PDF's encoding.
			*/
			const GAIA::CH* GetEncoding() const;

			// Page management.
			/*!
				@brief Insert a page at a specified page index.
			*/
			GAIA::GVOID InsertPage(GAIA::NUM sPageIndex = GINVALID);

			/*!
				@brief Get page count.
			*/
			GAIA::NUM GetPageCount() const;

			/*!
				@brief Select a page for current operation page.
			*/
			GAIA::GVOID SelectPage(GAIA::NUM sPageIndex = GINVALID);

			/*!
				@brief Set page's width.
			*/
			GAIA::GVOID SetPageWidth(GAIA::REAL rWidth);

			/*!
				@brief Get page's width.
			*/
			GAIA::REAL GetPageWidth() const;

			/*!
				@brief Set page's height.
			*/
			GAIA::GVOID SetPageHeight(GAIA::REAL rHeight);

			/*!
				@brief Get page's height.
			*/
			GAIA::REAL GetPageHeight() const;

			// Color management.
			/*!
				@brief Set current pipeline's Fill-RGB color.
			*/
			GAIA::GVOID SetFillRGB(GAIA::REAL r, GAIA::REAL g, GAIA::REAL b);

			/*!
				@brief Get current pipeline's Fill-RGB color.
			*/
			GAIA::GVOID GetFillRGB(GAIA::REAL& r, GAIA::REAL& g, GAIA::REAL& b) const;

			/*!
				@brief Set current pipeline's Fill-CMYK color.
			*/
			GAIA::GVOID SetFillCMYK(GAIA::REAL c, GAIA::REAL m, GAIA::REAL y, GAIA::REAL k);

			/*!
				@brief Get current pipeline's Fill-CMYK color.
			*/
			GAIA::GVOID GetFillCMYK(GAIA::REAL& c, GAIA::REAL& m, GAIA::REAL& y, GAIA::REAL& k) const;

			/*!
				@brief Set current pipeline's Stroke-RGB color.
			*/
			GAIA::GVOID SetStrokeRGB(GAIA::REAL r, GAIA::REAL g, GAIA::REAL b);

			/*!
				@brief Get current pipeline's Stroke-RGB color.
			*/
			GAIA::GVOID GetStrokeRGB(GAIA::REAL& r, GAIA::REAL& g, GAIA::REAL& b) const;

			/*!
				@brief Set current pipeline's Stroke-CMYK color.
			*/
			GAIA::GVOID SetStrokeCMYK(GAIA::REAL c, GAIA::REAL m, GAIA::REAL y, GAIA::REAL k);

			/*!
				@brief Get current pipeline's Stroke-CMYK color.
			*/
			GAIA::GVOID GetStrokeCMYK(GAIA::REAL& c, GAIA::REAL& m, GAIA::REAL& y, GAIA::REAL& k) const;

			/*!
				@brief Set current pipeline's Fill-Alpha value.
			*/
			GAIA::GVOID SetFillAlpha(GAIA::REAL a);

			/*!
				@brief Get current pipeline's Fill-Alpha value.
			*/
			GAIA::REAL GetFillAlpha() const;

			/*!
				@brief Set current pipeline's Stroke-Alpha value.
			*/
			GAIA::GVOID SetStrokeAlpha(GAIA::REAL a);

			/*!
				@brief Get current pipeline's Stroke-Alpha value.
			*/
			GAIA::REAL GetStrokeAlpha() const;

			/*!
				@brief Set current pipeline's blend mode.
			*/
			GAIA::GVOID SetBlendMode(GAIA::DOC::PDF_BLEND_MODE m);

			/*!
				@brief Get current pipeline's blend mode.
			*/
			GAIA::DOC::PDF_BLEND_MODE GetBlendMode() const;

			// Clip management.

			// State management.
			/*!
				@brief Save current pipeline's states.
			*/
			GAIA::GVOID StateSave();

			/*!
				@brief Restore current pipeline's states.
			*/
			GAIA::GVOID StateRestore();

			// Graphics management.
			/*!
				@brief Set current pipeline's Line-Width.
			*/
			GAIA::GVOID SetLineWidth(GAIA::REAL rLineWidth);

			/*!
				@brief Get current pipeline's Line-Width.
			*/
			GAIA::REAL GetLineWidth() const;

			/*!
				@brief Set current pipeline's Line-Cap type.
			*/
			GAIA::GVOID SetLineCap(GAIA::DOC::PDF_LINE_CAP m);

			/*!
				@brief Get current pipeline's Line-Cap type.
			*/
			GAIA::DOC::PDF_LINE_CAP GetLineCap() const;

			/*!
				@brief Set current pipeline's Line-Join type.
			*/
			GAIA::GVOID SetLineJoin(GAIA::DOC::PDF_LINE_JOIN m);

			/*!
				@brief Get current pipeline's Line-Join type.
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
				@brief Set PDF's creator's password.
			*/
			GAIA::GVOID SetCreatorPassword(const GAIA::CH* pszCreatorPassword);

			/*!
				@brief Get PDF's creator's password.
			*/
			const GAIA::CH* GetCreatorPassword() const;

			/*!
				@brief Set PDF's reader password.
			*/
			GAIA::GVOID SetReaderPassword(const GAIA::CH* pszReaderPassword);

			/*!
				@brief Get PDF's reader password.
			*/
			const GAIA::CH* GetReaderPassword() const;

			// Save management.
			/*!
				@brief Save PDF to file.
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
