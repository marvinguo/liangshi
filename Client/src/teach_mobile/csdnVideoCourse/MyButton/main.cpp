#include "widget.h"
#include <QApplication>
#include "myButton.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    MyButton btn(&w);
    btn.move(5, 5);
    btn.setText("Quit");
    QObject::connect(&btn, SIGNAL(clicked()),
                     &a, SLOT(quit()));
    w.show();

    return a.exec();
}
