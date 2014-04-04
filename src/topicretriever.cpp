#include "topicretriever.h"

#include <QtNetwork/QNetworkRequest>
#include <QtDeclarative>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

TopicRetriever::TopicRetriever(QObject *parent) :
    QObject(parent),
    m_loading(false),
    webPage(0)
{
    webPage = new QWebPage(this);
    QWebSettings* settings = webPage->settings();
    settings->setAttribute(QWebSettings::AutoLoadImages, false);
    settings->setAttribute(QWebSettings::JavascriptEnabled, false);

    connect(webPage, SIGNAL(loadStarted()), SLOT(slotLoadStarted()));
    connect(webPage, SIGNAL(loadFinished(bool)), SLOT(slotLoadFinished(bool)));
}

TopicRetriever::~TopicRetriever()
{
}

void TopicRetriever::sendRequest(const int &pageNumber)
{
    QUrl url(URL_TOPICSLIST);
    url.addQueryItem("page", QString::number(pageNumber));
    QNetworkRequest req(url);
    req.setRawHeader("user-agent", "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident/6.0)");
    req.setRawHeader("X-Requested-With", "XMLHttpRequest");
    req.setRawHeader("Referer", "http://www.cnbeta.com/");

    webPage->setNetworkAccessManager(qmlEngine(this)->networkAccessManager());
    webPage->mainFrame()->load(req);
}

bool TopicRetriever::loading() const
{
    return m_loading;
}

void TopicRetriever::slotLoadStarted()
{
    m_loading = true;
    emit loadingChanged();
}

void TopicRetriever::slotLoadFinished(bool ok)
{
    m_loading = false;
    emit loadingChanged();

    if (ok){
        QVariantMap result;

        QWebElement topicContainer = webPage->mainFrame()->findFirstElement("div.content_body.topic_list.fl");

        QWebElement topic = topicContainer.findFirst("dl");
        QVariantList contentList;
        QVariantMap contentMap;
        QRegExp regexp("(\\\d+)\\\.htm");
        while (!topic.isNull()) {
            contentMap.clear();
            regexp.indexIn(topic.findFirst("a").attribute("href"));
            contentMap.insert("id", regexp.cap(1));
            contentMap.insert("logo", topic.findFirst("img").attribute("src"));
            contentMap.insert("name", topic.findFirst("dd").toPlainText());
            contentList.append(contentMap);
            topic = topic.nextSibling();
        }
        result.insert("content", contentList);

        QWebElement pages = topicContainer.findFirst("div#yw0");
        result.insert("hasPrev", !pages.findFirst("a.page-prev").isNull());
        result.insert("hasMore", !pages.findFirst("a.page-next").isNull());
        result.insert("pageNumber", pages.findFirst("strong").toPlainText().toInt());

        emit dataReceived(result);
    }
}
