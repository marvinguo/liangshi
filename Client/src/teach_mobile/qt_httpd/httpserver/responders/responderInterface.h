#ifndef RESPONDERINTERFACE_H
#define RESPONDERINTERFACE_H

#include <QObject>
//
// 所有的Responder都要从ResponderInterface派生。
// 因管理需要，ResponderInterface继承自QObject，以便管理器连接destroyed()信号来接收Responder销毁通知。
//

class ResponderInterface : public QObject
{
public:
    ResponderInterface(){}
    virtual ~ResponderInterface(){}

    /// @brief 处理请求
    ///
    /// @return 0 - 请求处理完毕；1 - 请求未处理完毕，需要继续处理
    virtual int handle() = 0;
    virtual bool runAlone() { return false; }
};

#endif // RESPONDERINTERFACE_H
