#include "widget.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QFont>
#include "sizeUtil.h"
#include "qDebug2Logcat.h"
int main(int argc, char *argv[])
{
    installLogcatMessageHandler("scalableUI");
    QApplication a(argc, argv);
    Widget w;
    w.setMinimumSize(400, 400);
    w.show();

    QScreen *screen = a.primaryScreen();
    QFont f = a.font();
    qDebug() << "font pointSize - " << f.pointSize();
    int pixelSize = (f.pointSize() * screen->logicalDotsPerInch()) / 72;
    qDebug() << "font pixelSize - " << pixelSize;
    qDebug() << "font height in pixel - " << SizeUtil::instance().defaultFontHeight();
    qDebug() << "dpi - " << screen->logicalDotsPerInch();

    return a.exec();
}
