#include "AnimationThread.h"

AnimationThread::AnimationThread(QObject* parent):
QThread(parent),
_stoped(false)
{

}

AnimationThread::~AnimationThread()
{

}


void AnimationThread::run()
{
	while(!_stoped)
	{
		Q_EMIT OnAnimated();
		msleep(100);
	}
}
