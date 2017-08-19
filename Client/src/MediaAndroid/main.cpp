
#define QT_NO_EMIT
#include "mainwindow.h"
#include "../../library/include/Media.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Media* media = new Media();
    QString mediaFile = QDir::tempPath() + "/" + QUuid::createUuid().toString();
    media->InitRecorder(const_cast<char*>(mediaFile.toStdString().c_str()));
    //media->StartRecording();

    MainWindow w;
    w.show();
    return a.exec();
}
