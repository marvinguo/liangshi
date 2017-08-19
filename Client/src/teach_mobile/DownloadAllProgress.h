#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/YunHttp.h"

class DownloadAllProgress : public QDialog
{
	Q_OBJECT
public:
	DownloadAllProgress(QString classTitle, vector<KeDTO> dtos, QWidget* parent = 0);

	void ShowModal();

private Q_SLOTS:
	void DownloadKeFileTriggered(QString localFile);

private:
	void BeginDownload();

private:
	QProgressBar* _progress;
	QString _classTitle;
	vector<KeDTO> _kes;
	int _currentCount;
	int _totalFileCount;

	QMutex _progressMutex;
};

