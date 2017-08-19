#ifndef RESPONDER_H_
#define RESPONDER_H_
#include "responderInterface.h"
#include "../serverconfig.h"
class HttpRequest;
class HttpResponse;
class ResponderPrivate;

class Responder : public ResponderInterface
{
    Q_OBJECT
public:
    Responder(HttpRequest *req, HttpResponse *resp);
    ~Responder();

    int handle();

private slots:
    void onResponseDone();

private:
    ResponderPrivate * m_d;
};


#endif
