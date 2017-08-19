#pragma once

#include "Qt.h"
#include "library.h"
#include "SPoint.h"
#include "../../../SharedCode/LokiBuffer.h"

class FontUtil
{
public:
	static QFont GetTextFontBySize(int size, bool hasNoneEnglish);
	static QFont GetTextFontByPixel(int pixel, bool hasNoneEnglish);
	static int GetTabWidth(QFont& font);
	static int GetStandardFontPixelSize(int size);
	static bool HasNoneEnglish(QString& str);
};

