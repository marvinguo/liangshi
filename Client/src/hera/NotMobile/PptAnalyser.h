#pragma once

#include "../common/library.h"
#include "../common/Qt.h"
#include "SlidePresenter.h"
#include "SizeAction.h"
#include "PptSlide.h"

class PptAnalyser
{
public:
	PptAnalyser();
	virtual ~PptAnalyser();

	SlidePresenter* GeneratePresenter(QString filepath);

private:
	bool DetectWPS2013(QAxObject* &workWpp);
	bool DetectOffice(QAxObject* &workWpp);

private:
	vector<PptSlide*> _slides;
};

