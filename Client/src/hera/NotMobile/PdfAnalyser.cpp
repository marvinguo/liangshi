#include "PdfAnalyser.h"
#include "ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PdfAnalyser::PdfAnalyser()
{
}

PdfAnalyser::~PdfAnalyser()
{
}

SlidePresenter* PdfAnalyser::GeneratePresenter(QString filepath)
{
	PoseidonDocument* document = loadPoseidonDocument(filepath);
	if (document == NULL)
	{
		ChineseMessageBox::Show(QObject::tr("不能识别为Pdf文件"), QObject::tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
		return NULL;
	}

	int pages = document->numPages();
	if(pages > 0)
	{
		SlidePresenter* presenter = new SlidePresenter();
		for(int i = 0; i < pages; ++i)
		{
			PoseidonPage* page = document->page(i);//, 1.0f, 1.0f, 90.0f);
			if (page == NULL)
			{
				continue;
			}
			QImage image = page->renderImage();
		
			presenter->AddSlideFromImage(image, image.size());

			delete page;
		}
		delete document;
		return presenter;
	}
	delete document;
	return NULL;
}