#include "KeTime.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

KeTime::KeTime(QObject* parent):
QObject(parent),
_selfTimeUpdate(true)
{
	_timer = new QTimer(this);
	connect(_timer, &QTimer::timeout, this, &KeTime::OnTimerTriggered);
	_timer->stop();

	Reset();
}

KeTime::~KeTime()
{
}

void KeTime::Reset()
{
	_state = NotStart;
	_resumeState = NotStart;
	_recrodTime = 0;
	_timer->stop();
	Q_EMIT OnReset();
}

void KeTime::Start()
{
	if(_state != Working)
	{
		_beginningTime = QDateTime::currentDateTime();
		_state = Working;
	}
	_timer->start(500);
	Q_EMIT OnStart();
}

void KeTime::Pause()
{
	if(_state != Paused)
	{
		_recrodTime += _beginningTime.msecsTo(QDateTime::currentDateTime());
		_timer->stop();
		_state = Paused;
		Q_EMIT OnPause();
	}
}

void KeTime::SetAnimatedTime(int time)
{
	_recrodTime = time;
	_hadesTime = time;
	_beginningTime = QDateTime::currentDateTime();
	Q_EMIT OnUpdateCurrentTime(GetAnimatedTime());
}

bool KeTime::HadStarted()
{
	return _state != NotStart;
}

bool KeTime::IsPausing()
{
	return _state == Paused || _state == NotStart;
}


void KeTime::OnTimerTriggered()
{
	if(_selfTimeUpdate)
	{
		Q_EMIT OnUpdateCurrentTime(GetAnimatedTime());
	}
}

//新的Hades的time的方式
void KeTime::OnHadesTime(int time)
{
	_hadesTime = time;
	Q_EMIT OnUpdateCurrentTime(time);
}

int KeTime::GetAnimatedTime()
{
	if(_selfTimeUpdate)
	{
		int whole = _recrodTime;
		if(_state == Working)
		{
			whole += _beginningTime.msecsTo(QDateTime::currentDateTime());
		}
		return whole;
	}
	return _hadesTime;
}
