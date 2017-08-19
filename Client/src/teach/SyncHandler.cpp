#include "SyncHandler.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

SyncHandler::SyncHandler(QObject *parent):
QObject(parent)
{
}

SyncHandler::~SyncHandler()
{
}

vector<Tyr::FileSyncInfo> SyncHandler::GetFolderFiles(QString path)
{
	vector<Tyr::FileSyncInfo> files;
	QDir dir(path);
	bool exist = dir.exists();
	dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks);

	QFileInfoList list = dir.entryInfoList();

	foreach (QFileInfo file, list)
	{
		Tyr::FileSyncInfo f;
		f._filename = file.fileName().toStdString();
		f._isDir = file.isDir();
		f._size = file.size();
		files.push_back(f);
	}
	return files;
}

void SyncHandler::OnCommandTriggered(Tyr::TyrCommand* command)
{
	Tyr::LSCommand* ls = dynamic_cast<Tyr::LSCommand*>(command);
	if(ls != NULL)
	{
		QString path = ls->GetPath();
		if(path == "~")
		{
#ifdef Q_OS_IOS					
			path = QDir::homePath() + "/Documents";
#else
			path = QDir::homePath();
#endif
			QDir dir(path);
			dir.mkpath(".");	

		}
		ls->SetPath(path);
		vector<Tyr::FileSyncInfo> files = GetFolderFiles(path);
		ls->SetFiles(files);
		_sync->SendCommand(ls);
	}

	Tyr::PUTCommand* put = dynamic_cast<Tyr::PUTCommand*>(command);
	if(put != NULL)
	{
		QString filepath = put->GetPath();
		LokiBuffer buffer;
		buffer = put->GetContent();

		QFile file(filepath);
		file.open(QIODevice::WriteOnly);
		file.write(buffer.Data(), buffer.Size());
		file.flush();
		file.close();

		Tyr::LSCommand* ls = new Tyr::LSCommand();
		QFileInfo info(filepath);
		ls->SetPath(info.absoluteDir().absolutePath());
		vector<Tyr::FileSyncInfo> files = GetFolderFiles(info.absoluteDir().absolutePath());
		ls->SetFiles(files);
		_sync->SendCommand(ls);
		delete ls;
	}

}
