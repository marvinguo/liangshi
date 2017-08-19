#include "LiangshiJavascript.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LiangshiJavascript::LiangshiJavascript(QObject *parent):
QObject(parent)
{
}

void LiangshiJavascript::Init(QWebView *webView)
{
	_webFrame = webView->page()->mainFrame();
	AddObject();
	connect(_webFrame, &QWebFrame::javaScriptWindowObjectCleared, this, &LiangshiJavascript::AddObject);
}

void LiangshiJavascript::AddObject()
{
	QWebSettings::clearMemoryCaches();
	_webFrame->addToJavaScriptWindowObject("collectLiang", this);
}

void LiangshiJavascript::StartBan()
{
	QProcess* process = new QProcess(this);
	process->setWorkingDirectory(QCoreApplication::applicationDirPath());
	process->start("collect.exe");
}

void LiangshiJavascript::StartLive(QString session, QString role)
{
	QProcess* process = new QProcess(this);
	process->setWorkingDirectory(QCoreApplication::applicationDirPath());
	QStringList arguments;
	arguments << session;
	arguments << role;
	process->start("live.exe", arguments);
}
