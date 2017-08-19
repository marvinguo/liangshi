#include "responder.h"
#include "httpRequest.h"
#include "httpResponse.h"
#include <QEvent>
#include <QTimerEvent>
#include <QFileInfo>
#include <QStringList>
#include "../utils/asyncLog.h"
#include "serverconfig.h"
#include "responderDebugHelper.h"

#define FR_STATE_DELETE_LATER  0x1
#define FR_STATE_END_DIRECTLY  0x2
#define FR_STATE_WAIT_END      0x4

static const char * g_ext_to_content_type[] = {
    ".001","application/x-001"
    ,".301","application/x-301"
    ,".323","text/h323"
    ,".906","application/x-906"
    ,".907","drawing/907"
    ,".a11","application/x-a11"
    ,".acp","audio/x-mei-aac"
    ,".ai","application/postscript"
    ,".aif","audio/aiff"
    ,".aifc","audio/aiff"
    ,".aiff","audio/aiff"
    ,".anv","application/x-anv"
    ,".asa","text/asa"
    ,".asf","video/x-ms-asf"
    ,".asp","text/asp"
    ,".asx","video/x-ms-asf"
    ,".au","audio/basic"
    ,".avi","video/avi"
    ,".awf","application/vnd.adobe.workflow"
    ,".biz","text/xml"
    ,".bmp","application/x-bmp"
    ,".bot","application/x-bot"
    ,".c4t","application/x-c4t"
    ,".c90","application/x-c90"
    ,".cal","application/x-cals"
    ,".cat","application/vnd.ms-pki.seccat"
    ,".cdf","application/x-netcdf"
    ,".cdr","application/x-cdr"
    ,".cel","application/x-cel"
    ,".cer","application/x-x509-ca-cert"
    ,".cg4","application/x-g4"
    ,".cgm","application/x-cgm"
    ,".cit","application/x-cit"
    ,".class","java/*"
    ,".cml","text/xml"
    ,".cmp","application/x-cmp"
    ,".cmx","application/x-cmx"
    ,".cot","application/x-cot"
    ,".conf","text/plain"
    ,".crl","application/pkix-crl"
    ,".crt","application/x-x509-ca-cert"
    ,".csi","application/x-csi"
    ,".css","text/css"
    ,".cut","application/x-cut"
    ,".dbf","application/x-dbf"
    ,".dbm","application/x-dbm"
    ,".dbx","application/x-dbx"
    ,".dcd","text/xml"
    ,".dcx","application/x-dcx"
    ,".der","application/x-x509-ca-cert"
    ,".dgn","application/x-dgn"
    ,".dib","application/x-dib"
    ,".dll","application/x-msdownload"
    ,".doc","application/msword"
    ,".dot","application/msword"
    ,".drw","application/x-drw"
    ,".dtd","text/xml"
    ,".dwf","Model/vnd.dwf"
    ,".dwf","application/x-dwf"
    ,".dwg","application/x-dwg"
    ,".dxb","application/x-dxb"
    ,".dxf","application/x-dxf"
    ,".edn","application/vnd.adobe.edn"
    ,".emf","application/x-emf"
    ,".eml","message/rfc822"
    ,".ent","text/xml"
    ,".epi","application/x-epi"
    ,".eps","application/x-ps"
    ,".eps","application/postscript"
    ,".etd","application/x-ebx"
    ,".exe","application/x-msdownload"
    ,".fax","image/fax"
    ,".fdf","application/vnd.fdf"
    ,".fif","application/fractals"
    ,".fo","text/xml"
    ,".frm","application/x-frm"
    ,".g4","application/x-g4"
    ,".gbr","application/x-gbr"
    ,".gcd","application/x-gcd"
    ,".gif","image/gif"
    ,".gl2","application/x-gl2"
    ,".gp4","application/x-gp4"
    ,".hgl","application/x-hgl"
    ,".hmr","application/x-hmr"
    ,".hpg","application/x-hpgl"
    ,".hpl","application/x-hpl"
    ,".hqx","application/mac-binhex40"
    ,".hrf","application/x-hrf"
    ,".hta","application/hta"
    ,".htc","text/x-component"
    ,".htm","text/html"
    ,".html","text/html"
    ,".htt","text/webviewhtml"
    ,".htx","text/html"
    ,".icb","application/x-icb"
    ,".ico","image/x-icon"
    ,".ico","application/x-ico"
    ,".iff","application/x-iff"
    ,".ig4","application/x-g4"
    ,".igs","application/x-igs"
    ,".iii","application/x-iphone"
    ,".img","application/x-img"
    ,".ini","text/plain"
    ,".ins","application/x-internet-signup"
    ,".isp","application/x-internet-signup"
    ,".IVF","video/x-ivf"
    ,".java","java/*"
    ,".jfif","image/jpeg"
    ,".jpe","image/jpeg"
    ,".jpe","application/x-jpe"
    ,".jpeg","image/jpeg"
    ,".jpg","image/jpeg"
    ,".jpg","application/x-jpg"
    ,".js","application/x-javascript"
    ,".jsp","text/html"
    ,".la1","audio/x-liquid-file"
    ,".lar","application/x-laplayer-reg"
    ,".latex","application/x-latex"
    ,".lavs","audio/x-liquid-secure"
    ,".lbm","application/x-lbm"
    ,".lmsff","audio/x-la-lms"
    ,".log","text/plain"
    ,".ls","application/x-javascript"
    ,".ltr","application/x-ltr"
    ,".m1v","video/x-mpeg"
    ,".m2v","video/x-mpeg"
    ,".m3u","audio/mpegurl"
    ,".m4e","video/mpeg4"
    ,".mac","application/x-mac"
    ,".man","application/x-troff-man"
    ,".math","text/xml"
    ,".mdb","application/msaccess"
    ,".mdb","application/x-mdb"
    ,".mfp","application/x-shockwave-flash"
    ,".mht","message/rfc822"
    ,".mhtml","message/rfc822"
    ,".mi","application/x-mi"
    ,".mid","audio/mid"
    ,".midi","audio/mid"
    ,".mil","application/x-mil"
    ,".mml","text/xml"
    ,".mnd","audio/x-musicnet-download"
    ,".mns","audio/x-musicnet-stream"
    ,".mocha","application/x-javascript"
    ,".movie","video/x-sgi-movie"
    ,".mp1","audio/mp1"
    ,".mp2","audio/mp2"
    ,".mp2v","video/mpeg"
    ,".mp3","audio/mp3"
    ,".mp4","video/mpeg4"
    ,".mpa","video/x-mpg"
    ,".mpd","application/vnd.ms-project"
    ,".mpe","video/x-mpeg"
    ,".mpeg","video/mpg"
    ,".mpg","video/mpg"
    ,".mpga","audio/rn-mpeg"
    ,".mpp","application/vnd.ms-project"
    ,".mps","video/x-mpeg"
    ,".mpt","application/vnd.ms-project"
    ,".mpv","video/mpg"
    ,".mpv2","video/mpeg"
    ,".mpw","application/vnd.ms-project"
    ,".mpx","application/vnd.ms-project"
    ,".mtx","text/xml"
    ,".mxp","application/x-mmxp"
    ,".net","image/pnetvue"
    ,".nrf","application/x-nrf"
    ,".nws","message/rfc822"
    ,".odc","text/x-ms-odc"
    ,".out","application/x-out"
    ,".p10","application/pkcs10"
    ,".p12","application/x-pkcs12"
    ,".p7b","application/x-pkcs7-certificates"
    ,".p7c","application/pkcs7-mime"
    ,".p7m","application/pkcs7-mime"
    ,".p7r","application/x-pkcs7-certreqresp"
    ,".p7s","application/pkcs7-signature"
    ,".pc5","application/x-pc5"
    ,".pci","application/x-pci"
    ,".pcl","application/x-pcl"
    ,".pcx","application/x-pcx"
    ,".pdf","application/pdf"
    ,".pdf","application/pdf"
    ,".pdx","application/vnd.adobe.pdx"
    ,".pfx","application/x-pkcs12"
    ,".pgl","application/x-pgl"
    ,".pic","application/x-pic"
    ,".pko","application/vnd.ms-pki.pko"
    ,".pl","application/x-perl"
    ,".plg","text/html"
    ,".pls","audio/scpls"
    ,".plt","application/x-plt"
    ,".png","image/png"
    ,".png","application/x-png"
    ,".pot","application/vnd.ms-powerpoint"
    ,".ppa","application/vnd.ms-powerpoint"
    ,".ppm","application/x-ppm"
    ,".pps","application/vnd.ms-powerpoint"
    ,".ppt","application/vnd.ms-powerpoint"
    ,".ppt","application/x-ppt"
    ,".pr","application/x-pr"
    ,".prf","application/pics-rules"
    ,".prn","application/x-prn"
    ,".prt","application/x-prt"
    ,".ps","application/x-ps"
    ,".ps","application/postscript"
    ,".ptn","application/x-ptn"
    ,".pwz","application/vnd.ms-powerpoint"
    ,".r3t","text/vnd.rn-realtext3d"
    ,".ra","audio/vnd.rn-realaudio"
    ,".ram","audio/x-pn-realaudio"
    ,".ras","application/x-ras"
    ,".rat","application/rat-file"
    ,".rdf","text/xml"
    ,".rec","application/vnd.rn-recording"
    ,".red","application/x-red"
    ,".rgb","application/x-rgb"
    ,".rjs","application/vnd.rn-realsystem-rjs"
    ,".rjt","application/vnd.rn-realsystem-rjt"
    ,".rlc","application/x-rlc"
    ,".rle","application/x-rle"
    ,".rm","application/vnd.rn-realmedia"
    ,".rmf","application/vnd.adobe.rmf"
    ,".rmi","audio/mid"
    ,".rmj","application/vnd.rn-realsystem-rmj"
    ,".rmm","audio/x-pn-realaudio"
    ,".rmp","application/vnd.rn-rn_music_package"
    ,".rms","application/vnd.rn-realmedia-secure"
    ,".rmvb","application/vnd.rn-realmedia-vbr"
    ,".rmx","application/vnd.rn-realsystem-rmx"
    ,".rnx","application/vnd.rn-realplayer"
    ,".rp","image/vnd.rn-realpix"
    ,".rpm","audio/x-pn-realaudio-plugin"
    ,".rsml","application/vnd.rn-rsml"
    ,".rt","text/vnd.rn-realtext"
    ,".rtf","application/msword"
    ,".rtf","application/x-rtf"
    ,".rv","video/vnd.rn-realvideo"
    ,".sam","application/x-sam"
    ,".sat","application/x-sat"
    ,".sdp","application/sdp"
    ,".sdw","application/x-sdw"
    ,".sit","application/x-stuffit"
    ,".slb","application/x-slb"
    ,".sld","application/x-sld"
    ,".slk","drawing/x-slk"
    ,".smi","application/smil"
    ,".smil","application/smil"
    ,".smk","application/x-smk"
    ,".snd","audio/basic"
    ,".sol","text/plain"
    ,".sor","text/plain"
    ,".spc","application/x-pkcs7-certificates"
    ,".spl","application/futuresplash"
    ,".spp","text/xml"
    ,".ssm","application/streamingmedia"
    ,".sst","application/vnd.ms-pki.certstore"
    ,".stl","application/vnd.ms-pki.stl"
    ,".stm","text/html"
    ,".sty","application/x-sty"
    ,".svg","text/xml"
    ,".swf","application/x-shockwave-flash"
    ,".tdf","application/x-tdf"
    ,".tg4","application/x-tg4"
    ,".tga","application/x-tga"
    ,".tif","image/tiff"
    ,".tif","application/x-tif"
    ,".tiff","image/tiff"
    ,".tld","text/xml"
    ,".top","drawing/x-top"
    ,".torrent","application/x-bittorrent"
    ,".tsd","text/xml"
    ,".txt","text/plain"
    ,".uin","application/x-icq"
    ,".uls","text/iuls"
    ,".vcf","text/x-vcard"
    ,".vda","application/x-vda"
    ,".vdx","application/vnd.visio"
    ,".vml","text/xml"
    ,".vpg","application/x-vpeg005"
    ,".vsd","application/vnd.visio"
    ,".vsd","application/x-vsd"
    ,".vss","application/vnd.visio"
    ,".vst","application/vnd.visio"
    ,".vst","application/x-vst"
    ,".vsw","application/vnd.visio"
    ,".vsx","application/vnd.visio"
    ,".vtx","application/vnd.visio"
    ,".vxml","text/xml"
    ,".wav","audio/wav"
    ,".wax","audio/x-ms-wax"
    ,".wb1","application/x-wb1"
    ,".wb2","application/x-wb2"
    ,".wb3","application/x-wb3"
    ,".wbmp","image/vnd.wap.wbmp"
    ,".wiz","application/msword"
    ,".wk3","application/x-wk3"
    ,".wk4","application/x-wk4"
    ,".wkq","application/x-wkq"
    ,".wks","application/x-wks"
    ,".wm","video/x-ms-wm"
    ,".wma","audio/x-ms-wma"
    ,".wmd","application/x-ms-wmd"
    ,".wmf","application/x-wmf"
    ,".wml","text/vnd.wap.wml"
    ,".wmv","video/x-ms-wmv"
    ,".wmx","video/x-ms-wmx"
    ,".wmz","application/x-ms-wmz"
    ,".wp6","application/x-wp6"
    ,".wpd","application/x-wpd"
    ,".wpg","application/x-wpg"
    ,".wpl","application/vnd.ms-wpl"
    ,".wq1","application/x-wq1"
    ,".wr1","application/x-wr1"
    ,".wri","application/x-wri"
    ,".wrk","application/x-wrk"
    ,".ws","application/x-ws"
    ,".ws2","application/x-ws"
    ,".wsc","text/scriptlet"
    ,".wsdl","text/xml"
    ,".wvx","video/x-ms-wvx"
    ,".xdp","application/vnd.adobe.xdp"
    ,".xdr","text/xml"
    ,".xfd","application/vnd.adobe.xfd"
    ,".xfdf","application/vnd.adobe.xfdf"
    ,".xhtml","text/html"
    ,".xls","application/vnd.ms-excel"
    ,".xls","application/x-xls"
    ,".xlw","application/x-xlw"
    ,".xml","text/xml"
    ,".xpl","audio/scpls"
    ,".xq","text/xml"
    ,".xql","text/xml"
    ,".xquery","text/xml"
    ,".xsd","text/xml"
    ,".xsl","text/xml"
    ,".xslt","text/xml"
    ,".xwd","application/x-xwd"
    ,".x_b","application/x-x_b"
    ,".x_t","application/x-x_t"
};

