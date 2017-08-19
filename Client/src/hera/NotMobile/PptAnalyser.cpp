#include "PptAnalyser.h"
#include "../common/FileUtil.h"
#include "ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PptAnalyser::PptAnalyser()
{
}

PptAnalyser::~PptAnalyser()
{
	for(auto slide : _slides)
	{
		delete slide;
	}
}

SlidePresenter* PptAnalyser::GeneratePresenter(QString filepath)
{
	::CoInitialize(NULL);
	QAxObject* wpp = NULL;
	bool wps2013 = false;
	bool office = false;
	office = DetectOffice(wpp);
	if(!office)
	{	
		wps2013 = DetectWPS2013(wpp);
		if(!wps2013)
		{
			ChineseMessageBox::Show(QObject::tr("没有检测到WPS或者Office"), QObject::tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
			return NULL;
		}
		wpp->setProperty("Visible", false);
		wpp->setProperty("DisplayAlerts", false);
	}

	QAxObject* presentations = wpp->querySubObject("Presentations");
	QAxObject* presention = presentations->querySubObject("Open(const QString&, QVariant, QVariant, QVariant)", filepath, true, false, false);
	if (presention == NULL)
	{
		if (office)
		{
			ChineseMessageBox::Show(QObject::tr("Powerpoint打开文件错误，如果是WPS编辑过的文档，请在Powerpoint里重新别存为ppt"), QObject::tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
			return NULL;
		}
		else
		{
			ChineseMessageBox::Show(QObject::tr("WPS不能打开文件"), QObject::tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
			return NULL;
		}
	}

	QAxObject* pageSetup = presention->querySubObject("PageSetup");
	int width = pageSetup->property("SlideWidth").toInt();
	int height = pageSetup->property("SlideHeight").toInt();


	QAxObject* slides = presention->querySubObject("Slides");
	int count = slides->property("Count").toInt();

	for(int i = 1; i <= count; i++)
	{
		QAxObject* slideObject = slides->querySubObject("Item(QVariant)", i);
		PptSlide* slide = new PptSlide(width, height);
		slide->AnalyseShapeAnimation(slideObject);

		QString temp = FileUtil::GetTempFolder() + FileUtil::RandomFilename() + ".jpg";
		slideObject->dynamicCall("Export(const QString&, const QString&, int, int)", temp, "jpg", 0, 0);
		slide->_backgroundFilename = temp;
		_slides.push_back(slide);
	}
	wpp->dynamicCall("Quit()");
	delete wpp;
	::CoUninitialize();

	SlidePresenter* presenter = new SlidePresenter();
	for(auto slide : _slides)
	{
		QImage image;
		image.load(slide->_backgroundFilename);
		Slide* presentSlide = presenter->AddSlideFromImage(image, slide->GetSize());

		for(auto shape : slide->_shapes)
		{
			presentSlide->AddShape(shape.first, shape.second);
		}
		for(auto action : slide->_shapeActions)
		{
			presentSlide->AddAction(action.id, action.action);
		}
		QFile::remove(slide->_backgroundFilename);
	}
	return presenter;
}

bool PptAnalyser::DetectWPS2013(QAxObject* &workWpp)
{
	QAxObject* wpp = new QAxObject("KWPP.Application");
	if (wpp != NULL)
	{
		if (wpp->querySubObject("Presentations") != NULL)
		{
			workWpp = wpp;
			return true;
		}
	}
	return false;
}

bool PptAnalyser::DetectOffice(QAxObject* &workWpp)
{
	QAxObject* wpp = new QAxObject("PowerPoint.Application");
	if(wpp != NULL)
	{
		if(wpp->querySubObject("Presentations") != NULL)
		{
			workWpp = wpp;
			return true;
		}
	}
	return false;
}
