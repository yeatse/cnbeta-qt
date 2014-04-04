import QtQuick 1.1
import com.nokia.symbian 1.1
import com.yeatse.cnbeta 1.0
import "Component"
import "Silica"

MyPage {
    id: page;

    property string sid;
    onSidChanged: network.sendRequest(sid);

    loading: network.loading;

    tools: ToolBarLayout {
        ToolButton {
            iconSource: "toolbar-back";
            onClicked: pageStack.pop();
        }
    }

    Article {
        id: network;
        onDataReceived: {
            titleText.text = data.title;
            timeText.text = data.date;
            sourceText.text = data.from;
            logoImage.source = data.logo;
            introText.text = data.intro;
            var parse = function(value){
                if (value.type === "P"){
                    textComp.createObject(contentCol, { "text": value.content });
                } else if (value.type === "IMG"){
                    imageComp.createObject(contentCol, { "source": value.content });
                }
            }
            data.content.forEach(parse);
        }
    }

    ViewHeader {
        id: viewHeader;
        onClicked: view.scrollToTop();
    }

    SilicaFlickable {
        id: view;
        anchors { fill: parent; topMargin: viewHeader.height; }
        contentWidth: width;
        contentHeight: contentCol.height + constant.paddingLarge*2;
        visible: !loading;

        Column {
            id: contentCol;
            anchors {
                left: parent.left; top: parent.top; right: parent.right;
                margins: constant.paddingLarge;
            }
            spacing: constant.paddingXLarge;
            Text {
                id: titleText;
                width: parent.width;
                font {
                    family: platformStyle.fontFamilyRegular;
                    pixelSize: constant.fontXXLarge;
                }
                color: constant.colorLight;
                wrapMode: Text.Wrap;
            }
            Item {
                width: parent.width;
                height: introColumn.height + constant.paddingLarge*2;
                Rectangle {
                    id: introIndicator;
                    width: constant.paddingMedium;
                    height: parent.height;
                    color: "#007ACC";
                }
                Column {
                    id: introColumn;
                    anchors {
                        left: introIndicator.right;
                        leftMargin: constant.paddingLarge;
                        verticalCenter: parent.verticalCenter;
                    }
                    spacing: constant.paddingMedium;
                    Text {
                        id: timeText;
                        font: constant.labelFont;
                        color: constant.colorMid;
                    }
                    Text {
                        id: viewCountText;
                        font: constant.labelFont;
                        color: constant.colorMid;
                        text: " ";
                    }
                    Text {
                        id: sourceText;
                        font: constant.labelFont;
                        color: constant.colorMid;
                    }
                }
                Image {
                    id: logoImage;
                    anchors {
                        right: parent.right;
                        verticalCenter: parent.verticalCenter;
                    }
                }
            }
            Text {
                id: introText;
                width: parent.width;
                wrapMode: Text.Wrap;
                font: constant.labelFont;
                color: constant.colorLight;
            }
        }
    }

    Component {
        id: textComp;
        Text {
            width: contentCol.width;
            wrapMode: Text.Wrap;
            font: constant.labelFont;
            color: constant.colorLight;
        }
    }

    Component {
        id: imageComp;
        Item {
            property alias source: actualImage.source;
            width: contentCol.width;
            height: thumbnailImage.height;
            Image {
                id: actualImage;
                anchors.horizontalCenter: parent.horizontalCenter;
                sourceSize.width: parent.width;
                opacity: 0;
                NumberAnimation {
                    id: fadeInAnimation;
                    target: actualImage;
                    property: "opacity";
                    to: 1;
                    duration: 500;
                }
                onStatusChanged: {
                    if (status == Image.Ready){
                        thumbnailImage.visible = false;
                        parent.height = height;
                        fadeInAnimation.start();
                    }
                }
            }
            Image {
                id: thumbnailImage;
                anchors.horizontalCenter: parent.horizontalCenter;
                source: "gfx/placeholder.png";
            }
        }
    }

    ScrollDecorator { flickableItem: view; platformInverted: true; }
}
