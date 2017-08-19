#pragma once

#include "../common/library.h"
#include "../common/config.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "KeWidget.h"
#include "PlayerWidget.h"

class LIANGSHIEXPORT KeFile
{
public:
	KeFile(KeWidget* doc);
	virtual ~KeFile();

public:
	static PlayerWidget* LoadKeAsPlayerWidget(QString filepath);
	static KeWidget* LoadKeAsKeWidget(QString filepath);

public:
	void SaveFile(QString filepath);

private:
	static PlayerWidget* LoadPlayerFile(QString filepath);
	static void LoadPlayerPages(PlayerWidget* player, QString filepath);
	static void LoadPlayerPage(PlayerWidget* player, QString filepath);
	static void LoadPlayerActions(PlayerWidget* player, QString filepath);

	void SaveThisFile(QString filepath);
	void SavePages(QString filepath);
	void SaveTests(QString filepath);
	void SaveActions(QString filepath);
	void SaveAudio(QString filepath);

private:
	KeWidget* _ke;
};

