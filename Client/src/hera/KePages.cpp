#include "KePages.h"
#include "../common/library.h"
#include "../common/FileUtil.h"
#include "../common/ImageUtil.h"
#include "DrawElement.h"
#include "LineElement.h"
#include "RectElement.h"
#include "EllipseElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

KePages::KePages(QWidget *parent):
QDialog(parent),
QtSingleton<KePages>(this)
{
	setWindowIcon(QIcon(":/App/collect"));
	setWindowTitle(QObject::tr("选择黑板"));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	setWindowFlags(flags);
	setModal(true);

	_list = new QListWidget();
	_list->setViewMode(QListView::IconMode);
	_list->setResizeMode(QListView::Fixed);
	_list->setIconSize(QSize(200, 200));
	_list->setTextElideMode(Qt::ElideMiddle);
	connect(_list, &QListWidget::itemDoubleClicked, this, &KePages::OnItemDoubleClickTriggered);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(_list);
	setLayout(mainLayout);

	setMinimumWidth(600);
	setMinimumHeight(400);
}

KePages::~KePages()
{
}

//对于网络，多一块网络黑板，然后写本地黑板
void KePages::ShowModal(vector<BanBaseWidget*> &allPages)
{
	_list->clear();

	int number = 0;

	for(vector<BanBaseWidget*>::iterator i = allPages.begin(); i != allPages.end(); ++i, ++number)
	{
		BanBaseWidget* materialWidget = *i;
		QPixmap pixmap = materialWidget->ToFullPixmap(1, 1, true);
		QIcon icon(pixmap);
		QString title = tr("第") + QString::number(number + 1) + tr("块");
		QListWidgetItem* item = new QListWidgetItem(icon, title);
		item->setData(Qt::UserRole, QVariant(number));
		_list->addItem(item);
	}

	show();
	exec();
}

void KePages::OnItemDoubleClickTriggered(QListWidgetItem *item)
{
	int number = (item->data(Qt::UserRole)).toInt();
	Q_EMIT OnSelectPage(number);
	close();
}