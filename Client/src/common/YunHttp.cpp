#include "YunHttp.h"
#include "Settings.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

QDataStream& operator << (QDataStream &qos, const KeDTO &ke)
{
	qos << ke.id;
	qos << ke.title;
	qos << ke.url;
	qos << ke.filesize;
	qos << ke.time;
	qos << ke.index;
	return qos;
}


QDataStream& operator >> (QDataStream &qis, KeDTO &ke)
{
	qis >> ke.id;
	qis >> ke.title;
	qis >> ke.url;
	qis >> ke.filesize;
	qis >> ke.time;
	qis >> ke.index;
	return qis;
}

YunHttp::YunHttp(QString cookie):
QObject(NULL),
_cookie(cookie)
{
	Settings settings;
	_host = settings.GetHomeUrl();
	_http.setHost(_host);

	_clientBase = "/Client/";
	_mobileBase = "/Mobile/";
	connect(&_http, &QHttp::done, this, &YunHttp::DoneTriggered);
}

YunHttp::~YunHttp()
{
}

void YunHttp::CheckClientVersion(int current)
{
	QString remote(_clientBase + "version");
	_mode = CheckClientVersionMode;
	_http.get(remote);
}

void YunHttp::GetClientUpdateUrl()
{
	QString remote(_clientBase + "updateUrl");
	_mode = UpdateClientUrlMode;
	_http.get(remote);
}

void YunHttp::CheckMobileVersion(int current)
{
	QString remote(_mobileBase + "version");
	_mode = CheckMobileVersionMode;
	_http.get(remote);
}

void YunHttp::GetMobileUpdateUrl()
{
	QString remote(_mobileBase + "updateUrl");
	_mode = UpdateMobileUrlMode;
	_http.get(remote);
}

void YunHttp::Post(QString action, QMap<QString, QString> data)
{
	QByteArray postData;
	for(QMap<QString, QString>::iterator i = data.begin(); i != data.end(); ++i)
	{
		QString key = i.key();
		QString value = i.value();
		postData.append(key).append("=").append(value).append("&");
	}

	//这个不工作
	bool enableXdebug = false;
	if(enableXdebug)
	{
		postData.append("XDEBUG_SESSION").append("=").append("android").append("&");
	}

	QString remote(_mobileBase + action);
	QHttpRequestHeader header(QLatin1String("POST"), remote);
	header.setValue("Host", _host);
	header.setValue("Cookie", _cookie);
	header.setContentType("application/x-www-form-urlencoded");
	header.setContentLength(postData.length());
	_http.request(header, postData);
}

void YunHttp::Get(QString action)
{
	QString remote(_mobileBase + action);
	QHttpRequestHeader header(QLatin1String("GET"), remote);
	header.setValue("Host", _host);
	header.setValue("Cookie", _cookie);
	_http.request(header);
}

void YunHttp::MobileLogin(QString &user, QString &password, QString &deviceID, QString& verify)
{
	QMap<QString, QString> data;
	data.insert("user", user);

	QByteArray hashData = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
	QString md5(hashData.toHex());
	data.insert("password", md5);

	data.insert("deviceID", deviceID);
	data.insert("verify", verify);
	_mode = LoginMode;
	Post("login", data);
}

void YunHttp::GetType()
{
	_mode = GetTypeMode;
	Get("type");
}

void YunHttp::GetClasses()
{
	_mode = GetClassesMode;
	Get("classes");
}

void YunHttp::GetKes(QString id)
{
	QMap<QString, QString> data;
	data.insert("id", id);
	_mode = GetKesMode;
	Post("kes", data);
}

void YunHttp::GetTeachings()
{
	_mode = GetTeachingMode;
	Get("teachings");
}

