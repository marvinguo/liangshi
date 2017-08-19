#ifndef TIMEMEASURE_H
#define TIMEMEASURE_H
#include <QTime>
#include "asyncLog.h"

class TimeMeasure
{
public:
    TimeMeasure(const char *tag)
        : m_szTag(tag)
    {
        m_time.start();
    }

    ~TimeMeasure()
    {
        aDebug() << m_szTag << " elapsed - " << m_time.elapsed() << " ms" << endl;
    }

    QTime m_time;
    const char *m_szTag;
};

#endif // TIMEMEASURE_H
