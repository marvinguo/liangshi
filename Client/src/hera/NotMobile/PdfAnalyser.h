#pragma once

#include "../common/library.h"
#include "../common/Qt.h"
#include "SlidePresenter.h"

class PdfAnalyser
{
public:
	PdfAnalyser();
	virtual ~PdfAnalyser();

	SlidePresenter* GeneratePresenter(QString filepath);
};

