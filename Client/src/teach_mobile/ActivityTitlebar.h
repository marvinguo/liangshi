#pragma once

#include "../common/Qt.h"

#include "ActivityTitleButton.h"
#include "ActivityTitleLabel.h"
#include "PlayTitlebar.h"
#include "KeTitlebar.h"
#include "LiveTitlebar.h"

enum TitlebarType
{
	TitlebarType_Player,
	TitlebarType_Ke,
	TitlebarType_Live,
	TitlebarType_Normal,
};

class ActivityTitlebar : public QWidget
{
	Q_OBJECT
public:
	ActivityTitlebar(TitlebarType type, QString title, QWidget* parent = NULL);

	void SetTitleText(QString text);
	PlayTitlebar* GetPlayTitleBar(){return _play;}
	KeTitlebar* GetKeTitleBar(){return _ke;}
	LiveTitlebar*  GetLiveTitleBar(){return _live;}


Q_SIGNALS:
	void OnBack();

private:
	ActivityTitleLabel* _titleLabel;
	PlayTitlebar* _play;
	KeTitlebar* _ke;
	LiveTitlebar* _live;
};

