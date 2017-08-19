#include "responderDebugHelper.h"
#include "httpRequest.h"
#include "httpResponse.h"
#include "../utils/asyncLog.h"
#include <QThread>

void showConnectionInfo(const char *tag, HttpRequest *req)
{
    aDebug() << tag << " client - " << req->remoteAddress() << ":" << req->remotePort()
                << " path - " << req->path() << endl;
}

void showRequestHeaders(const char *tag, HttpRequest *req)
{
    const HeaderHash & headerhash = req->headers();
    HeaderHash::const_iterator it = headerhash.begin();
    aDebug() << tag << " method - " << req->method() << endl;
    while(it != headerhash.end())
    {
        aDebug() << "header key-" << it.key()
                 << " val-" << it.value() << endl;
        it++;
    }
}
