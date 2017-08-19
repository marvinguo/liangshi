#ifndef PRIVHTTPHEADER_H
#define PRIVHTTPHEADER_H
#include <QString>

class PrivateHttpHeader
{
public:
    PrivateHttpHeader()
        : m_siteName("site-name"), m_videoKey("video-key"), m_encryption("encryption")
    {

    }
    QString m_siteName;
    QString m_videoKey;
    QString m_encryption;
};

#endif // PRIVHTTPHEADER_H
