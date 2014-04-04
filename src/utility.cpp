#include "utility.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <zlib.h>

Utility::Utility(QObject *parent) :
    QObject(parent)
{
}

Utility::~Utility()
{
}

Utility* Utility::Instance()
{
    static Utility u;
    return &u;
}

QString Utility::appVersion() const
{
    return qApp->applicationVersion();
}

int Utility::qtVersion() const
{
    QString qtver(qVersion());
    QStringList vlist = qtver.split(".");
    if (vlist.length() >= 3){
        int major = vlist.at(0).toInt();
        int minor = vlist.at(1).toInt();
        int patch = vlist.at(2).toInt();
        return (major << 16) + (minor << 8) + patch;
    } else {
        return 0;
    }
}

void Utility::clearSettings()
{
    settings.clear();
}

QVariant Utility::jsonParse(const QByteArray &jsonData, bool *ok)
{
    return parser.parse(jsonData, ok);
}

QByteArray Utility::gUncompress(const QByteArray &data)
{
    if (data.size() <= 4){
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15+32);
    if (ret != Z_OK)
        return QByteArray();

    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);

        switch(ret){
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }
        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    inflateEnd(&strm);
    return result;
}

QVariant Utility::getValue(const QString &key, const QVariant defaultValue)
{
    return settings.value(key, defaultValue);
}

void Utility::setValue(const QString &key, const QVariant &value)
{
    settings.setValue(key, value);
}

QUrl Utility::fixUrl(const QUrl &url)
{
    QString urlString = url.toString();
    int index = QRegExp("\\\.(jpg|png|gif)_", Qt::CaseInsensitive).indexIn(urlString);
    if (index > 0){
        urlString.truncate(index+4);
        return QUrl(urlString);
    } else {
        return url;
    }
}
