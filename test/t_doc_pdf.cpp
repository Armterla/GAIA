#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_doc_pdf(GAIA::LOG::Log& logobj)
	{
		GTRY
		{
			GAIA::DOC::PDFWriter pdf;
			pdf.SetEncoding("CHS");

			pdf.SetCreator("GAIA");
			if(GAIA::ALGO::gstrcmp(pdf.GetCreator(), "GAIA") != 0)
				TERROR;
			pdf.SetAuthor("Armterla");
			if(GAIA::ALGO::gstrcmp(pdf.GetAuthor(), "Armterla") != 0)
				TERROR;
			pdf.SetTitle("GAIA pdf test");
			if(GAIA::ALGO::gstrcmp(pdf.GetTitle(), "GAIA pdf test") != 0)
				TERROR;
			pdf.SetSubject("PDF test");
			if(GAIA::ALGO::gstrcmp(pdf.GetSubject(), "PDF test") != 0)
				TERROR;
			pdf.SetKeywords("GAIA PDF TEST ARMTERLA");
			if(GAIA::ALGO::gstrcmp(pdf.GetKeywords(), "GAIA PDF TEST ARMTERLA") != 0)
				TERROR;

			pdf.SetCreatorPassword("123");
			if(GAIA::ALGO::gstrcmp(pdf.GetCreatorPassword(), "123") != 0)
				TERROR;
			pdf.SetReaderPassword("321");
			if(GAIA::ALGO::gstrcmp(pdf.GetReaderPassword(), "321") != 0)
				TERROR;
			pdf.SetPermission(GAIA::DOC::PDF_PERMISSION_READ);
			if(pdf.GetPermission() != GAIA::DOC::PDF_PERMISSION_READ)
				TERROR;

			pdf.SetCompressMode(GAIA::DOC::PDF_COMPRESS_TEXT | GAIA::DOC::PDF_COMPRESS_IMAGE | GAIA::DOC::PDF_COMPRESS_META);

			// First page(show catalog)
			GAIA::REAL rFirstPageWidth;
			GAIA::REAL rFirstPageHeight;
			{
				pdf.InsertPage();
				GAIA::REAL rPageWidth = pdf.GetPageWidth();
				GAIA::REAL rPageHeight = pdf.GetPageHeight();
				rFirstPageWidth = rPageWidth;
				rFirstPageHeight = rPageHeight;

				pdf.BeginText();
				{
					pdf.MoveTextTo(24, rPageHeight - 32);
					pdf.Text("Second Page");

					pdf.MoveTextTo(0, -16);
					pdf.Text("Third Page");

					pdf.MoveTextTo(0, -16);
					pdf.Text("Fourth Page");

					pdf.MoveTextTo(0, -16);
					pdf.Text("Access www.baidu.com");
				}
				pdf.EndText();
			}

			// Second page(show text)
			GAIA::REAL rSecondPageWidth;
			GAIA::REAL rSecondPageHeight;
			{
				pdf.InsertPage();
				GAIA::REAL rPageWidth = pdf.GetPageWidth();
				GAIA::REAL rPageHeight = pdf.GetPageHeight();
				rSecondPageWidth = rPageWidth;
				rSecondPageHeight = rPageHeight;
				GAIA::REAL rTextWidth;
				GAIA::REAL rTextPosX;
				GAIA::REAL rTextPosY;
				pdf.BeginText();
				{
					pdf.MoveTextTo(24, rPageHeight - 32);
					pdf.Text("Color with default : ");
					pdf.Text("RGB(0, 0, 0)");
					rTextWidth = pdf.GetTextWidth("Color with default");
					rTextPosX = 24;
					rTextPosY = rPageHeight - 32;

					pdf.SetFillRGB(0.5, 0.0, 0.0);
					pdf.MoveTextTo(0, -16);
					pdf.Text("Color with deep-red : ");
					pdf.Text("RGB(0.5, 0.0, 0.0)");

					pdf.SetFillRGB(0.0, 0.0, 0.0);
					pdf.MoveTextTo(0, -16);
					pdf.SetFont("Helvetica-Bold");
					pdf.Text("Bold text shown with black!");

					pdf.MoveTextTo(0, -16);
					pdf.SetFont("Helvetica-Oblique");
					pdf.Text("Italic text shown with black!");

					GAIA::CTN::WString strW = L"Hello World! I am GAIA!";
					GAIA::CTN::AString strA = strW.toSys();
					pdf.MoveTextTo(0, -16);
					pdf.SetFont("SimSun Encoding:GBK-EUC-H");
					pdf.Text(strA.fptr());
					pdf.MoveTextTo(0, -16);
					pdf.SetFont("SimHei Encoding:GBK-EUC-H");
					pdf.Text(strA.fptr());

					pdf.SetLineWidth(0.5);
					pdf.SetFontSize(24);
					pdf.SetTextRenderMode(GAIA::DOC::PDF_TEXT_RENDER_MODE_STROKE);
					pdf.MoveTextTo(0, -24);
					pdf.SetFont("Helvetica");
					pdf.Text("Text with stroke format!");
					pdf.SetTextRenderMode(GAIA::DOC::PDF_TEXT_RENDER_MODE_FILL);
					pdf.SetFontSize(12);

					GAIA::REAL radian1 = (GAIA::REAL)(48.0 / 180.0 * 3.141592);
					pdf.SetTextMatrix(100, 300, cos(radian1), sin(radian1), -sin(radian1), cos(radian1));
					pdf.SetFont("Helvetica-Oblique");
					pdf.Text("Rotate text with 45 degree!");
					pdf.SetTextMatrix(0, 0, 1, 0, 0, 1);

					pdf.Text(40, rPageHeight - 160, rPageWidth - 80, 320, "This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! This is text by rect!! ");

					pdf.SetFont("Helvetica");
					pdf.Text(24, 32, "This is text by point!");
				}
				pdf.EndText();

				pdf.SetLineCap(GAIA::DOC::PDF_LINE_CAP_ROUND);
				pdf.SetLineJoin(GAIA::DOC::PDF_LINE_JOIN_ROUND);
				pdf.SetStrokeRGB(R(0.6), R(0.2), R(0.0));
				pdf.SetLineWidth(1.0);
				pdf.MoveTo(rTextPosX, rTextPosY - 1.0);
				pdf.LineTo(rTextPosX + rTextWidth, rTextPosY - 1.0);
				pdf.Stroke();
			}

			// Third page(show graphics)
			{
				pdf.InsertPage();
				GAIA::REAL rPageWidth = pdf.GetPageWidth();
				GAIA::REAL rPageHeight = pdf.GetPageHeight();

				pdf.SetLineCap(GAIA::DOC::PDF_LINE_CAP_ROUND);
				pdf.SetLineJoin(GAIA::DOC::PDF_LINE_JOIN_ROUND);

				pdf.SetStrokeRGB(R(0.2), R(0.2), R(0.6));
				pdf.MoveTo(24, rPageHeight - 32);
				pdf.LineTo(rPageWidth - 24, rPageHeight - 32);
				pdf.Stroke();

				pdf.SetLineWidth(4);
				pdf.SetFillRGB(R(0.4), R(0.2), R(0.2));
				pdf.Circle(rPageWidth * 0.5 - 100, rPageHeight * 0.5, 128);
				pdf.Fill();
				pdf.Circle(rPageWidth * 0.5 - 100, rPageHeight * 0.5, 128);
				pdf.Stroke();

				pdf.SetFillRGB(R(0.3), R(0.7), R(0.2));
				pdf.Circle(rPageWidth * 0.5 + 100, rPageHeight * 0.5 - 100, 80);
				pdf.Fill();
				pdf.SetStrokeRGB(R(0.2), R(0.2), R(0.2));
				pdf.Circle(rPageWidth * 0.5 + 100, rPageHeight * 0.5 - 100, 80);
				pdf.Stroke();

				pdf.SetLineWidth(2);
				GAIA::U16 dashp[4] = {9,7,1,7};
				pdf.SetDash(dashp, 4, 0);
				pdf.MoveTo(rPageWidth * 0.5 - 100, rPageHeight * 0.5);
				pdf.CurveTo(rPageWidth * 0.5 + 100, rPageHeight * 0.5 - 100, 300, 100);
				pdf.Stroke();

				pdf.SetDash(GNIL, 0, 0);
				pdf.MoveTo(rPageWidth * 0.5 - 100, rPageHeight * 0.5);
				pdf.CurveTo(rPageWidth * 0.5 + 100, rPageHeight * 0.5 - 100, 200, 500, 300, 500);
				pdf.Stroke();
			}

			// Fourth page(show image)
			{
				pdf.InsertPage();
				GAIA::REAL rPageWidth = pdf.GetPageWidth();
				GAIA::REAL rPageHeight = pdf.GetPageHeight();
				GAIA::REAL rMinSide = GAIA::ALGO::gmin(rPageWidth, rPageHeight);
				
				GAIA::CTN::Buffer buf;
				buf.resize(512 * 512 * 3);
				GAIA::U8* p = buf.fptr();
				for(GAIA::NUM y = 0; y < 512; ++y)
				{
					for(GAIA::NUM x = 0; x < 512; ++x)
					{
						GAIA::NUM sOffset = y * 512 * 3 + x * 3;
						p[sOffset + 0] = 0x12;
						p[sOffset + 1] = 0x34;
						p[sOffset + 2] = 0x56;
					}
				}
				pdf.DrawImage(64, 64, 256, 256, p, rMinSide / 2, rMinSide / 2, 24);
			}

			// Create catalog.
			{
				GAIA::GVOID* pRoot = pdf.MakeCatalog(GNIL, "GAIA PDF test", 0);
				pdf.MakeCatalog(pRoot, "Catagory Page", 0);
				pdf.MakeCatalog(pRoot, "Text Page", 1);
				pdf.MakeCatalog(pRoot, "Graphics Page", 2);
				pdf.MakeCatalog(pRoot, "Image Page", 3);
			}

			// Create link.
			{
				GAIA::REAL x = 24;
				GAIA::REAL y = rFirstPageHeight - 32 + 16;
				GAIA::REAL w = rFirstPageWidth - 48;
				GAIA::REAL h = 16;
				pdf.MakeLink(0, x, y, w, h, 1);

				y -= h;
				pdf.MakeLink(0, x, y, w, h, 2);

				y -= h;
				pdf.MakeLink(0, x, y, w, h, 3);

				y -= h;
				pdf.MakeURLLink(0, x, y, w, h, "www.baidu.com");
			}

			// Create comment.
			{
				GAIA::REAL x = 24;
				GAIA::REAL y = rSecondPageHeight - 32 + 16;
				GAIA::REAL w = rSecondPageWidth - 48;
				GAIA::REAL h = 16;
				pdf.MakeComment(1, x, y, w, h, "Hello World!\n\tHello Kitty!");
			}

			GAIA::TIME::Time tCurrent;
			tCurrent.localtime();
			pdf.SetCreateTime(tCurrent);
			pdf.SetModTime(tCurrent);

			GAIA::CH szFileName[GAIA::MAXPL];
			GAIA::ALGO::gstrcpy(szFileName, g_gaia_appdocdir);
			GAIA::ALGO::gstrcat(szFileName, "test.pdf");
			pdf.Save(szFileName);
		}
		GCATCHALL
		{
			TERROR;
		}
	}
}
