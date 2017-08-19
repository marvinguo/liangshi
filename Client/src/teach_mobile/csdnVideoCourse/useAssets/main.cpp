#include <QApplication>
#include <QLabel>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLabel label;
    QPixmap pixmap("assets:/test.jpg");
    label.setPixmap(pixmap);
    label.show();

    return a.exec();
}
