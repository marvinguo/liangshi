#include "HeraDocManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HeraDocManager::HeraDocManager():
QtSingleton<HeraDocManager>(this)
{
}

HeraDocManager::~HeraDocManager()
{
}

void HeraDocManager::AddHera(HeraMainWidget* hera)
{
	QMutexLocker lock(&_changeMutex);
	_allDocs.push_back(hera);
	Q_EMIT OnHeraCountChanged(_allDocs.size());
}

void HeraDocManager::RemoteHera(HeraMainWidget* hera)
{
	QMutexLocker lock(&_changeMutex);
	vector<HeraMainWidget*>::iterator i = std::find(_allDocs.begin(), _allDocs.end(), hera);
	if(i != _allDocs.end())
	{
		_allDocs.erase(i);
		Q_EMIT OnHeraCountChanged(_allDocs.size());
	}
}

int HeraDocManager::GetHeraCount()
{
	QMutexLocker lock(&_changeMutex);
	return _allDocs.size();
}