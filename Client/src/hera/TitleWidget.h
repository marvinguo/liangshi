#pragma once

#include "../common/Qt.h"
#include "FourStateImageButton.h"
#include "FileTitleWidget.h"
#include "DrawingToolsTitleWidget.h"
#include "BanTitleWidget.h"
#ifdef _LiangshiDesktop
	#include "NotMobile/ScriptTitleWidget.h"
#endif
#include "TestTitleWidget.h"
#include "KeTitleWidget.h"
#include "LiveTitleWidget.h"
#include "PlayerTitleWidget.h"
#include "SysTitleWidget.h"
#include "HeraMainWidget.h"
#include "KeTime.h"

class LIANGSHIEXPORT TitleWidget : public QWidget
{
	Q_OBJECT
public:
	TitleWidget(KeTime* keTime, NewableDocType type, QWidget *parent = 0);
	virtual ~TitleWidget();
	void EnableDrag(bool drag){_dragable = drag;}

	FileTitleWidget* GetFileWidget(){return _file;}
	DrawingToolsTitleWidget* GetDrawingToolsWidget(){return _drawing;}
	BanTitleWidget* GetMaterialWidget(){return _material;}
#ifdef _LiangshiDesktop
	ScriptTitleWidget* GetScriptWidget(){return _script;}
#endif
	TestTitleWidget* GetTestWidget(){return _test;}
	KeTitleWidget* GetKeWidget(){return _ke;}
	LiveTitleWidget* GetLiveWidget(){return _live;}
	PlayerTitleWidget* GetPlayerWidget(){return _play;}

Q_SIGNALS:
	void showSkin();
	void showMainMenu();
	void OnMin();
	void OnMax();
	void OnClose();

public Q_SLOTS:
	void OnSetTitleTriggered(QString title, bool modifysign);

protected:
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseDoubleClickEvent(QMouseEvent *);

private:
	void Init(QString icon, bool hasDrawing, bool hasSc, bool hasScript, bool hasTest, bool hasKe, bool hasLive, bool hasPlay, bool hasMax);

private:
	NewableDocType _type;
	QPoint _clickPoint;
	bool _isMoving;
	bool _dragable;

	QLabel* _icon;
	QLabel* _title;

	FileTitleWidget* _file;
	DrawingToolsTitleWidget * _drawing;
	BanTitleWidget* _material;
#ifdef _LiangshiDesktop
	ScriptTitleWidget* _script;
#endif
	TestTitleWidget* _test;
	KeTitleWidget* _ke;
	LiveTitleWidget* _live;
	PlayerTitleWidget* _play;
	SysTitleWidget* _system;
};

