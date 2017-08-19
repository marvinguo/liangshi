#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/stl.h"

class LIANGSHIEXPORT KeTime : public QObject
{
	Q_OBJECT
private:
	enum State
	{
		NotStart,
		Working,
		Paused,
	};

public:
	KeTime(QObject* parent);
	~KeTime();

public:
	void DisableSeltTime(){_selfTimeUpdate = false;}
	void OnHadesTime(int time);

public:
	void Reset();
	void Start();
	void Pause();
	void SetAnimatedTime(int time);
	int GetAnimatedTime();
	bool HadStarted();
	bool IsPausing();

Q_SIGNALS:
	void OnUpdateCurrentTime(int time);
	void OnPause();
	void OnStart();
	void OnReset();

private Q_SLOTS:
	void OnTimerTriggered();

private:
	QDateTime _beginningTime;
	int _recrodTime;
	State _state;
	State _resumeState;

	QTimer* _timer;
	bool _selfTimeUpdate;
	int _hadesTime;
};

