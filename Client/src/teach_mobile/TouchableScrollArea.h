#pragma once
#include "../common/Qt.h"
#include "flickcharm.h"

class TouchableScrollArea : public QScrollArea
{
	Q_OBJECT
public:
	TouchableScrollArea(QWidget *parent = 0);

private:
	FlickCharm _charm;
};


