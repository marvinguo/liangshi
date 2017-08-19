#pragma once

#include "../common/library.h"
#include "../common/QtSingleton.h"
#include "SlidePresenter.h"

class SlidePresenterManager : public QtSingleton<SlidePresenterManager>
{
public:
	SlidePresenterManager();
	virtual ~SlidePresenterManager();

	void AddPresenter(SlidePresenter* slide);
	void RemotePresenter(SlidePresenter* slide);
	int GetPresenterCount();
	void ChangeParent(QWidget* parent);

private:
	vector<SlidePresenter*> _presenters;
	QMutex _changeMutex;
};

