#pragma once

#include "../common/library.h"
#include "../common/QtSingleton.h"
#include "AnimationAction.h"
#include "Element.h"
#include "HeraDocument.h"
#include "BanBaseWidget.h"
#include "Menu.h"

class LIANGSHIEXPORT KePages : public QDialog, public QtSingleton<KePages>
{
	Q_OBJECT
public:
	KePages(QWidget *parent = 0);
	virtual ~KePages();
	void ShowModal(vector<BanBaseWidget*> &allPages);

Q_SIGNALS:
	void OnSelectPage(int page);

private Q_SLOTS:
	void OnItemDoubleClickTriggered(QListWidgetItem *item);

private:
	QListWidget* _list;
};

