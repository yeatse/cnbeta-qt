#include "commentretriever.h"

#include <QtDeclarative>
#include "articleretriever.h"
#include "utility.h"

CommentRetriever::CommentRetriever(QObject *parent) :
    QObject(parent),
    m_loading(false)
{
}

CommentRetriever::~CommentRetriever()
{
    abortRequest();
}

bool CommentRetriever::loading() const
{
    return m_loading;
}

void CommentRetriever::sendRequest(const QString &sid, const QString &seriesNumber, const int &pageNumber)
{
    if (m_loading) return;

    m_loading = true;
    emit loadingChanged();

    QNetworkRequest req(QUrl(URL_COMMENTLIST));
    req.setRawHeader("user-agent", "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident/6.0)");
    req.setRawHeader("X-Requested-With", "XMLHttpRequest");
    req.setRawHeader("Referer", QString(URL_ARTICLECONTENT).arg(sid).toAscii());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray pending;
    pending.append(QString::number(pageNumber)).append(",").append(sid).append(",").append(seriesNumber);
    pending = pending.toBase64();
    QByteArray pending2 = QString::number((uint)time(NULL)).right(6).toAscii();
    pending.append(pending2.toBase64());
    pending = pending.replace("=", "%253D");
    pending.prepend("op=");

    req.setHeader(QNetworkRequest::ContentLengthHeader, pending.length());
    req.setRawHeader("Expect", "100-continue");

    reply = qmlEngine(this)->networkAccessManager()->post(req, pending);
    connect(reply.data(), SIGNAL(finished()), SLOT(slotRequestFinished()));
}

void CommentRetriever::abortRequest()
{
    if (reply && reply->isRunning()){
        reply->abort();
    }
}

void CommentRetriever::slotRequestFinished()
{
    reply->deleteLater();
    m_loading = false;
    emit loadingChanged();

    if (reply->error() == QNetworkReply::NoError){
        Utility* u = Utility::Instance();
        QByteArray replyData = u->gUncompress(reply->readAll());
        QVariantMap resp = u->jsonParse(replyData).toMap();
        if (resp.value("status").toString() == "success"){
            QByteArray result = QByteArray::fromBase64(resp.value("result").toByteArray());
            result.remove(0, 6);
            QVariantMap resultData = u->jsonParse(result).toMap();
            emit dataReceived(resultData);
        }
    }
}
