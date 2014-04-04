import QtQuick 1.1
import com.nokia.symbian 1.1

QtObject {
    id: constant;

    // color
    property color colorLight: platformStyle.colorNormalLightInverted;
    property color colorMid: platformStyle.colorNormalMidInverted;
    property color colorMarginLine: platformStyle.colorDisabledLightInverted;
    property color colorTextSelection: platformStyle.colorTextSelectionInverted;
    property color colorDisabled: platformStyle.colorDisabledMidInverted;

    // padding size
    property int paddingSmall: platformStyle.paddingSmall
    property int paddingMedium: platformStyle.paddingMedium
    property int paddingLarge: platformStyle.paddingLarge
    property int paddingXLarge: platformStyle.paddingLarge + platformStyle.paddingSmall

    // graphic size
    property int graphicSizeTiny: platformStyle.graphicSizeTiny
    property int graphicSizeSmall: platformStyle.graphicSizeSmall
    property int graphicSizeMedium: platformStyle.graphicSizeMedium
    property int graphicSizeLarge: platformStyle.graphicSizeLarge
    property int thumbnailSize: platformStyle.graphicSizeLarge * 1.5

    // font size
    property int fontXSmall: platformStyle.fontSizeSmall - 2
    property int fontSmall: platformStyle.fontSizeSmall
    property int fontMedium: platformStyle.fontSizeMedium
    property int fontLarge: platformStyle.fontSizeLarge
    property int fontXLarge: platformStyle.fontSizeLarge + 2
    property int fontXXLarge: platformStyle.fontSizeLarge + 4

    property variant subTitleFont: __subTitleText.font;
    property variant labelFont: __label.font;
    property variant titleFont: __titleText.font;

    // size
    property variant sizeTiny: Qt.size(graphicSizeTiny, graphicSizeTiny);
    property variant sizeMedium: Qt.size(graphicSizeMedium, graphicSizeMedium);

    // others
    property int headerHeight: privateStyle.tabBarHeightPortrait;

    // private
    property ListItemText __titleText: ListItemText {}
    property ListItemText __subTitleText: ListItemText { role: "SubTitle"; }
    property Label __label: Label {}
}
