#include "ActionTable.h"
#include "../common/config.h"
#include "../hera/ChineseMessageBox.h"
#include "../hera/KeFile.h"
#include "../hera/HeraMainWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ActionTable::ActionTable(QWidget* parent):
QTableWidget(parent)
{
	setColumnCount(2);

	QStringList labels;
	labels << tr("时间") << tr("类型");
	setHorizontalHeaderLabels(labels);
}

ActionTable::~ActionTable()
{
}

void ActionTable::OnItemDoubleClickTirggered(QTableWidgetItem *item)
{
	QString uuid = item->text();
	for(auto i = _keWidget->_allPages.begin(); i != _keWidget->_allPages.end(); ++i)
	{
		BanBaseWidget* ban = *i;
		if(ban->GetObjectUuid() == uuid)
		{
			HeraMainWidget* hera = HeraMainWidget::LoadFromLokiBuffer(NewableDocType_BanEmbedded, ban->GetMsgPackData(), qVariantFromValue((void*)ban), tr("编辑初始板书"));
			if(hera != NULL)
			{
				connect(hera, &HeraMainWidget::OnGetScBuffer, this, &ActionTable::OnGetScBufferTriggered);
				hera->ShowModal();
			}
			break;
		}
	}
}

void ActionTable::OnGetScBufferTriggered(LokiBuffer& buffer, QVariant& relatedData, bool canDelete)
{

}

void ActionTable::UpdateActions(KeWidget* keWidget)
{
	_keWidget = keWidget;
	clearContents();
	setRowCount(keWidget->_allPages.size());
	if(keWidget != NULL)
	{
		int index = 0;
	}
	resizeColumnsToContents();
}
