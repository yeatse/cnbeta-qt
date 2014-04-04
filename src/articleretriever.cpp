#include "articleretriever.h"
#include <QtDeclarative>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>

ArticleRetriever::ArticleRetriever(QObject *parent) :
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

ArticleRetriever::~ArticleRetriever()
{
}

void ArticleRetriever::sendRequest(const QString &sid)
{
    QNetworkRequest req(articleUrl(sid));
    req.setRawHeader("user-agent", "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident/6.0)");
    req.setRawHeader("X-Requested-With", "XMLHttpRequest");
    req.setRawHeader("Referer", "http://www.cnbeta.com/");

    webPage->setNetworkAccessManager(qmlEngine(this)->networkAccessManager());
    webPage->mainFrame()->load(req);
}

bool ArticleRetriever::loading() const
{
    return m_loading;
}

void ArticleRetriever::slotLoadStarted()
{
    m_loading = true;
    emit loadingChanged();
}

void ArticleRetriever::slotLoadFinished(bool ok)
{
    m_loading = false;
    emit loadingChanged();

    if (ok){
        QVariantMap result;

        QWebElement body = webPage->mainFrame()->findFirstElement("div.body");
        result.insert("title", body.findFirst("h2#news_title").toPlainText());  // 标题
        result.insert("date", body.findFirst("span.date").toPlainText());       // 日期
        result.insert("from", body.findFirst("span.where").toPlainText());      // 稿源

        QWebElement articleContent = body.findFirst("section.article_content");
        QWebElement introduction = articleContent.findFirst("div.introduction");
        result.insert("logo", introduction.findFirst("img").attribute("src"));  // logo
        result.insert("intro", introduction.findFirst("p").toPlainText());      // 简介

        QVariantList contentList;
        QVariantMap contentMap;
        QWebElementCollection contents = articleContent.findFirst("div.content").findAll("p");
        foreach (QWebElement content, contents) {
            contentMap.clear();
            QWebElement firstChild = content.firstChild();
            QString tagName = firstChild.tagName();
            if (tagName == "IMG"||tagName == "EMBED"){
                contentMap.insert("type", tagName);
                contentMap.insert("content", firstChild.attribute("src"));
            } else if (tagName == "A"){
                QWebElement ff = firstChild.firstChild();
                if (ff.tagName() == "IMG"){
                    contentMap.insert("type", "IMG");
                    contentMap.insert("content", ff.attribute("src"));
                }
            } else if (tagName == "OBJECT"){
                QWebElement embed = content.findFirst("embed");
                if (!embed.isNull()){
                    contentMap.insert("type", "EMBED");
                    contentMap.insert("content", embed.attribute("src"));
                }
            }
            if (contentMap.isEmpty()){
                contentMap.insert("type", "P");
                contentMap.insert("content", content.toPlainText());
            }
            // IMG图片, EMBED视频, P文字
            contentList.append(contentMap);
        }
        result.insert("content", contentList);  // 内容

        QRegExp regexp("GV\\\.DETAIL.*SN:\"([^\"]*)\"");
        regexp.setMinimal(true);
        if (webPage->mainFrame()->toHtml().indexOf(regexp) > 0){
            result.insert("SN", regexp.cap(1)); // 序列号
        }

        emit dataReceived(result);
    }
}

QUrl ArticleRetriever::articleUrl(const QString &sid) const
{
    return QUrl(QString(URL_ARTICLECONTENT).arg(sid));
}