static const char * _get_content_type(char *filename)
{
    int i = 0;
    int size = sizeof(g_ext_to_content_type)/sizeof(g_ext_to_content_type[0]);

    char *ext = filename;
    char * slash = strrchr(filename, '/');
    if(slash)
    {
        slash++;
    }
    else
    {
        slash = filename;
    }

    ext = strrchr(filename, '.');
    if(!ext)
    {
        return "application/octet-stream";
    }

    for(; i < size; i += 2)
    {
        if(!strcmp(ext, g_ext_to_content_type[i]))
        {
            return g_ext_to_content_type[i+1];
        }
    }

    return "application/octet-stream";
}


#define RESPONDER_BUFFER_SIZE 1024*16

//header reference
//
//HTTP/1.1 200 OK
//Server: nginx/1.1.0
//Date: Wed, 16 Nov 2011 09:27:29 GMT
//Content-Type: application/octet-stream
//Content-Length: 21952873
//Last-Modified: Wed, 16 Nov 2011 01:21:30 GMT
//Connection: keep-alive
//Accept-Ranges: bytes

class ResponderPrivate
{
public:
    ResponderPrivate(Responder * responder, HttpRequest * req, HttpResponse * resp)
        : m_responder(responder)
        , m_config(HttpServerConfig::instance())
        , m_req(req)
        , m_resp(resp)
        , m_bytesSent(0)
        , m_file(0)
        , m_bytesClientRequest(0)
        , m_nState(0)
    {
        showConnectionInfo("file responder", m_req);
        if( 0 == prepare() ) m_nState = FR_STATE_END_DIRECTLY;
    }

