import QtQuick 1.1
import com.nokia.symbian 1.1
import com.yeatse.cnbeta 1.0
import "Component"
import "Silica"

MyPage {
    id: page;

    property int pageNumber: 1;
    property bool hasMore: false;

    function getlist(option){
        option = option||"renew";
        if (option === "renew"){
            pageNumber = 1;
            network.sendRequest(pageNumber);
        } else {
            network.sendRequest(pageNumber + 1);
        }
    }

    loading: network.loading;

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "toolbar-back";
            onClicked: pageStack.pop();
        }
    }

    TopicList {
        id: network;
        onDataReceived: {
            hasMore = data.hasMore;
            pageNumber = data.pageNumber;
            if (pageNumber <= 1){
                listModel.clear();
            }
            data.content.forEach(function(value){listModel.append(value)});
        }
    }

    ViewHeader {
        id: viewHeader;
        title: "新闻主题";
        onClicked: view.scrollToTop();
    }

    SilicaListView {
        id: view;

        property bool nextPageRequested: false;
        property int nextPageThreshold: 20;

        onAtYEndChanged: {
            if (atYEnd && hasMore && count > 0){
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

            Text {
                id: titleText;
                anchors {
                    left: root.paddingItem.left;
                    verticalCenter: parent.verticalCenter;
                }
                width: root.paddingItem.width - 120;
                font: constant.titleFont;
                wrapMode: Text.WrapAnywhere;
                color: constant.colorLight;
                text: name;
            }

            Image {
                id: logoImg;
                anchors {
                    right: root.paddingItem.right;
                    verticalCenter: parent.verticalCenter;
                }
                sourceSize.height: root.paddingItem.height;
                source: logo;
            }
        }
    }

    ScrollDecorator { flickableItem: view; }

    Component.onCompleted: getlist();
}
