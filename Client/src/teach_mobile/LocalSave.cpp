#include "LocalSave.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

void CachedKe::AddKeFile(KeDTO ke)
{
	bool find = false;

	for(auto i = _kes.begin(); i != _kes.end(); ++i)
	{
		KeDTO aKe = *i;
		if(aKe.index == ke.index)
		{
			_kes.erase(i);
			break;
		}
	}
	QString filepath = FileUtil::GetKeCacheFolder() + ke.url;
	QFileInfo info(filepath);
	if(ke.filesize == info.size())
	{
		_kes.push_back(ke);
	}
	//这里需要排序了
	qSort(_kes.begin(), _kes.end(), [](const KeDTO&a,const KeDTO&b){return a.index < b.index;});
//	qsort(_kes.begin(), _kes.size(), sizeof(KeDTO), [](const KeDTO&a,const KeDTO&b){return a.time > b.time;});
}

void CachedKe::CleanKeCount(int size)
{
	if(_kes.size() > size)
	{
		_kes.erase(_kes.begin() + size, _kes.end());
	}
}

QDataStream& operator << (QDataStream &qos, const CachedKe &ke)
{
	qos << ke._cource;
	qos << ke._kes;
	return qos;
}


QDataStream& operator >> (QDataStream &qis, CachedKe &ke)
{
	qis >> ke._cource;
	qis >> ke._kes;
	return qis;
}

LocalSave::LocalSave()
{
#ifdef _WIN32
	_filepath = QCoreApplication::applicationDirPath() + QObject::tr("/localsave");
#else
	_filepath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QObject::tr("/localsave");
	qDebug() << "local save path:" << _filepath;
#endif
	Load();
}

LocalSave::~LocalSave()
{
	Save();
}

void LocalSave::Save()
{
	QFile file(_filepath);
	if(!file.open(QIODevice::WriteOnly))
		return;
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_4_0);
	out << *this;
	file.close();
}

void LocalSave::Load()
{
	if(QFile::exists(_filepath))
	{
		QFile file(_filepath);
		if(!file.open(QIODevice::ReadOnly))
			return;

		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_0);
		in >> *this;
		file.close();
	}
}

QDataStream& operator << (QDataStream &qos, const LocalSave &save)
{
	int version = 101;
	qos << version;
	qos << save._username;
	qos << save._password;
	qos << save._rememmberPassword;
	qos << save._cookie;
	qos << save._cachedKes;
	return qos;
}
QDataStream& operator >> (QDataStream &qis, LocalSave &save)
{
	int version;
	qis >> version;
	if(version < 101)
		return qis;

	qis >> save._username;
	qis >> save._password;
	qis >> save._rememmberPassword;
	qis >> save._cookie;
	qis >> save._cachedKes;
	return qis;
}

void LocalSave::AddKeFile(QString course, KeDTO ke)
{
	if(course.isEmpty())
		return;
	bool find = false;
	for(QVector<CachedKe>::iterator i = _cachedKes.begin(); i != _cachedKes.end(); ++i)
	{
		CachedKe& cache = *i;
		if(cache._cource == course)
		{
			find = true;
			cache.AddKeFile(ke);
		}
	}

	if(!find)
	{
		CachedKe cache;
		cache._cource = course;
		cache.AddKeFile(ke);
		_cachedKes.push_back(cache);
	}
}

void LocalSave::CleanKeCount(QString course, int size)
{
	for(QVector<CachedKe>::iterator i = _cachedKes.begin(); i != _cachedKes.end(); ++i)
	{
		CachedKe& cache = *i;
		if(cache._cource == course)
		{
			cache.CleanKeCount(size);
		}
	}
}