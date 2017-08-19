#include "ElementTable.h"
#include "../common/config.h"
#include "../hera/ChineseMessageBox.h"
#include "../hera/KeFile.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ElementTable::ElementTable(QWidget* parent):
QTableWidget(parent)
{
	setColumnCount(3);

	QStringList labels;
	labels << tr("类型") << tr("颜色") << tr("宽度");
	setHorizontalHeaderLabels(labels);
}

ElementTable::~ElementTable()
{
}