void YunHttp::DoneTriggered(bool hasError)
{
	if(hasError)
	{
		Q_EMIT HasError();
		return;
	}
	QByteArray value = _http.readAll();

	if(_mode == CheckClientVersionMode)
	{
		Q_EMIT CheckClientVersionFinished(value.toInt());
	}
	else if(_mode == UpdateClientUrlMode)
	{
		Q_EMIT GetClientUpdateUrlFinished(value);
	}
	else if(_mode == CheckMobileVersionMode)
	{
		Q_EMIT CheckMobileVersionFinished(value.toInt());
	}
	else if(_mode == UpdateMobileUrlMode)
	{
		Q_EMIT GetMobileUpdateUrlFinished(value);
	}
	else if(_mode == LoginMode)
	{

		if(value == tr("OK"))
		{
			Q_EMIT MobileLoginFinished(true, "");
		}
		else
		{
			Q_EMIT MobileLoginFinished(false, value);
		}
	}
	else if(_mode == GetTypeMode)
	{
		Q_EMIT GetTypeFinished(value.toInt());
	}
	else if(_mode == GetClassesMode)
	{
		vector<ClassDTO> classes = ParseClassesJson(value);
		Q_EMIT GetClassesFinished(classes);
	}
	else if(_mode == GetKesMode)
	{
		vector<KeDTO> kes = ParseKesJson(value);
		Q_EMIT GetKesFinished(kes);
	}
	else if(_mode == GetTeachingMode)
	{
		vector<TeachingDTO> teachings = ParseTeachingsJson(value);
		Q_EMIT GetTeachingsFinished(teachings);
	}
}

vector<ClassDTO> YunHttp::ParseClassesJson(QByteArray& json)
{
	vector<ClassDTO> results;
	QJsonParseError error;
	QJsonDocument jdoc= QJsonDocument::fromJson(json, &error);

	if(error.error != QJsonParseError::NoError)
	{
		return results;
	}

	QJsonArray array = jdoc.array();
	foreach(const QJsonValue& value, array)
	{
		QJsonObject obj = value.toObject();
		ClassDTO dto;
		dto.id = obj["id"].toString();
		dto.type = obj["type"].toString().toInt();
		dto.title = obj["title"].toString();
		dto.session = obj["session"].toString();

		QJsonValue all_teachers = obj["all_teachers"];
		QJsonValue::Type type = all_teachers.type();
		dto.all_teachers = all_teachers.toString();
		results.push_back(dto);
	}
	return results;
}

vector<KeDTO> YunHttp::ParseKesJson(QByteArray& json)
{
	vector<KeDTO> results;
	QJsonParseError error;
	QJsonDocument jdoc= QJsonDocument::fromJson(json, &error);

	if(error.error != QJsonParseError::NoError)
	{
		return results;
	}

	QJsonArray array = jdoc.array();
	foreach(const QJsonValue& value, array)
	{
		QJsonObject obj = value.toObject();
		KeDTO dto;
		dto.id = obj["id"].toString();
		dto.title = obj["title"].toString();
		dto.url = obj["url"].toString();
		QJsonValue filesize = obj["filesize"];
		dto.filesize = filesize.toString().toInt();
		QJsonValue time = obj["time"];
		QJsonValue::Type type = time.type();
		dto.time = time.toString().toInt();
		dto.index = obj["index"].toInt();
		results.push_back(dto);
	}
	return results;
}

vector<TeachingDTO> YunHttp::ParseTeachingsJson(QByteArray& json)
{
	vector<TeachingDTO> results;
	QJsonParseError error;
	QJsonDocument jdoc= QJsonDocument::fromJson(json, &error);

	if(error.error != QJsonParseError::NoError)
	{
		return results;
	}

	QJsonArray array = jdoc.array();
	foreach(const QJsonValue& value, array)
	{
		QJsonObject obj = value.toObject();
		TeachingDTO dto;
		dto.id = obj["id"].toString();
		dto.type = obj["type"].toString().toInt();
		dto.title = obj["title"].toString();
		dto.session = obj["session"].toString();
		dto.all_teachers = obj["all_teachers"].toString();
		results.push_back(dto);
	}
	return results;
}
