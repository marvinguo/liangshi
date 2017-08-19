#pragma once

#include "../common/Qt.h"
#include "../common/qhttp.h"
#include "../common/library.h"
#include "../common/YunHttp.h"

//cookie等东西，应该保存

struct CachedKe
{
	QString _cource;
	QList<KeDTO> _kes;

	void AddKeFile(KeDTO ke);
	void CleanKeCount(int size);

public:
	friend QDataStream& operator << (QDataStream &qos, const CachedKe &ke);
	friend QDataStream& operator >> (QDataStream &qis, CachedKe &ke);
};


struct LocalSave
{
	LocalSave();
	~LocalSave();

	void Save();
	void Load();

	void AddKeFile(QString course, KeDTO ke);
	void CleanKeCount(QString course, int size);

	friend QDataStream& operator << (QDataStream &qos, const LocalSave &save);
	friend QDataStream& operator >> (QDataStream &qis, LocalSave &save);

	QString _username;
	QString _password;
	bool _rememmberPassword;
	QString _cookie;
	QVector<CachedKe> _cachedKes;

private:
	QString _filepath;
};

