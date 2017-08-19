#pragma once

#include "../common/library.h"
#include "../common/Qt.h"
#include "SlidePresenter.h"
#include "SizeAction.h"

class PptSlide
{
	friend class PptAnalyser;

	typedef enum {
		msoAnimTypeMixed = -2,
		msoAnimTypeNone = 0,
		msoAnimTypeMotion = 1,
		msoAnimTypeColor = 2,
		msoAnimTypeScale = 3,
		msoAnimTypeRotation = 4,
		msoAnimTypeProperty = 5,
		msoAnimTypeCommand = 6,
		msoAnimTypeFilter = 7,
		msoAnimTypeSet = 8
	} MsoAnimType;

public:
	PptSlide(int width, int height);
	virtual ~PptSlide();

	void AnalyseShapeAnimation(QAxObject* slide);
	QPoint ParseVMLPath(QString path);
	void DeleteSlideShape(QAxObject* slide, int id);
	QSize GetSize(){ return QSize(_width, _height); }

private:
	//第一个shape的id对应的文件路径
	map<int, QString> _shapes;
	//map，会按int排序
	vector<ShapeAction> _shapeActions;
	vector<int> _shapesWithAnimation;

private:
	QString _backgroundFilename;
	int _width;
	int _height;
};

