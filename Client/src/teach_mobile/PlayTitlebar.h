#pragma once

#include "../common/Qt.h"

#include "ActivityTitleButton.h"
#include "ActivityTitleLabel.h"
#include "../hera/PlayerProgressBar.h"
#include "../hera/KeTime.h"

class PlayTitlebar : public QWidget
{
	Q_OBJECT
public:
	PlayTitlebar(QWidget* parent = NULL);

	void SetKeTime(KeTime* keTime);
	void SetTotalTime(int time);

Q_SIGNALS:
	void OnGotoPosition(double value);

public Q_SLOTS:
	void OnPlayButtonTriggered();
	void OnSliderChangeTriggered(int value);
	void OnStartTriggered();
	void OnPauseTriggered();
	void OnTimeUpdateTriggered(int time);

private:
	void UpdateProgressPercent(double value);

private:
	ActivityTitleButton* _play;
	ActivityTitleLabel* _totalTimeLabel;
	ActivityTitleLabel* _currentTimeLabel;
	PlayerProgressBar* _progress;
	KeTime* _keTime;
	bool _paused;
	int _totalTime;
};

