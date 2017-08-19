#pragma once

#include "../common/Qt.h"
#include "../common/stl.h"
#include "FourStateImageButton.h"
#include "BanCheckableButton.h"
#include "ColorButton.h"
#include "NumberButton.h"
#include "TitleButton.h"

class LIANGSHIEXPORT BanTitleWidget : public QWidget
{
	Q_OBJECT
public:
	BanTitleWidget(QWidget *parent = 0);
	~BanTitleWidget();

Q_SIGNALS:
	void OnInsertScFile(QString filepath);

private Q_SLOTS:
	void OnInsertTriggered();

private:
	TitleButton* _insert;
};

