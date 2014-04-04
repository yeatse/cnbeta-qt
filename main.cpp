#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"
#include "newslistretriever.h"
#include "articleretriever.h"
#include "commentretriever.h"
#include "topicretriever.h"
#include "utility.h"

//#define DEBUGGING

#ifdef DEBUGGING
#include <QtNetwork/QNetworkProxy>
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

#ifdef Q_OS_SYMBIAN
    QSplashScreen *splash = new QSplashScreen(QPixmap("qml/cnBeta/gfx/splash.png"));
    splash->show();
    splash->raise();
#endif

    app->setApplicationName("cnBeta");
    app->setOrganizationName("Yeatse");
    app->setApplicationVersion(VER);

    qmlRegisterType<NewsListRetriever>("com.yeatse.cnbeta", 1, 0, "NewsList");
    qmlRegisterType<ArticleRetriever>("com.yeatse.cnbeta", 1, 0, "Article");
    qmlRegisterType<CommentRetriever>("com.yeatse.cnbeta", 1, 0, "Comment");
    qmlRegisterType<TopicRetriever>("com.yeatse.cnbeta", 1, 0, "TopicList");

#ifdef DEBUGGING
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("192.168.1.64");
    proxy.setPort(8888);
    QNetworkProxy::setApplicationProxy(proxy);
#endif

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("utility", Utility::Instance());
    viewer.setMainQmlFile(QLatin1String("qml/cnBeta/main.qml"));
    viewer.showExpanded();

#ifdef Q_OS_SYMBIAN
    splash->finish(&viewer);
    splash->deleteLater();
#endif

    return app->exec();
}
