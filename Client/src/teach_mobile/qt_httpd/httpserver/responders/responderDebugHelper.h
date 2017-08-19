#ifndef RESPONDERDEBUGHELPER_H
#define RESPONDERDEBUGHELPER_H

class HttpRequest;
void showConnectionInfo(const char *tag, HttpRequest *req);
void showRequestHeaders(const char *tag, HttpRequest *req);

#endif // RESPONDERDEBUGHELPER_H
