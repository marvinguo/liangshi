#include "AddCorrectDialog.h"
#include "TitleButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

AddCorrectDialog::AddCorrectDialog():
QtSingleton<AddCorrectDialog>(this)
{
	setWindowIcon(QIcon(":/App/collect"));
	setWindowTitle(QObject::tr("选择正确答案"));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	setWindowFlags(flags);
	setModal(true);

	_list = new QListWidget();
	_list->setSelectionMode(QAbstractItemView::MultiSelection);
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(_list);

	QHBoxLayout* bottom = new QHBoxLayout();
	bottom->addStretch();
	TitleButton* ok = new TitleButton(tr("确定"));
	connect(ok, &TitleButton::clicked, this, &AddCorrectDialog::OnOKTriggered);
	TitleButton* cancel = new TitleButton(tr("取消"));
	connect(cancel, &TitleButton::clicked, this, &AddCorrectDialog::OnCancelTriggered);
	bottom->addWidget(ok);
	bottom->addWidget(cancel);

	mainLayout->addLayout(bottom);
	setLayout(mainLayout);

	setMinimumWidth(200);
	setMinimumHeight(400);
}

AddCorrectDialog::~AddCorrectDialog()
{
}

void AddCorrectDialog::ShowModal(vector<string>& allAnswers, vector<string>&correctLabels)
{
	_list->clear();
	for(vector<string>::iterator i = allAnswers.begin(); i != allAnswers.end(); ++i)
	{
		QString answer = QString(i->c_str());
		QListWidgetItem* item = new QListWidgetItem(answer);
		_list->addItem(item);
		
		for(vector<string>::iterator j = correctLabels.begin(); j != correctLabels.end(); ++j)
		{
			QString correct = QString(j->c_str());
			if(answer == correct)
			{
				_list->setItemSelected(item, true);
				break;
			}
		}
	}
	show();
	//exec();
}

void AddCorrectDialog::OnOKTriggered()
{
	vector<string> correct;

	int count = _list->selectedItems().count();

	foreach(QListWidgetItem *item, _list->selectedItems())
	{
		QString text = item->text();
		correct.push_back(text.toStdString());
	}

	Q_EMIT OnGetCorrect(correct);
	close();
}

void AddCorrectDialog::OnCancelTriggered()
{
	close();
}
