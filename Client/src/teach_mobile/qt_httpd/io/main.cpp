#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile bin("binary.dat");
    if(bin.open(QFile::WriteOnly))
    {
        unsigned char data[32] = {
            0x0, 0x20, 0x1, 0x4F, 0x80, 0x11, 0xFE, 0x3,
            0x0, 0x20, 0x2, 0x4F, 0x50, 0x11, 0xFE, 0x3,
            0x7, 0x20, 0x6, 0x4F, 0x80, 0x13, 0x5E, 0x3,
            0x3, 0x20, 0x1, 0x4F, 0x40, 0x19, 0x6B, 0x3
        };
        bin.write((const char*)data, 32);

        QByteArray ba;
        ba.append('a');
        ba.append("xixi I\'m happy.");
        bin.write(ba);

        qDebug() << "length of binary.dat = " << bin.size();

        bin.close();
    }

    {
        QFile txt("plain.txt");
        if(txt.open(QFile::WriteOnly))
        {
            QTextStream ts(&txt);
            ts.setCodec("UTF-8");
            ts << "Hello Jack" << endl;
            ts << QString::fromUtf8("  I\'m Mary, 胡说八道");
            ts.flush();
            txt.close();
        }
    }

    QFile f("plain.txt");
    if(f.open(QFile::ReadOnly))
    {
        QTextStream ts(&f);
        ts.setCodec(QTextCodec::codecForName("UTF-8"));
        while(!ts.atEnd())
        {
            qDebug() << ts.readLine();
        }
        f.close();
    }

    return a.exec();
}
