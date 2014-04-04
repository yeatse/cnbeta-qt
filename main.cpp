#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"
#include "newslistretriever.h"
#include "articleretriever.h"

#define DEBUGGING

#ifdef DEBUGGING
#include <QtNetwork/QNetworkProxy>
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<NewsListRetriever>("com.yeatse.cnbeta", 1, 0, "NewsList");
    qmlRegisterType<ArticleRetriever>("com.yeatse.cnbeta", 1, 0, "Article");

#ifdef DEBUGGING
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("192.168.1.64");
    proxy.setPort(8888);
    QNetworkProxy::setApplicationProxy(proxy);
#endif

    QmlApplicationViewer viewer;
    viewer.setMainQmlFile(QLatin1String("qml/cnBeta/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
