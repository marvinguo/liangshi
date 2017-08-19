#pragma once

#include "../common/Qt.h"
#include "../hera/KeWidget.h"

class ActionTable : public QTableWidget
{
	Q_OBJECT
public:
	ActionTable(QWidget* parent);
	~ActionTable();

	void UpdateActions(KeWidget* keWidget);

private Q_SLOTS:
	void OnItemDoubleClickTirggered(QTableWidgetItem *item);
	void OnGetScBufferTriggered(LokiBuffer& buffer, QVariant& relatedData, bool canDelete);

private:
	KeWidget* _keWidget;
};

