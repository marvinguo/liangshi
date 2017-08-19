#include "ClasseActivity.h"
#include "MainWindow.h"
#include "MenuButton.h"
#include "LocalSave.h"
#include "HeaderLabel.h"
#include "ItemButton.h"
#include "DownloadAllProgress.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ClasseActivity::ClasseActivity(MainWindow* mainWindow):
MobileActivity(mainWindow),
_classLayout(NULL)
{
	connect(this, &ClasseActivity::OnCollect, _mainWindow, &MainWindow::OnGotoNewCollect);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	_title = new ActivityTitlebar(TitlebarType_Normal, tr(""));
	connect(_title, &ActivityTitlebar::OnBack, _mainWindow, &MainWindow::OnGotoMenu);
	mainLayout->addWidget(_title);

	_firstLayout = new QHBoxLayout();
	mainLayout->addLayout(_firstLayout);

	QWidget* viewer = new QWidget();
	_viewerLayout = new QVBoxLayout();
	viewer->setLayout(_viewerLayout);

	_scroll = new TouchableScrollArea();
	_scroll->setWidget(viewer);
	mainLayout->addWidget(_scroll);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);
}

void ClasseActivity::OnBackKey()
{
	_mainWindow->OnGotoMenu();
}

void ClasseActivity::OnPause()
{
}

void ClasseActivity::UpdateData(ClassDTO dto)
{
	//这个地方，如果太宽，就把界面撑宽了
	_title->SetTitleText(dto.title);
	_classDTO = dto;
	if(_classLayout != NULL)
		Util::ClearLayout(_classLayout);
	Util::ClearLayout(_firstLayout);
	Util::ClearLayout(_viewerLayout);

	if(dto.type == 1)
	{
		MenuButton* download = new MenuButton(tr("下载所有课件"));
		connect(download, &MenuButton::clicked, this, &ClasseActivity::OnDownlaodALLClicked);
		_firstLayout->addWidget(download);

		HeaderLabel* ke = new HeaderLabel(tr("精讲课件"));
		_viewerLayout->addWidget(ke);
		_classLayout = new QVBoxLayout();
		_viewerLayout->addLayout(_classLayout);
		_viewerLayout->addStretch();

	}
	if(dto.type == 2)
	{
//		MenuButton* collect = new MenuButton(tr("问问题"));
//		connect(collect, &MenuButton::clicked, this, &ClasseActivity::OnCollectClicked);
		MenuButton* live = new MenuButton(tr("直播辅导"));
		connect(live, &MenuButton::clicked, this, &ClasseActivity::OnLiveClicked);
		
		//问问题，这个，先不实现
		//_firstLayout->addWidget(collect);
		_firstLayout->addWidget(live);

		HeaderLabel* ke = new HeaderLabel(tr("班级课件"));
		_viewerLayout->addWidget(ke);
		_classLayout = new QVBoxLayout();
		_viewerLayout->addLayout(_classLayout);
		HeaderLabel* question = new HeaderLabel(tr("历史答疑"));
		_viewerLayout->addWidget(question);
		_questionLayout = new QVBoxLayout();
		_viewerLayout->addLayout(_questionLayout);
		_viewerLayout->addStretch();
	}

	LocalSave save;
	YunHttp* http = new YunHttp(save._cookie);
	connect(http, &YunHttp::GetKesFinished, this, &ClasseActivity::GetKesTriggered);
	http->GetKes(dto.id);
}

void ClasseActivity::GetKesTriggered(vector<KeDTO> kes)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;

	_kes = kes;
	for(vector<KeDTO>::iterator i = kes.begin(); i != kes.end(); ++i)
	{
		KeDTO dto = *i;
		QString title = tr("第") + QString::number(dto.index) + tr("讲: ") + dto.title;
		ItemButton* button = new ItemButton(title);
		connect(button, &ItemButton::clicked, this, &ClasseActivity::OnPlayClicked);
		button->SetKeDTO(dto);
		_classLayout->addWidget(button);
	}
}

void ClasseActivity::OnPlayClicked()
{
	ItemButton* button = dynamic_cast<ItemButton*>(sender());
	KeDTO dto = button->GetKeDTO();
	_mainWindow->OnGotoNewPlay(true, _classDTO.title, dto);
}

void ClasseActivity::OnCollectClicked()
{
	Q_EMIT OnCollect(_classDTO.id);
}

void ClasseActivity::OnLiveClicked()
{
	_mainWindow->OnGotoNewLive(true, _classDTO.all_teachers == "1", _classDTO.session);
}

void ClasseActivity::OnDownlaodALLClicked()
{
	DownloadAllProgress progress(_classDTO.title, _kes, this);
	progress.ShowModal();
}