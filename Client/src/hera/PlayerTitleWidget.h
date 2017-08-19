#pragma once

#include "../common/Qt.h"
#include "TwoStateImageButton.h"
#include "PlayerProgressBar.h"
#include "PlayerWidget.h"
#include "KeTime.h"
#include "HeraMainWidget.h"
#include "TitleButton.h"

class LIANGSHIEXPORT PlayerTitleWidget : public QWidget
{
	Q_OBJECT
public:
	PlayerTitleWidget(NewableDocType type, KeTime* keTime, QWidget *parent = 0);
	~PlayerTitleWidget();

	void Init(QWidget* hera, PlayerWidget* playerWidget);
	void Enable(bool able);
	void SetTotalTime(int time);
	TwoStateImageButton* GetPlayButton(){return _play;}

private:
	virtual void mouseDoubleClickEvent(QMouseEvent *);

Q_SIGNALS:
	void OnGotoPosition(double value);

public Q_SLOTS:
	void OnStartTriggered();
	void OnPauseTriggered();
	void OnResetTriggered();
	void OnTimeUpdateTriggered(int time);

private Q_SLOTS:
	void OnPlayTriggered();
	void OnFullscreenTriggered();
	void OnTestTriggered();
	void OnSliderChangeTriggered(int value);
	void OnQuestionTriggered();	
	void OnReRecordTriggered();

private:
	void UpdateProgressPercent(double value);

private:
	QLabel* _totalTimeLabel;
	QLabel* _currentTimeLabel;
	TwoStateImageButton* _play;
	PlayerProgressBar* _progress;
	TitleButton* _full;
	TitleButton* _test;
	TitleButton* _question;
	TitleButton* _reRecord;

	QWidget* _hera;
	QWidget* _heraParent;
	QSize _oldSize;
	PlayerWidget* _playerWidget;
	int _totalTime;
	bool _paused;
	KeTime* _keTime;
	NewableDocType _type;
	bool _isFullscreen;
};

