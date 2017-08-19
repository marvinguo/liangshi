#pragma once

#include "../common/Qt.h"
#include "SelectNumberMenu.h"

class LIANGSHIEXPORT AudioVolumeIndicator : public QProgressBar
{
	Q_OBJECT
public:
	AudioVolumeIndicator(QString title, QWidget *parent = 0);
	~AudioVolumeIndicator();
};

