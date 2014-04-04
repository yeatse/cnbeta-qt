import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1

PageStackWindow {
    id: app;

    platformInverted: true;
    showStatusBar: true;
    showToolBar: true;

    initialPage: MainPage { id: mainPage; }

    Binding {
        target: app.pageStack.toolBar;
        property: "platformInverted";
        value: false;
    }

    InfoBanner { id: infoBanner; platformInverted: true; }

    Constant { id: constant; }

    SignalCenter { id: signalCenter; }
}
