#pragma once

#include "../common/library.h"
#include "Element.h"
#include "HeraDocument.h"
#include "BanBaseWidget.h"
#include "PlayerPage.h"
#include "KeTime.h"
#include "Menu.h"
#include "AnimationAction.h"
#include "SwitchPageAction.h"

class AnimationThread;

class LIANGSHIEXPORT PlayerWidget : public QWidget, public HeraDocument
{
	Q_OBJECT
	friend class HeraMainWidget;
	friend class PlayerTitleWidget;
	friend class PlayerActivity;
	friend class PreviewActivity;
	friend class KeFile;
	friend class MainWindow;
	friend class ConvertProgress;
public:
	PlayerWidget(QWidget *parent = 0);
	virtual ~PlayerWidget();

public:
	int GetTotalTime(){return _totalTime;};
	void DrawBackground(QPixmap& pixmap);
	QPixmap GetBackground();
	void GotoTime(int time);

//Widget event, 为了快进的功能
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void keyPressEvent(QKeyEvent * e);

//HeraDocument virtual
public:
	virtual bool CanQuit(){return true;}
	virtual QString GetSaveFileDialongExtension(){return "";}
	virtual void UpdateFilepath(QString filepath);
	virtual bool HasSavedPath(){return false;}
	virtual bool SaveDocument(QString filepath = "");
	virtual bool SaveAsDocument(QString filepath){return "";}
	virtual QWidget* Convert2QWidget(){return static_cast<QWidget*>(this);}

public:
	virtual LokiBuffer GetMsgPackData();
	virtual void SetMsgPackData(LokiBuffer& buffer);
	virtual void UpdateDataFromMsgPack();

Q_SIGNALS:
	void OnSaveFileNameChanged(QString title, bool modifysign);

//control related
public Q_SLOTS:
	void OnPlayTriggered();
	void OnPauseTriggered();
	void OnGotoPositionTriggered(double value);
	void OnResetTriggered();

private Q_SLOTS:
	//更新动画，这个也更新时间
	void AnimatedTriggered();

private:
	void ResetPages();
	void AddPage(BanBaseWidget* page);
	void AddAction(AnimationAction* action);
	void ReleaseMedia();
	void InitMedia(QString filepath);
	vector<AnimationAction*> GetUnAnimatedActions(int time);
	vector<AnimationAction*> GetLatestSwitchBoardActions(int time);
	void PlayActions(vector<AnimationAction*> actions);
	void PlaySwitchPageAction(SwitchPageAction* action);
	void OnGotoPosition(int time);

private:
	QStackedLayout* _mainLayout;
	KeTime* _keTime;

	vector<AnimationAction*> _actions;
	map<string, PlayerPage*> _allPages;
	PlayerPage* _currentPage;
	vector<QString> _attachedTests;
	Hades* _hades;
	QString _hadesFile;

	//不用thread, 用timer试试吧
	//AnimationThread* _thread;
	QTimer* _timer;
	QMutex _animateMutex;

	bool _isMouseDown;
	QPoint _downPoint;
	int _moveDirection;
private:
	int _pageWidth;
	int _pageHeight;
	int _totalTime;

public:
	MSGPACK_DEFINE(_pageWidth, _pageHeight, _totalTime);
};

