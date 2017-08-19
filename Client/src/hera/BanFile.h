#pragma once

#include "../common/library.h"
#include "../common/config.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "HeraDocument.h"
#include "BanDocument.h"
#include "../common/library.h"

class BanBaseWidget;
class BanElement;

class LIANGSHIEXPORT BanFile
{
public:
	BanFile(BanDocument* doc);
	virtual ~BanFile();

public:
	static bool LoadScImageOnly(QString filepath, QPixmap& pixmap);
	static bool LoadScThumbnailOnly(QString filepath, QPixmap& pixmap);
	static BanDocument* LoadAsScDocument(QString filepath, bool direct = true);
	static vector<BanElement*> LoadAsScElements(QString filepath, bool needTitle = false);

public:
	void SaveFile(QString filepath);
	void SaveFileWithAllElements(QString filepath);

private:
	static BanDocument* LoadScDocument(QString tempFolder, bool direct);
	static bool LoadScElements(QString filepath, vector<BanElement*>& elements, bool needTitle);
	void SaveImageFile(QString filepath);
	void SaveThumbnailFile(QString filepath);
	void SaveThisFile(QString filepath, bool allElement = false);
	void SaveScriptDepsFiles(QString folder);

private:
	BanDocument* _doc;
};

