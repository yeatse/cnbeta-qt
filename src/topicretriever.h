#ifndef TOPICRETRIEVER_H
#define TOPICRETRIEVER_H

#define URL_TOPICSLIST "http://www.cnbeta.com/topics.htm"

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtWebKit/QWebPage>

class TopicRetriever : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    explicit TopicRetriever(QObject *parent = 0);
    ~TopicRetriever();

    Q_INVOKABLE void sendRequest(const int &pageNumber);

    bool loading() const;

signals:
    void loadingChanged();
    void dataReceived(const QVariantMap &data);

private slots:
    void slotLoadStarted();
    void slotLoadFinished(bool ok);

private:
    bool m_loading;
    QWebPage* webPage;
};

#endif // TOPICRETRIEVER_H