    ~ResponderPrivate()
    {
        releaseResources();
    }

    QString uri2path(const QString & uri)
    {
        QString strURI(uri);
        QMap<QString, QString>::iterator it = m_config.m_alias.find(strURI);
        while(it == m_config.m_alias.end())
        {
            if(strURI.endsWith('/'))
            {
                strURI.chop(1);
                it = m_config.m_alias.find(strURI);
            }
            else
            {
                int sep = strURI.lastIndexOf('/');
                if(sep == -1)
                {
                    break;
                }

                strURI = strURI.left(sep+1);
                it = m_config.m_alias.find(strURI);
            }
        }

        if(it != m_config.m_alias.end())
        {
            QString strAlias = it.value();
            QString strURIPath(uri);
            strURIPath.remove(strURI);
            if(strURIPath.isEmpty())
            {
                return strAlias;
            }
            else
            {
                QString strLocalPath(strAlias);
                if(strAlias.endsWith('/'))
                {
                    if( strURIPath.startsWith('/') )
                    {
                        strLocalPath += strURIPath.mid(1);
                    }
                    else
                    {
                        strLocalPath += strURIPath;
                    }
                }
                else
                {
                    if(strURIPath.startsWith('/'))
                    {
                        strLocalPath += strURIPath;
                    }
                    else
                    {
                        strLocalPath += "/" + strURIPath;
                    }
                }
                aDebug() << "local path -%s" << strLocalPath << endl;
                return strLocalPath;
            }
        }
        else
        {
            QString strLocalPath(m_config.m_strDocumentRoot);
            if(strLocalPath.endsWith('/'))
            {
                if(uri.startsWith('/'))
                {
                    strLocalPath += uri.mid(1);
                }
                else
                {
                    strLocalPath += uri;
                }
            }
            else
            {
                if(uri.startsWith('/'))
                {
                    strLocalPath += uri;
                }
                else
                {
                    strLocalPath += "/" + uri;
                }
            }
            return strLocalPath;
        }
        return "";
    }

