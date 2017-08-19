#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT AnimationThread : public QThread
{
	Q_OBJECT
public:
	AnimationThread(QObject *parent = 0);
	~AnimationThread();

public:
	void Stop(bool stop){_stoped = stop;}
	virtual void run();

Q_SIGNALS:
	void OnAnimated();

private:
	bool _stoped;
};