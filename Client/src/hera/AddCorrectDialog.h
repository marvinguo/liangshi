#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/stl.h"
#include "../common/QtSingleton.h"
#include "BanCategoryList.h"
#include "BanList.h"

class LIANGSHIEXPORT AddCorrectDialog : public QDialog, public QtSingleton<AddCorrectDialog>
{
	Q_OBJECT
public:
	AddCorrectDialog();
	virtual ~AddCorrectDialog();
	void ShowModal(vector<string>& allAnswers, vector<string>&correctLabels);

Q_SIGNALS:
	void OnGetCorrect(vector<string>& correct);

private Q_SLOTS:
	void OnOKTriggered();
	void OnCancelTriggered();

private:
	QListWidget* _list;
};

