#include "../common/config.h"
#include "../common/Settings.h"
#include "PlayerPluginFactory.h"
#include "MainWindow.h"
#include "UserAgentPage.h"
#include "LiangshiJavascript.h"
#include "UpdateProgress.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#define TEACH_VERSION 32

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent)
{
	setWindowIcon(QIcon(":/App/teach"));
	//在这里检测新版本

	YunHttp* http = new YunHttp();
	connect(http, &YunHttp::CheckClientVersionFinished, this, &MainWindow::OnCheckVersionTriggered);
	http->CheckClientVersion(TEACH_VERSION);
}

void MainWindow::OnCheckVersionTriggered(int version)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;

	if(TEACH_VERSION < version)
	{
		UpdateProgress progress(this);
		progress.ShowModal();
		//如果进程没有被杀死，那么还继续
	}
	InitBrowser();
}

void MainWindow::InitBrowser()
{
	QNetworkProxyFactory::setUseSystemConfiguration (true);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
	//QWebSettings::globalSettings()->setFontFamily(QWebSettings::StandardFont, "Times New Roman");
	QWebSettings::globalSettings()->setFontSize(QWebSettings::DefaultFontSize, 14);
	QWebSettings::globalSettings()->setMaximumPagesInCache(10);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages,true);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled,true);
	//QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	//QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true)
	QWebSettings::globalSettings()->enablePersistentStorage(qApp->applicationDirPath() + tr("/Browser/Cache/"));

	/*
	QNetworkDiskCache* diskCache = new QNetworkDiskCache(this);
	QNetworkAccessManager* manager = new QNetworkAccessManager(this);

	QString location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
	diskCache->setCacheDirectory(location);
	manager->setCache(diskCache);

	QNetworkRequest request = QNetworkRequest();
	request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
	request.setUrl(websiteUrl);
	*/

	QWebSettings::clearMemoryCaches();
	QWebView* webView = new QWebView();
	connect(webView, &QWebView::titleChanged, this, &MainWindow::OnTitleChangedTriggered);

	UserAgentPage* userAgentPage= new UserAgentPage(this);
	webView->setPage(userAgentPage);
//	webView->page()->setNetworkAccessManager(manager);
	PlayerPluginFactory *factory = new PlayerPluginFactory(this);
	webView->page()->setPluginFactory(factory);


	LiangshiJavascript* collect = new LiangshiJavascript(webView);
	collect->Init(webView);

	QWebFrame *frame = webView->page()->mainFrame();
	frame->addToJavaScriptWindowObject("collectLiang", collect);

	Settings settings;
	webView->setUrl(QUrl("http://" + settings.GetHomeUrl()));

	setCentralWidget(webView);
	setWindowTitle(tr("良师益友在线学习"));

	setFocus();
}

void MainWindow::OnTitleChangedTriggered(QString title)
{
	setWindowTitle(title);
}