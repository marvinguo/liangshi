#include "ConvertProgress.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#ifdef QT_DEBUG
#define LIB_PATH	"../../library/lib/Win32/Debug/"
#else
#define LIB_PATH	"../../library/lib/Win32/Release/"
#endif

#pragma comment(lib, LIB_PATH"Ares.lib")

ConvertProgress::ConvertProgress(PlayerWidget* playerWidget, int width, int height, QWidget* parent):
QDialog(parent),
_playerWidget(playerWidget),
_width(width),
_height(height),
_totalCount(0),
_currentCount(0)
{
	setWindowIcon(QIcon(":/App/guru"));
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

	//在这里开始吗？
	QTimer::singleShot(500, this, SLOT(Convert()));
}

void ConvertProgress::ShowModal()
{
	show();
	exec();
}

void ConvertProgress::Convert()
{
	//算一下total
	int totalTime = _playerWidget->GetTotalTime();
	//100ms一帧
	_totalCount = totalTime * 10 +2;
	_progress->setMaximum(_totalCount);

	//audio
	setWindowTitle(tr("正在转化声音..."));
	ConvertAudio();
	//video
	setWindowTitle(tr("正在转化视频..."));
	ConvertVideo();
	//合成
	setWindowTitle(tr("正在合成视频..."));
	MixAudioVideo();

	close();
}

void ConvertProgress::ConvertAudio()
{
	QString dest = QCoreApplication::applicationDirPath() + "/audio.ilbc";
	FileUtil::CopyFile(_playerWidget->_hadesFile, dest);

	//ffmpeg -y -i audio.ilbc -c:a libfdk_aac audio.aac
	QProcess* process = new QProcess(this);
	process->setWorkingDirectory(QCoreApplication::applicationDirPath());
	QStringList arguments;
	arguments << "-y";
	arguments << "-i";
	arguments << "audio.ilbc";
	arguments << "-c:a";
	arguments << "libfdk_aac";
	arguments << "audio.aac";
	process->start("ffmpeg.exe", arguments);
	process->waitForFinished(-1);
	_progress->setValue(++_currentCount);
}

void ConvertProgress::MixAudioVideo()
{
	//ffmpeg -y -i audio.aac -i video.h264 -map 0:0 -map 1:0 out.mp4
	QProcess* process = new QProcess(this);
	process->setWorkingDirectory(QCoreApplication::applicationDirPath());
	QStringList arguments;
	arguments << "-y";
	arguments << "-i";
	arguments << "audio.aac";
	arguments << "-i";
	arguments << "video.h264";
	arguments << "-map";
	arguments << "0:0";
	arguments << "-map";
	arguments << "1:0";
	arguments << "ok.mp4";
	process->start("ffmpeg.exe", arguments);
	process->waitForFinished(-1);
	_progress->setValue(++_currentCount);
}

void ConvertProgress::ConvertVideo()
{
	Ares* ares = new Ares();
	int totalTime = _playerWidget->GetTotalTime();
	_playerWidget->GotoTime(totalTime);

	QPixmap pixmap(_width, _height);
	_playerWidget->DrawBackground(pixmap);

	QString dest = QCoreApplication::applicationDirPath() + "/video.h264";
	ares->Init(dest.toUtf8().data(), pixmap.width(), pixmap.height());
	for(int i = 100; i <= totalTime; i+=100)
	{
		ConvertFrame(ares, i);
	}
	//最后一帧
	ConvertFrame(ares, totalTime - 1);
	ares->Release();
	delete ares;
}

void ConvertProgress::ConvertFrame(Ares* ares, int time)
{
	_playerWidget->GotoTime(time);
	QPixmap pixmap(1280, 800);
	_playerWidget->DrawBackground(pixmap);
	QImage image = pixmap.toImage();
	image = image.convertToFormat(QImage::Format_RGB888);
	image = image.rgbSwapped();

	//qimage 默认是小结尾 即 r是在低位
	//bmp等windows用大结尾，其本质是bgr

	char* data = (char*)(image.bits());
	ares->EncodeFrame(data);
	_progress->setValue(++_currentCount);
}
