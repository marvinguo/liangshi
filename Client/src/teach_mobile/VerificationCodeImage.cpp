#include "VerificationCodeImage.h"
#include "../common/QHttpFileDownloader.h"
#include "../common/Settings.h"
#include "../common/YunHttp.h"
#include "../common/FileUtil.h"
#include "LocalSave.h"
#include "MainWindow.h"
#include "../hera/ChineseMessageBox.h"
#include "BusyDialog.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#define MOBILE_VERSION 34

VerificationCodeImage::VerificationCodeImage(MainWindow* mainWindow, QWidget *parent):
QLabel(parent),
_mainWindow(mainWindow)
{
	setScaledContents(true);
	//我觉得在VerificationCode里检查比较好，这样，先检测到是不是有问题，然后再RefreshImage
	YunHttp* http = new YunHttp();
	connect(http, &YunHttp::CheckMobileVersionFinished, this, &VerificationCodeImage::OnCheckVersionTriggered);
	connect(http, &YunHttp::HasError, _mainWindow, &MainWindow::UseOffliceTriggered);
	http->CheckMobileVersion(MOBILE_VERSION);
}

void VerificationCodeImage::OnCheckVersionTriggered(int version)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;

	if(MOBILE_VERSION < version)
	{
		ChineseMessageBox::Show(tr("点击确定后开始下载升级"), tr("检测到新版本"), ChineseMessageBox::OK, ChineseMessageBox::Information);
		
		//开始update url
		YunHttp* http = new YunHttp();
		connect(http, &YunHttp::GetMobileUpdateUrlFinished, this, &VerificationCodeImage::OnGetUpdateUrlTriggered);
		http->GetMobileUpdateUrl();

		BusyDialog dialog;
		connect(this, &VerificationCodeImage::DoneAPK, &dialog, &BusyDialog::Done);
		dialog.ShowModal();
	}
	else
	{
		RefreshImage();
	}
}

void VerificationCodeImage::OnGetUpdateUrlTriggered(QString url)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;

	url = "liangshi.oss-cn-hangzhou.aliyuncs.com";
	QString remote = "/Download/liangshiyy.apk";

	//2015.0526,开始用新路径下载
	QString local;
		
#ifdef Q_OS_ANDROID
	local = Util::GetAndroidWritablePath() + "liangshiyy.apk";
#else
	local = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/liangshiyy.apk";
#endif

	QHttpFileDownloader* downloader = new QHttpFileDownloader(url, remote, local);
	connect(downloader, &QHttpFileDownloader::HasError, _mainWindow, &MainWindow::NeedNetworkErrorTriggered);
	connect(downloader, &QHttpFileDownloader::DownloadFinished, this, &VerificationCodeImage::DownloadAPKFileTriggered);
	downloader->Download();
}

void VerificationCodeImage::DownloadAPKFileTriggered(QString localFile)
{
	QHttpFileDownloader* downloader = qobject_cast<QHttpFileDownloader *>(sender());
	if(downloader != NULL)
		delete downloader;
	
	//不消失，一直到重装了
#ifdef _WIN32
	Q_EMIT DoneAPK();
#endif

	//call jni to install apk
#ifdef Q_OS_ANDROID
	qDebug() << "VerificationCodeImage::DownloadAPKFileTriggered";
	QAndroidJniObject jtext = QAndroidJniObject::fromString(localFile);
	QAndroidJniObject::callStaticMethod<void>("org/qtproject/qt5/android/bindings/QtActivity",
		"installAPK",
		"(Ljava/lang/String;)V",
		jtext.object<jstring>());
	qDebug() << "end VerificationCodeImage::DownloadAPKFileTriggered";
#endif
}

VerificationCodeImage::~VerificationCodeImage()
{
}

void VerificationCodeImage::mousePressEvent(QMouseEvent *e)
{
	RefreshImage();
}

void VerificationCodeImage::RefreshImage()
{
	qDebug() << "enter VerificationCodeImage::RefreshImage";
	QString input = "liangshiyy_verify.png";
	QString local = FileUtil::GetTempFolder() + input;
	qDebug() << "begin download verify Image:" << local;
	Settings settings;
	QString remote = "/Util/verify";
	QHttpFileDownloader* downloader = new QHttpFileDownloader(settings.GetHomeUrl(), remote, local);
	qDebug() << "downloader:" << downloader;

	qDebug() << "downloader before connect";
	connect(downloader, &QHttpFileDownloader::DownloadFinished, this, &VerificationCodeImage::DownloadImageFileTriggered);
	if(_mainWindow != NULL)
		connect(downloader, &QHttpFileDownloader::HasError, _mainWindow, &MainWindow::NeedNetworkErrorTriggered);
	qDebug() << "downloader after connect";
	downloader->Download();
	qDebug() << "downloader->Download();";
}

void VerificationCodeImage::DownloadImageFileTriggered(QString localFile)
{
	qDebug() << "enter VerificationCodeImage::DownloadImageFileTriggered";

	QPixmap pixmap;
	pixmap.load(localFile);
	setPixmap(pixmap);
	QFile::remove(localFile);

	QHttpFileDownloader* downloader = qobject_cast<QHttpFileDownloader *>(sender());
	if(downloader != NULL)
	{
		disconnect(downloader, &QHttpFileDownloader::DownloadFinished, this, &VerificationCodeImage::DownloadImageFileTriggered);
		LocalSave save;
		save._cookie = downloader->GetCookie();
		qDebug() << "save.SetHeader(header);";
		delete downloader;
		qDebug() << "delete downloader;";
	}
}
