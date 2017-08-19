#include "SlidePresenterManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SlidePresenterManager::SlidePresenterManager():
QtSingleton<SlidePresenterManager>(this)
{
}

SlidePresenterManager::~SlidePresenterManager()
{
}

void SlidePresenterManager::AddPresenter(SlidePresenter* slide)
{
	QMutexLocker lock(&_changeMutex);
	_presenters.push_back(slide);
}

void SlidePresenterManager::RemotePresenter(SlidePresenter* slide)
{
	QMutexLocker lock(&_changeMutex);
	vector<SlidePresenter*>::iterator i = std::find(_presenters.begin(), _presenters.end(), slide);
	if (i != _presenters.end())
	{
		_presenters.erase(i);
	}
}

int SlidePresenterManager::GetPresenterCount()
{
	QMutexLocker lock(&_changeMutex);
	return _presenters.size();
}

void SlidePresenterManager::ChangeParent(QWidget* parent)
{
	for(auto presenter : _presenters)
	{
		presenter->ChangeParentWidget(parent);
		presenter->ShowCurrentSlide();
	}
}
