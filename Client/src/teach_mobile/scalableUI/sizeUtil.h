#ifndef SIZEUTIL_H
#define SIZEUTIL_H
#include <QFont>
#include <QString>

class SizeUtil
{
private:
    SizeUtil(){}
    SizeUtil(const SizeUtil &);
    SizeUtil & operator=(const SizeUtil&);
public:
    ~SizeUtil(){}
    static SizeUtil & instance();
    int defaultFontHeight();
    int widthWithDefaultFont(const QString &text);
    int widthWithFont(const QString &text, int fontPointSize);
    int fontHeight(int fontPointSize);
    float dpiFactor();
};

#endif // SIZEUTIL_H
