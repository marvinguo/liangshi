#include "DownloadAllProgress.h"
#include "../common/YunHttp.h"
#include "../common/QHttpFileDownloader.h"
#include "../common/FileUtil.h"
#include "LocalSave.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

DownloadAllProgress::DownloadAllProgress(QString classTitle, vector<KeDTO> kes, QWidget* parent):
QDialog(parent),
_classTitle(classTitle),
_currentCount(0),
_totalFileCount(0),
_kes(kes)
{
	setWindowIcon(QIcon(":/App/teach"));
	setWindowTitle(tr("正在努力下载中..."));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	flags = flags & (~Qt::WindowCloseButtonHint);
	setWindowFlags(flags);
	setModal(true);

	_progress = new QProgressBar();
	_progress->setTextVisible(false);
	_progress->setFixedHeight(20);
	_progress->setMinimum(0);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(_progress);
	setLayout(mainLayout);

	setFixedHeight(40);
	setFixedWidth(300);

	//在这里下载
	//BeginDownload();
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &DownloadAllProgress::BeginDownload);
	timer->setSingleShot(true);
	timer->start(500);
}

void DownloadAllProgress::ShowModal()
{
	QSize screenSize = QApplication::desktop()->geometry().size();
	int primaryScreenWidth = screenSize.width();
	int primaryScreeHeight = screenSize.height();
	int widgetWidth = width();
	int widgetHeight = height();
	move(primaryScreenWidth / 2 - widgetWidth / 2, primaryScreeHeight / 2 - widgetHeight / 2);
	show();
	exec();
}

void DownloadAllProgress::BeginDownload()
{
	_totalFileCount = _kes.size();
	_progress->setMaximum(_totalFileCount);
	_currentCount = 0;

	//这种情况也点处理一下
	if(_totalFileCount == 0)
	{
		close();
		return;
	}

	LocalSave save;
	save.CleanKeCount(_classTitle, 0);
	for(vector<KeDTO>::iterator i = _kes.begin(); i != _kes.end(); ++i)
	{
		KeDTO dto = *i;
		QString url = dto.url;
		QString remote = "/ke/" + url;
		QString local = FileUtil::GetKeCacheFolder() + url;
		QFileInfo info(local);
		if(dto.filesize == info.size() && info.exists())
		{
			QMutexLocker lock(&_progressMutex);
			save.AddKeFile(_classTitle, dto);
			save.Save();
			_progress->setValue(++_currentCount);

			if(_currentCount >= _totalFileCount)
			{
				close();
				break;
			}
			continue;
		}
		else
		{
			QHttpFileDownloader* downloader = new QHttpFileDownloader("liangshi.oss-cn-hangzhou.aliyuncs.com", remote, local);
			downloader->SetData(QVariant::fromValue(dto));
			connect(downloader, &QHttpFileDownloader::DownloadFinished, this, &DownloadAllProgress::DownloadKeFileTriggered);
			downloader->Download();
		}
	}

	save.CleanKeCount(_classTitle, _kes.size());
}

void DownloadAllProgress::DownloadKeFileTriggered(QString localFile)
{
	QHttpFileDownloader* downloader = qobject_cast<QHttpFileDownloader *>(sender());
	KeDTO dto = downloader->GetData().value<KeDTO>();;
	if(downloader != NULL)
		delete downloader;

	{
		QMutexLocker lock(&_progressMutex);
		LocalSave save;
		save.AddKeFile(_classTitle, dto);
		_progress->setValue(++_currentCount);
	}

	if(_currentCount >= _totalFileCount)
	{
		close();
	}
}