#pragma once

#include "../common/Qt.h"
#include "../hera/KeWidget.h"

class BanTable : public QTableWidget
{
	Q_OBJECT
public:
	BanTable(QWidget* parent);
	~BanTable();

	void UpdateBans(KeWidget* keWidget);

private Q_SLOTS:
	void OnItemDoubleClickTirggered(QTableWidgetItem *item);
	void OnGetScBufferTriggered(LokiBuffer& buffer, QVariant& relatedData, bool canDelete);

private:
	KeWidget* _keWidget;
};

