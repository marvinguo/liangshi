#include "SyncWidget.h"
#include "../common/ThorClient.h"
#include "../common/ThorProtocol.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SyncWidget::SyncWidget(SocketClient*client, QWidget *parent):
QListWidget(parent),
_menu(new SyncMenu(this)),
_currentPath("~"),
_protocol(new TyrProtocol(this))
{
	setAcceptDrops(true);
	//setDragDropMode(QAbstractItemView::DropOnly);
	//setDefaultDropAction(Qt::CopyAction);

	InitMenu();
//	connect(_protocol, SIGNAL(OnCommand(Tyr::TyrCommand*)), this, SLOT(OnCommandTriggered(Tyr::TyrCommand*)));
	connect(_protocol, &TyrProtocol::OnCommand, this, &SyncWidget::OnCommandTriggered);
	connect(this, &QListWidget::itemDoubleClicked, this, &SyncWidget::OnItemDoubleClickTriggered);
	_protocol->SetSocketClient(client);
	_protocol->ls(_currentPath);
}

SyncWidget::~SyncWidget()
{
}

void SyncWidget::InitMenu()
{
	_menu->InitMenu();
	connect(_menu, SIGNAL(SyncDevice()), this, SLOT(SyncDeviceTriggered()));
}

void SyncWidget::OnCommandTriggered(Tyr::TyrCommand* command)
{
	Tyr::LSCommand* ls = dynamic_cast<Tyr::LSCommand*>(command);
	if(ls != NULL)
	{
		clear();
		_currentPath = ls->GetPath();
		vector<Tyr::FileSyncInfo> files = ls->GetFiles();
		for(vector<Tyr::FileSyncInfo>::iterator i = files.begin(); i != files.end(); ++i)
		{
			QString filename((*i)._filename.c_str());
			QListWidgetItem* item = new QListWidgetItem(filename, this);
			item->setData(Qt::UserRole, QVariant(*i));
			addItem(item);
		}
	}
}

void SyncWidget::contextMenuEvent(QContextMenuEvent *e)
{
	_menu->Show(e->globalPos());
}

void SyncWidget::dragEnterEvent(QDragEnterEvent *event)
{
	//表示是否接受这类型文件
	if(event->mimeData()->hasUrls())
	{
		event->acceptProposedAction();
	}
}

void SyncWidget::dragMoveEvent(QDragMoveEvent *event)
{
	event->accept();
}

void SyncWidget::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	for(QList<QUrl>::iterator i = urls.begin(); i != urls.end(); ++i)
	{
		QUrl url = *i;
		QString filepath = url.toLocalFile();
		_protocol->put(_currentPath, filepath);
	}
	event->acceptProposedAction();
}

void SyncWidget::OnItemDoubleClickTriggered(QListWidgetItem *item)
{
	QString filename = item->text();
	QVariant variant = item->data(Qt::UserRole);

	if (variant.canConvert<Tyr::FileSyncInfo>())
	{
		Tyr::FileSyncInfo file = variant.value<Tyr::FileSyncInfo>();
		if(file._isDir)
		{
			_currentPath += "/";
			_currentPath += filename;
			_protocol->ls(_currentPath);
		}
	}
}