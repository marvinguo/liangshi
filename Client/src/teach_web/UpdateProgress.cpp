#include "UpdateProgress.h"
#include "../common/YunHttp.h"
#include "../common/HttpFileDownloader.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

UpdateProgress::UpdateProgress(QWidget* parent):
QDialog(parent),
_localUpdateFolder(QCoreApplication::applicationDirPath() + "/Update"),
_currentCount(0),
_totalFileCount(0)
{
	setWindowIcon(QIcon(":/App/teach"));
	setWindowTitle(tr("检测到新版本，正在更新..."));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	flags = flags & (~Qt::WindowCloseButtonHint);
	setWindowFlags(flags);
	setModal(true);

	_progress = new QProgressBar();
	_progress->setTextVisible(false);
	_progress->setFixedHeight(20);
	_progress->setMinimum(0);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(_progress);
	setLayout(mainLayout);

	setFixedHeight(40);
	setFixedWidth(300);

	YunHttp* http = new YunHttp();
	connect(http, &YunHttp::GetClientUpdateUrlFinished, this, &UpdateProgress::OnGetUpdateUrlTriggered);
	http->GetClientUpdateUrl();
}

void UpdateProgress::ShowModal()
{
	show();
	exec();
}

void UpdateProgress::OnGetUpdateUrlTriggered(QString url)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;

	_updateUrl = url;

	QString remoteJson = _updateUrl + "/all.json";
	QDir local;
	local.mkpath(_localUpdateFolder);
	QString localJson = _localUpdateFolder + "/all.json";

	HttpFileDownloader* downloader = new HttpFileDownloader(remoteJson, localJson);
	connect(downloader, &HttpFileDownloader::DownloadFinished, this, &UpdateProgress::OnDownloadJsonTriggered);
	downloader->Download();
}

void UpdateProgress::DownloadUpdateFile()
{

}

QMap<QString, QString> UpdateProgress::ParseJsonFile(QString path)
{
	QMap<QString, QString> results;

	QFile file(path);
	file.open(QIODevice::ReadOnly);

	QJsonParseError error;
	QJsonDocument jdoc= QJsonDocument::fromJson(file.readAll(), &error);
	file.close();
	if(error.error != QJsonParseError::NoError)
	{
		return results;
	}

	QJsonArray array = jdoc.array();
	foreach(const QJsonValue& value, array)
	{
		QJsonObject obj = value.toObject();
		results.insert(obj["name"].toString(), obj["md5"].toString());
	}
	return results;
}

void UpdateProgress::OnDownloadJsonTriggered(QString jsonFile)
{
	HttpFileDownloader* downloader = qobject_cast<HttpFileDownloader *>(sender());
	if(downloader != NULL)
		delete downloader;

	QMap<QString, QString> remoteMd5 = ParseJsonFile(jsonFile);
	QMap<QString, QString> localMd5 = ParseJsonFile(QCoreApplication::applicationDirPath() + "/all.json");
	QVector<QString> downloadUrls;

	for(QMap<QString, QString>::iterator i = remoteMd5.begin(); i != remoteMd5.end(); ++i)
	{
		QString key = i.key();
		QString value = i.value();

		if(!localMd5.contains(key))
		{
			downloadUrls.push_back(key);
		}
		else
		{
			QString md5 = localMd5[key];
			if(md5 != value)
			{
				downloadUrls.push_back(key);
			}
		}
	}
	_totalFileCount = downloadUrls.count();
	_progress->setMaximum(_totalFileCount);

	//这种情况也点处理一下
	if(_totalFileCount == 0)
	{
		close();
		return;
	}

	for(QVector<QString>::iterator i = downloadUrls.begin(); i != downloadUrls.end(); ++i)
	{
		QString subfix = *i;

		QString remoteFile = _updateUrl + subfix;
		QDir local;
		QString localFile = _localUpdateFolder + subfix;
		local.mkpath(localFile);
		local.rmdir(localFile);
		HttpFileDownloader* downloader = new HttpFileDownloader(remoteFile, localFile);
		connect(downloader, &HttpFileDownloader::DownloadFinished, this, &UpdateProgress::OnDownloadFileTriggered);
		downloader->Download();
	}
}

void UpdateProgress::OnDownloadFileTriggered(QString localFile)
{
	HttpFileDownloader* downloader = qobject_cast<HttpFileDownloader *>(sender());
	if(downloader != NULL)
		delete downloader;
	_progress->setValue(++_currentCount);

	//开始启动update
	if(_currentCount >= _totalFileCount)
	{
		QProcess* process = new QProcess();
		process->setWorkingDirectory(QCoreApplication::applicationDirPath());
		process->start("update.exe");
		//qApp->exit();
	}
}