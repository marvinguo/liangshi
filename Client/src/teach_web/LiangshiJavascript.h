#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"

class LiangshiJavascript : public QObject
{
	Q_OBJECT
public:
	LiangshiJavascript(QObject *parent = 0);
	void Init(QWebView *webView);

public Q_SLOTS:
	void StartBan();
	void StartLive(QString session, QString role);

private:
	void AddObject();

private:
	QWebFrame* _webFrame;
};

