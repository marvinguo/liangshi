#include "CustomEvent.h"


QEvent::Type CustomEvent::m_evType = (QEvent::Type)QEvent::None;

CustomEvent::CustomEvent(int arg1, const QString &arg2)
    : QEvent(eventType()), m_arg1(arg1), m_arg2(arg2)
{}

CustomEvent::~CustomEvent()
{

}

QEvent::Type CustomEvent::eventType()
{
    if(m_evType == QEvent::None)
    {
        m_evType = (QEvent::Type)registerEventType();
    }
    return m_evType;
}

