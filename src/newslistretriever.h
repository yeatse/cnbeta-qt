#ifndef NEWSLISTRETRIEVER_H
#define NEWSLISTRETRIEVER_H

#define URL_NEWSLIST "http://www.cnbeta.com/more.htm"
#define URL_TOPICNEWS "http://www.cnbeta.com/topics/more.htm"

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtNetwork/QNetworkReply>

class NewsListRetriever : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    explicit NewsListRetriever(QObject *parent = 0);
    ~NewsListRetriever();

    Q_INVOKABLE void sendRequest(const QString &type, const int &pageNumber);
    Q_INVOKABLE void abortRequest();

    bool loading() const;

signals:
    void loadingChanged();
    void dataReceived(const QString &type, const QVariant &list, const int &pageNumber);

private slots:
    void slotRequestFinished();

private:
    bool m_loading;
    int m_pageNumber;
    QString m_type;

    QPointer<QNetworkReply> reply;
};

#endif // NEWSLISTRETRIEVER_H
