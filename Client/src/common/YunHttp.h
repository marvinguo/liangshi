#pragma once

#include "Qt.h"
#include "stl.h"
#include "qhttp.h"

struct ClassDTO
{
	QString id;
	int type;
	QString title;
	QString session;
	QString all_teachers;
};
Q_DECLARE_METATYPE(ClassDTO)

struct KeDTO
{
	QString id;
	QString title;
	QString url;
	int filesize;
	int time;
	int index;
public:
	friend QDataStream& operator << (QDataStream &qos, const KeDTO &ke);
	friend QDataStream& operator >> (QDataStream &qis, KeDTO &ke);
};
Q_DECLARE_METATYPE(KeDTO)

struct TeachingDTO
{
	QString id;
	int type;
	QString title;
	QString session;
	QString all_teachers;
};
Q_DECLARE_METATYPE(TeachingDTO)

class YunHttp : public QObject
{
	Q_OBJECT
private:
	enum APIMode
	{
		CheckClientVersionMode,
		UpdateClientUrlMode,
		LoginMode,
		GetTypeMode,
		GetClassesMode,
		GetKesMode,
		GetTeachingMode,
		CheckMobileVersionMode,
		UpdateMobileUrlMode,
	};

public:
	YunHttp(QString cookie = "");
	virtual ~YunHttp();

	//QString GetCookie(){return _cookie;}

//API
	void CheckClientVersion(int current);
	void GetClientUpdateUrl();
	void CheckMobileVersion(int current);
	void GetMobileUpdateUrl();
	void MobileLogin(QString &user, QString &password, QString &deviceID, QString& verify);
	void GetType();
	void GetClasses();
	void GetKes(QString id);
	void GetTeachings();

Q_SIGNALS:
	void MobileLoginFinished(bool, QString reason);
	void CheckClientVersionFinished(int);
	void GetClientUpdateUrlFinished(QString);
	void CheckMobileVersionFinished(int);
	void GetMobileUpdateUrlFinished(QString);
	void GetTypeFinished(int);
	void GetClassesFinished(vector<ClassDTO> classes);
	void GetKesFinished(vector<KeDTO> kes);
	void GetTeachingsFinished(vector<TeachingDTO> teachings);
	void HasError();

private Q_SLOTS:
	void DoneTriggered(bool hasError);

private:
	void Get(QString action);
	void Post(QString action, QMap<QString, QString> data);
	vector<ClassDTO> ParseClassesJson(QByteArray& json);
	vector<KeDTO> ParseKesJson(QByteArray& json);
	vector<TeachingDTO> ParseTeachingsJson(QByteArray& json);

private:
	APIMode _mode;
	
	QHttp _http;
	
	QString _cookie;
	QString _host;
	QString _clientBase;
	QString _mobileBase;
	QString _localFile;
};

