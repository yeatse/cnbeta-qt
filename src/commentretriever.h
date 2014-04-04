#ifndef COMMENTRETRIEVER_H
#define COMMENTRETRIEVER_H

#define URL_COMMENTLIST "http://www.cnbeta.com/cmt"

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtNetwork/QNetworkReply>

class CommentRetriever : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    explicit CommentRetriever(QObject *parent = 0);
    ~CommentRetriever();

    Q_INVOKABLE void sendRequest(const QString &sid, const QString &seriesNumber, const int &pageNumber);
    Q_INVOKABLE void abortRequest();

    bool loading() const;

signals:
    void loadingChanged();
    void dataReceived(const QVariantMap &data);

private slots:
    void slotRequestFinished();

private:
    bool m_loading;
    QPointer<QNetworkReply> reply;
};

#endif // COMMENTRETRIEVER_H
