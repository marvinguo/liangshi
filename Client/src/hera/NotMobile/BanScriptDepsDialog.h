#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/QtSingleton.h"
#include "BanScriptDeps.h"
#include "BanScriptWidget.h"
#include "TitleButton.h"

class LIANGSHIEXPORT BanScriptDepsDialog : public QDialog, public QtSingleton<BanScriptDepsDialog>
{
	Q_OBJECT
public:
	BanScriptDepsDialog();
	~BanScriptDepsDialog();
	void ShowModal(BanScriptDeps* deps, BanScriptWidget* widget);

private Q_SLOTS:
	void OnListItemChangedTriggered();
	void OnListItemDoubleClickedTriggered(QListWidgetItem *item);
	void OnAddTriggered();
	void OnDeleteTriggered();
	void OnSaveAsTriggered();

private:
	BanScriptDeps* _deps;
	BanScriptWidget* _widget;
	QListWidget* _fileList;

	TitleButton* _addButton;
	TitleButton* _deleteButton;
	TitleButton* _saveAsButton;
};

