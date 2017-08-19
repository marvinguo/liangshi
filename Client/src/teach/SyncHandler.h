#pragma once

#include "../common/Qt.h"
#include "../common/stl.h"
#include "../common/TyrProtocol.h"

class SyncHandler : public QObject
{
	Q_OBJECT
public:
	SyncHandler(QObject *parent = 0);
	~SyncHandler();

	void SetTyrProtocol(TyrProtocol* protocol){_sync = protocol;}


private Q_SLOTS:
	void OnCommandTriggered(Tyr::TyrCommand* command);

private:
	vector<Tyr::FileSyncInfo> GetFolderFiles(QString path);

private:
	TyrProtocol* _sync;
};