    void releaseResources()
    {
        if(m_req)
        {
            delete m_req;
            m_req = 0;
        }

        if(m_file)
        {
            m_file->close();
            delete m_file;
            m_file = 0;
        }
    }

    int sendFileData()
    {
        if(m_resp->bytesToWrite() >= RESPONDER_BUFFER_SIZE*2 )
        {
            return 1;
        }

        if(m_bytesSent == m_bytesClientRequest)
        {
            m_nState |= FR_STATE_WAIT_END;
            return 0;
        }

        qint64 toRead = (m_bytesSent + RESPONDER_BUFFER_SIZE) >= m_bytesClientRequest ? m_bytesClientRequest - m_bytesSent : RESPONDER_BUFFER_SIZE;
        qint64 realRead = m_file->read(m_buffer, toRead);
        if(realRead > 0)
        {
            m_resp->write(m_buffer, realRead);
            m_bytesSent += realRead;
        }
        else
        {
            m_file->close();
            delete m_file;
            m_file = 0;
            m_nState |= FR_STATE_WAIT_END;
            return 0;
        }

        return 1;
    }

    int prepare()
    {
        showRequestHeaders("file responder", m_req);

        const QString & method = m_req->method();

        if(method != "HEAD" && method != "GET")
        {
            m_resp->writeHead(403);
            return 0;
        }
        QString uriPath = m_req->path();
        if(isForbidden(uriPath))
        {
            m_resp->writeHead(403);
            return 0;
        }

        QString localPath = uri2path(uriPath);

        aInfo() << "uriPath - " << uriPath << " localPath - " << localPath << endl;

        if(uriPath == "/")
        {
            localPath = "index.htm";
        }

        QFileInfo fi(localPath);

        if(!fi.exists())
        {
            m_resp->writeHead(404);
            return 0;
        }

        if(!fi.isFile() )
        {
            m_resp->writeHead(403);
            return 0;
        }

        m_file = new QFile(localPath);
        if(!m_file->open(QFile::ReadOnly))
        {
            m_resp->writeHead(500);
            return 0;
        }

        int statusCode = 200;

        // range process
        qint64 r1 = 0;
        qint64 r2 = 0;
        QString strRequestRange = m_req->header("range");
        QString strContentRange;
        int rangeCount = 0;
        if(!strRequestRange.isEmpty())
        {
            //qDebug("header range - %s", strRequestRange.toAscii().data());
            strRequestRange.remove("bytes=");

            statusCode = 206;

            QStringList ranges = strRequestRange.split('-', QString::SkipEmptyParts);

            //aDebug() << "ranges - " << ranges << endl;

            rangeCount = ranges.size();
            if(rangeCount >= 1)
            {
                r1 = ranges.at(0).toLongLong();
            }
            if(rangeCount >= 2)
            {
                r2 = ranges.at(1).toLongLong();
            }
        }

        qint64 contentLength = (rangeCount == 2 ? r2 - r1 + 1: fi.size() - r1 );
        if(rangeCount > 0)
        {
            strContentRange = QString("bytes %1-%2/%3").arg(r1).arg(r1 + contentLength - 1).arg(fi.size());
            aDebug() << "Responder[common], " << strContentRange
                     << " r1 - " << r1
                     << " r2 - " << r2
                     << " cl - " << contentLength << endl;
        }

        if(r1 != 0 && !m_file->seek(r1))
        {
            m_resp->writeHead(500);
            return 0;
        }

        //TODO: add DATE, Last-Modified ... print header
        m_resp->setHeader("Content-Type", _get_content_type(localPath.toLatin1().data()));
        m_resp->setHeader("Content-Length", QString::number(contentLength));
        m_resp->setHeader("Accept-Ranges", "bytes");
        if(rangeCount > 0)
        {
            m_resp->setHeader("Content-Range", strContentRange);
        }
        m_resp->writeHead(statusCode);

        if(method == "HEAD")
        {
            return 0;
        }

        m_bytesClientRequest = contentLength;

        return 1;
    }

