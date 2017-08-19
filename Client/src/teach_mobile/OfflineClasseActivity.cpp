#include "OfflineClasseActivity.h"
#include "MainWindow.h"
#include "HeaderLabel.h"
#include "LocalSave.h"
#include "ItemButton.h"
#include "../common/FileUtil.h"
#include "../hera/ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

OfflineClasseActivity::OfflineClasseActivity(MainWindow* mainWindow):
MobileActivity(mainWindow)
{
	QVBoxLayout* mainLayout = new QVBoxLayout();
	_title = new ActivityTitlebar(TitlebarType_Normal, tr("离线学习"));
	connect(_title, &ActivityTitlebar::OnBack, _mainWindow, &MainWindow::OnGotoLogin);
	mainLayout->addWidget(_title);

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

void OfflineClasseActivity::OnBackKey()
{
	_mainWindow->OnGotoLogin();
}

void OfflineClasseActivity::OnPause()
{
}

void OfflineClasseActivity::UpdateData()
{
	//把班级直接列出来，点下就直接播放
	Util::ClearLayout(_viewerLayout);

	LocalSave save;

	QVector<CachedKe>& cachedKes = save._cachedKes;
	for(QVector<CachedKe>::iterator i = cachedKes.begin(); i != cachedKes.end(); ++i)
	{
		CachedKe ke = *i;
		if(ke._cource.isEmpty())
			continue;
		HeaderLabel* keHeader = new HeaderLabel(ke._cource);
		_viewerLayout->addWidget(keHeader);
		auto kes = ke._kes;
		for(auto j = kes.begin(); j != kes.end(); ++j)
		{
			KeDTO dto = *j;

			QString filepath = FileUtil::GetKeCacheFolder() + dto.url;
			QFileInfo info(filepath);
			if(dto.filesize == info.size() && info.exists())
			{
				QString title = tr("第") + QString::number(dto.index) + tr("讲: ") + dto.title;
				ItemButton* button = new ItemButton(title);
				connect(button, &ItemButton::clicked, this, &OfflineClasseActivity::OnPlayClicked);
				button->SetId(dto.url);
				button->SetKeDTO(dto);
				_viewerLayout->addWidget(button);
			}
			else
			{
				continue;
			}
		}
	}
	
	_viewerLayout->addStretch();
}

void OfflineClasseActivity::OnPlayClicked()
{
	qDebug() << "OfflineClasseActivity::OnPlayClicked";
	ItemButton* button = dynamic_cast<ItemButton*>(sender());
	QString filepath = FileUtil::GetKeCacheFolder() + button->GetId();
	QFileInfo info(filepath);
	KeDTO dto = button->GetKeDTO();
	if(dto.filesize == info.size() && info.exists())
	{
		_mainWindow->OnGotoPreview(true, filepath);
	}
	else
	{
		ChineseMessageBox::Show(tr("课件下载不完整，请重新下载"), tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
	}
}