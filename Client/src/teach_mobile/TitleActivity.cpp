#include "TitleActivity.h"
#include "MainWindow.h"
#include "MenuButton.h"
#include "LocalSave.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TitleActivity::TitleActivity(MainWindow* mainWindow):
MobileActivity(mainWindow, NULL)
{
	connect(this, &TitleActivity::OnClass, _mainWindow, &MainWindow::OnGotoNewClass);
}

void TitleActivity::OnBackKey()
{
}

void TitleActivity::OnPause()
{
}

void TitleActivity::OnInitialize()
{
	qDebug() << "TitleActivity::OnInitialize";
	LocalSave save;
	YunHttp* http = new YunHttp(save._cookie);
	connect(http, &YunHttp::GetTypeFinished, this, &TitleActivity::GetTypeTriggered);
	http->GetType();
}

void TitleActivity::GetTypeTriggered(int type)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;
	//1 student 2 teacher 

	LocalSave save;
	http = new YunHttp(save._cookie);
	connect(http, &YunHttp::GetClassesFinished, this, &TitleActivity::GetClassesTriggered);
	connect(http, &YunHttp::GetTeachingsFinished, this, &TitleActivity::GetTeachingsTriggered);
	if(type == 1)
		http->GetClasses();
	else if(type == 2)
		http->GetTeachings();
	
	//想想得到type以后再构造吧
	//构造主界面，以后其它的界面都模仿这个界面，想了想，是不是用tablewidget好一点
	QVBoxLayout* mainLayout = new QVBoxLayout();
	ActivityTitlebar* title = NULL;

	if(type == 1)
		title = new ActivityTitlebar(TitlebarType_Normal, tr("我的辅导班"));
	else if(type == 2)
		title = new ActivityTitlebar(TitlebarType_Normal, tr("我的教学"));

	connect(title, &ActivityTitlebar::OnBack, _mainWindow, &MainWindow::OnGotoLogin);

	mainLayout->addWidget(title);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);
}

void TitleActivity::GetClassesTriggered(vector<ClassDTO> classes)
{
	int row = 0;
	for(vector<ClassDTO>::iterator i = classes.begin(); i != classes.end(); ++i, ++row)
	{
		ClassDTO dto = *i;
		QListWidgetItem* item = new QListWidgetItem(dto.title);
		item->setData(Qt::UserRole, QVariant::fromValue(dto));
	}
}

void TitleActivity::OnClassesClicked(QListWidgetItem *item)
{
	ClassDTO dto = item->data(Qt::UserRole).value<ClassDTO>();
	Q_EMIT OnClass(dto);
}

void TitleActivity::GetTeachingsTriggered(vector<TeachingDTO> teachings)
{
}

void TitleActivity::OnTeachingsClicked(QListWidgetItem *item)
{

}
