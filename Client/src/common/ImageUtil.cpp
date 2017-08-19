#include "ImageUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

QPixmap ImageUtil::ToFixsizeBackgroundPixmap(int width, int height, QPixmap& pixmap)
{
	QPixmap temp(width, height);
	temp.fill(QColor(BLACKBOARD_BACKGROUND_COLOR));
	QPainter painter(&temp);
	painter.drawPixmap(0, 0, width, height, pixmap);
	//p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	return temp;
}
