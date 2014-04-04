import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: root;

    property bool loading;

    orientationLock: PageOrientation.LockPortrait;

    BusyIndicator {
        id: loadingIndicator;
        visible: root.loading;
        platformInverted: true;
        width: constant.graphicSizeLarge;
        height: constant.graphicSizeLarge;
        anchors.centerIn: parent;
        running: true;
        z: 10;
    }
}
