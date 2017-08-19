#pragma once

#include "../common/Qt.h"
#include "../hera/KeWidget.h"

class ElementTable : public QTableWidget
{
	Q_OBJECT
public:
	ElementTable(QWidget* parent);
	~ElementTable();
};

