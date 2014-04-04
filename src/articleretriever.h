#ifndef ARTICLERETRIEVER_H
#define ARTICLERETRIEVER_H

#define URL_ARTICLELIST "http://www.cnbeta.com/articles/%1.htm"

#include <QObject>
#include <QtCore/QVariantMap>
#include <QtCore/QUrl>
#include <QtWebKit/QWebPage>

class ArticleRetriever : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    explicit ArticleRetriever(QObject *parent = 0);
    ~ArticleRetriever();

    Q_INVOKABLE void sendRequest(const QString &sid);

    bool loading() const;

signals:
    void loadingChanged();
    void dataReceived(const QVariantMap &data);

private slots:
    void slotLoadStarted();
    void slotLoadFinished(bool ok);

private:
    QUrl articleUrl(const QString &sid) const;

private:
    bool m_loading;
    QWebPage* webPage;
};

#endif // ARTICLERETRIEVER_H
