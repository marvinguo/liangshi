#include "BanTable.h"
#include "../common/config.h"
#include "../hera/ChineseMessageBox.h"
#include "../hera/KeFile.h"
#include "../hera/HeraMainWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanTable::BanTable(QWidget* parent):
QTableWidget(parent)
{
	setColumnCount(1);

	QStringList labels;
	labels << tr("UUID");
	setHorizontalHeaderLabels(labels);

	connect(this, &BanTable::itemDoubleClicked, this, &BanTable::OnItemDoubleClickTirggered);
}

BanTable::~BanTable()
{
}

void BanTable::OnItemDoubleClickTirggered(QTableWidgetItem *item)
{
	QString uuid = item->text();
	for(auto i = _keWidget->_allPages.begin(); i != _keWidget->_allPages.end(); ++i)
	{
		BanBaseWidget* ban = *i;
		if(ban->GetObjectUuid() == uuid)
		{
			HeraMainWidget* hera = HeraMainWidget::LoadFromLokiBuffer(NewableDocType_BanEmbedded, ban->GetMsgPackData(), qVariantFromValue(uuid), tr("编辑初始板书"));
			if(hera != NULL)
			{
				connect(hera, &HeraMainWidget::OnGetScBuffer, this, &BanTable::OnGetScBufferTriggered);
				hera->ShowModal();
			}
			break;
		}
	}
}

void BanTable::OnGetScBufferTriggered(LokiBuffer& buffer, QVariant& relatedData, bool canDelete)
{
	if(canDelete)
		return;
	QString uuid = relatedData.toString();
	for(auto i = _keWidget->_allPages.begin(); i != _keWidget->_allPages.end(); ++i)
	{
		BanBaseWidget* ban = *i;
		if(ban->GetObjectUuid() == uuid)
		{
			ban->SetMsgPackData(buffer);
			break;
		}
	}
}

void BanTable::UpdateBans(KeWidget* keWidget)
{
	_keWidget = keWidget;
	clearContents();
	setRowCount(keWidget->_allPages.size());
	if(keWidget != NULL)
	{
		int index = 0;
		for(auto i = keWidget->_allPages.begin(); i != keWidget->_allPages.end(); ++i)
		{
			BanBaseWidget* ban = *i;
			QTableWidgetItem *item = new QTableWidgetItem(ban->GetObjectUuid());
			item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
			setItem(index++, 0, item);
		}
	}
	resizeColumnsToContents();
}
