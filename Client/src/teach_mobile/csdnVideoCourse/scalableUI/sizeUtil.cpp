#include "sizeUtil.h"
#include <QApplication>
#include <QFontMetrics>
#include <QScreen>

SizeUtil & SizeUtil::instance()
{
    static SizeUtil util;
    return util;
}

int SizeUtil::defaultFontHeight()
{
    return qApp->fontMetrics().height();
}

int SizeUtil::widthWithDefaultFont(const QString &text)
{
    return qApp->fontMetrics().boundingRect(text).width();
}

int SizeUtil::widthWithFont(const QString &text, int fontPointSize)
{
    QFont f = qApp->font();
    f.setPointSize(fontPointSize);
    QFontMetrics fm(f);
    return fm.boundingRect(text).width();
}

int SizeUtil::fontHeight(int fontPointSize)
{
    QFont f = qApp->font();
    f.setPointSize(fontPointSize);
    QFontMetrics fm(f);
    return fm.height();
}

float SizeUtil::dpiFactor()
{
    QScreen *screen = qApp->primaryScreen();
    return 72 / screen->logicalDotsPerInch();
}