    /// @return 0 - end handle; 1 - not end
    int handle()
    {
        if(m_nState & FR_STATE_END_DIRECTLY)
        {
            m_resp->end();
            return 0;
        }

        if(m_nState & FR_STATE_WAIT_END)
        {
            if(m_resp->bytesToWrite() > 0) return 1;
            m_resp->end();
            return 0;
        }


        sendFileData();

        return 1;
    }

    bool isForbidden(const QString &uriPath)
    {
        int count = m_config.m_forbiddenPatters.size();
        for(int i = 0; i < count; i++)
        {
            const QString & patter = m_config.m_forbiddenPatters.at(i);
            if(uriPath.startsWith(patter)) return true;
        }
        return false;
    }

    Responder * m_responder;
    HttpServerConfig & m_config;
    HttpRequest *m_req;
    HttpResponse *m_resp;

    qint64 m_bytesSent;
    QFile * m_file;
    qint64 m_bytesClientRequest;
    int m_nState;
    char m_buffer[RESPONDER_BUFFER_SIZE];
};

Responder::Responder(HttpRequest *req, HttpResponse *resp)
    : m_d(new ResponderPrivate(this, req, resp))
{
    connect(resp, SIGNAL(done()), this, SLOT(onResponseDone()));
}

Responder::~Responder()
{
    delete m_d;
    m_d = 0;
}

int Responder::handle()
{
    return (m_d->m_nState & FR_STATE_DELETE_LATER) ? 0 : m_d->handle();
}

void Responder::onResponseDone()
{
    m_d->m_nState |= FR_STATE_DELETE_LATER;
    deleteLater();
}
