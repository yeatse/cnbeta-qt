import QtQuick 1.1
import com.nokia.symbian 1.1
import com.yeatse.cnbeta 1.0
import "Component"
import "Silica"

MyPage {
    id: page;

    property int pageNumber: 0;

    function getlist(option){
        option = option||"renew";
        if (option === "renew"){
            pageNumber = 0;
            network.sendRequest("realtime", 0);
        } else {
            network.sendRequest("all", pageNumber + 1);
        }
    }

    loading: network.loading;

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "toolbar-back";
            onClicked: {
                if (quitTimer.running){
                    Qt.quit();
                } else {
                    quitTimer.start();
                    signalCenter.showMessage("再按一次退出");
                }
            }
            Timer {
                id: quitTimer;
                interval: infoBanner.timeout;
            }
        }
        ToolButton {
            iconSource: "gfx/applications.svg";
            onClicked: pageStack.push(Qt.resolvedUrl("TopicListPage.qml"))
        }
        ToolButton {
            iconSource: "toolbar-menu";
        }
    }

    NewsList {
        id: network;
        onDataReceived: {
            if (type == "realtime"){
                listModel.clear();
            }
            var parse = function(value){
                var prop;
                if (type == "realtime"){
                    prop = {
                        "sid": value.sid,
                        "time_show": value.time_show,
                        "title": value.title,
                        "logo": value.logo
                    }
                } else {
                    prop = {
                        "sid": value.sid,
                        "time_show": value.time,
                        "title": value.title_show,
                        "logo": value.logo
                    }
                }
                listModel.append(prop);
            }
            list.forEach(parse);
            page.pageNumber = pageNumber;
        }
    }

    ViewHeader {
        id: viewHeader;
        title: "全部分类";
        onClicked: view.scrollToTop();
    }

    SilicaListView {
        id: view;

        property bool nextPageRequested: false;
        property int nextPageThreshold: 20;

        onAtYEndChanged: {
            if (atYEnd && count > 0){
                nextPageRequested = true;
            }
        }
        onMovementEnded: {
            if (!loading && nextPageRequested && contentHeight - contentY - height < nextPageThreshold){
                getlist("next");
            }
            nextPageRequested = false;
        }

        anchors { fill: parent; topMargin: viewHeader.height; }
        cacheBuffer: screen.height;
        pressDelay: 150;
        model: ListModel { id: listModel; }
        header: PullToActivate {
            myView: view;
            enabled: !loading;
            onRefresh: getlist();
        }
        delegate: AbstractDelegate {
            id: root;

            implicitHeight: 88 + (constant.paddingSmall+constant.paddingLarge)*2;
            onClicked: {
                pageStack.push(Qt.resolvedUrl("ArticlePage.qml"),{sid:sid});
            }

            Image {
                id: logoImg;
                anchors {
                    right: root.paddingItem.right;
                    top: root.paddingItem.top;
                }
                sourceSize.width: 120;
                width: 120;
                height: 88;
                source: logo;
                fillMode: Image.PreserveAspectFit;
            }

            Text {
                id: titleText;
                anchors {
                    left: root.paddingItem.left;
                    top: root.paddingItem.top;
                    right: logoImg.left;
                    rightMargin: constant.paddingMedium;
                }
                font: constant.titleFont;
                wrapMode: Text.WrapAnywhere;
                color: constant.colorLight;
                maximumLineCount: 2;
                elide: Text.ElideRight;
                textFormat: Text.PlainText;
                text: title;
            }

            Text {
                id: timeShow;
                anchors {
                    left: root.paddingItem.left;
                    bottom: root.paddingItem.bottom;
                }
                font: constant.subTitleFont;
                color: constant.colorMid;
                text: time_show;
            }
        }
    }

    ScrollDecorator { flickableItem: view; platformInverted: true; }

    Component.onCompleted: getlist();
}
