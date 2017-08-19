#ifndef EXTRACTVALUE_H
#define EXTRACTVALUE_H

#include <QByteArray>

/// @brief 提取两个tag之间的字符串
///
/// @param contents 源
/// @param tag 起始tag
/// @param tagLen 起始tag的长度
/// @param closeTag 结束tag
/// @param from 源的起始位置
/// @param to 源的结束位置
///
/// @return 提取到的字符串，找不到时为空
inline QByteArray extractValue(const QByteArray &contents, const char *tag, int tagLen
                          , const char * closeTag, int from, int to)
{
    int start = contents.indexOf(tag, from);
    if(start == -1)
    {
        return QByteArray();
    }
    start += tagLen;
    if(start < to)
    {
        int end = contents.indexOf(closeTag, start);
        if(end > start && end < to)
        {
            return contents.mid(start, end - start);
        }
    }
    return QByteArray();
}


inline void trimChar(QByteArray & ba, char ch)
{
    if(ba.endsWith(ch))ba.chop(1);
    if(ba.startsWith(ch))ba.remove(0,1);
}

#endif // EXTRACTVALUE_H
