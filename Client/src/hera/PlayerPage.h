#pragma once

#include "../common/Qt.h"
#include "BanBaseWidget.h"

#include "SizeAction.h"
#include "ShowAction.h"
#include "HideAction.h"
#include "LineStyleAction.h"
#include "LineWidthAction.h"
#include "LineColorAction.h"
#include "TopMostAction.h"
#include "BottomMostAction.h"
#include "TextChangedAction.h"

//需要模仿一下ScElement
class LIANGSHIEXPORT PlayerPage : public QWidget
{
	Q_OBJECT
	friend class PlayerWidget;
	friend class PlayerTitleWidget;
	friend class KeFile;
public:
	PlayerPage(int pageWidth, int pageHeight, BanBaseWidget* viewer, QWidget *parent = 0);
	virtual ~PlayerPage();

public:
	void ResetElement();
	void PlayAction(AnimationAction* action);
	void PlayShowAction(ShowAction* action);
	void PlaySizeAction(SizeAction* action);
	void PlayHideAction(HideAction* action);
	void PlayLineWidthAction(LineWidthAction* action);
	void PlayLineStyleAction(LineStyleAction* action);
	void PlayLineColorAction(LineColorAction* action);
	void PlayTopMostAction(TopMostAction* action);
	void PlayBottomMostAction(BottomMostAction* action);
	void PlayTextChangedAction(TextChangedAction* action);
	void DrawBackground(QPixmap& pixmap);

//QWidget virtual 
protected:
	virtual void resizeEvent(QResizeEvent *e);
	virtual void paintEvent(QPaintEvent *e);

private:
	QPixmap _backgroudPixmap;
	BanBaseWidget* _viewer;
	int _pageWidth;
	int _pageHeight;
};

