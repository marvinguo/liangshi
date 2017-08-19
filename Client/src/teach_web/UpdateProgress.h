#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class UpdateProgress : public QDialog
{
	Q_OBJECT
public:
	UpdateProgress(QWidget* parent = 0);

	void ShowModal();

private Q_SLOTS:
	void OnGetUpdateUrlTriggered(QString url);
	void OnDownloadJsonTriggered(QString jsonFile);
	void OnDownloadFileTriggered(QString localFile);

private:
	void DownloadUpdateFile();
	QMap<QString, QString> ParseJsonFile(QString path);

private:
	QProgressBar* _progress;
	QString _updateUrl;
	QString _localUpdateFolder;
	int _currentCount;
	int _totalFileCount;
};

