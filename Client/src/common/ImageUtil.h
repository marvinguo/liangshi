#pragma once

#include "Qt.h"
#include "library.h"
#include "config.h"
#include "stl.h"
#include "../../../SharedCode/LokiBuffer.h"

class ImageUtil
{
public:
	static QPixmap ToFixsizeBackgroundPixmap(int width, int height, QPixmap& pixmap);
};

