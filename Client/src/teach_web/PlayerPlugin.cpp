#include "PlayerPlugin.h"
#include "../hera/ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PlayerPlugin::PlayerPlugin(QString cachefile, QWidget *parent):
QWidget(parent),
_cachefile(cachefile)
{
	_mainLayout = new QStackedLayout();
	_mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(_mainLayout);

	QLabel* label = new QLabel(tr("正在载入中, 请稍后..."));
	QFont f("Arial", 30, QFont::Bold);
	label->setFont(f);
	label->setStyleSheet("qproperty-alignment: AlignCenter;color:blue");
	_mainLayout->addWidget(label);
	_mainLayout->setCurrentWidget(label);
}

PlayerPlugin::~PlayerPlugin()
{
}

void PlayerPlugin::OnFinishDownload()
{
	QNetworkReply *reply = static_cast<QNetworkReply *>(sender());

	QNetworkReply::NetworkError error = reply->error();

	if (error != QNetworkReply::NoError)
		return;

	QFile file(_cachefile);
	file.open(QIODevice::WriteOnly);
	file.write(reply->readAll());
	file.flush();
	file.close();

	//就开始做hera吧
	LoadCacheKe();
}

void PlayerPlugin::LoadCacheKe()
{
	HeraMainWidget* hera = HeraMainWidget::LoadKeFile(_cachefile, true);
	if(hera == NULL)
	{
		ChineseMessageBox::Show(tr("错误"), tr("文件不能识别为课件文件"), ChineseMessageBox::OK, ChineseMessageBox::Error);
		return;
	}
	_hera = hera;
	_mainLayout->addWidget(_hera);
	_mainLayout->setCurrentWidget(_hera);
}

