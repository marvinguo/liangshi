#include "MainMenuActivity.h"
#include "MainWindow.h"
#include "KeItemButton.h"
#include "LocalSave.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MainMenuActivity::MainMenuActivity(MainWindow* mainWindow):
MobileActivity(mainWindow, NULL)
{
	connect(this, &MainMenuActivity::OnClass, _mainWindow, &MainWindow::OnGotoNewClass);
	connect(this, &MainMenuActivity::OnTeaching, _mainWindow, &MainWindow::OnGotoNewTeaching);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	_title = new ActivityTitlebar(TitlebarType_Normal, tr(""));
	connect(_title, &ActivityTitlebar::OnBack, _mainWindow, &MainWindow::OnGotoLogin);
	mainLayout->addWidget(_title);

	_itemLayout = new QVBoxLayout();

	QWidget* viewer = new QWidget();
	QVBoxLayout* viewerLayout = new QVBoxLayout();
	viewerLayout->addLayout(_itemLayout);
	viewerLayout->addStretch();
	viewerLayout->setSpacing(0);
	viewerLayout->setContentsMargins(0, 0, 0, 0);
	viewer->setLayout(viewerLayout);

	_scroll = new TouchableScrollArea();
	_scroll->setWidget(viewer);
	mainLayout->addWidget(_scroll);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);

}

void MainMenuActivity::OnBackKey()
{
	_mainWindow->OnGotoLogin();
}

void MainMenuActivity::OnPause()
{
}

void MainMenuActivity::UpdateData()
{
	qDebug() << "MainMenuActivity::UpdateData";
	Util::ClearLayout(_itemLayout);
	LocalSave save;
	YunHttp* http = new YunHttp(save._cookie);
	connect(http, &YunHttp::GetTypeFinished, this, &MainMenuActivity::GetTypeTriggered);
	http->GetType();
}

//重新登录，界面不行，这个bug
void MainMenuActivity::GetTypeTriggered(int type)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;
	//1 student 2 teacher 

	if(type == 1)
		_title->SetTitleText(tr("我的辅导班"));
	else if(type == 2)
		_title->SetTitleText(tr("我的教学"));

	LocalSave save;
	http = new YunHttp(save._cookie);
	connect(http, &YunHttp::GetClassesFinished, this, &MainMenuActivity::GetClassesTriggered);
	connect(http, &YunHttp::GetTeachingsFinished, this, &MainMenuActivity::GetTeachingsTriggered);
	if(type == 1)
		http->GetClasses();
	else if(type == 2)
		http->GetTeachings();
}

void MainMenuActivity::GetClassesTriggered(vector<ClassDTO> classes)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;

	for(vector<ClassDTO>::iterator i = classes.begin(); i != classes.end(); ++i)
	{
		ClassDTO dto = *i;
		KeItemButton* button = new KeItemButton(dto.title);
		connect(button, &KeItemButton::clicked, this, &MainMenuActivity::OnClassesClicked);
		button->SetClassDTO(dto);
		_itemLayout->addWidget(button);
	}
}

void MainMenuActivity::OnClassesClicked()
{
	KeItemButton* button = dynamic_cast<KeItemButton*>(sender());
	ClassDTO dto = button->GetClassDTO();
	Q_EMIT OnClass(dto);
}

void MainMenuActivity::GetTeachingsTriggered(vector<TeachingDTO> teachings)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;

	for(vector<TeachingDTO>::iterator i = teachings.begin(); i != teachings.end(); ++i)
	{
		TeachingDTO dto = *i;
		KeItemButton* button = new KeItemButton(dto.title);
		connect(button, &KeItemButton::clicked, this, &MainMenuActivity::OnTeachingsClicked);
		button->SetTeachingDTO(dto);
		_itemLayout->addWidget(button);
	}
}

void MainMenuActivity::OnTeachingsClicked()
{
	KeItemButton* button = dynamic_cast<KeItemButton*>(sender());
	TeachingDTO dto = button->GetTeachingDTO();
	Q_EMIT OnTeaching(dto);
}
