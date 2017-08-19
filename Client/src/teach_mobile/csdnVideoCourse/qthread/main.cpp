#include "widget.h"
#include <QApplication>
#include <QThread>
#include <QDebug>
/*
class MyThread : public QThread
{
public:
    void run()
    {
        qDebug() <<"hello my thread, id – "<<QThread::currentThreadId();
        exec();
    }
};
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
/*
    qDebug() << "main thread id - " << QThread::currentThreadId();

    MyThread t;
    t.start();
*/
    return a.exec();
}
