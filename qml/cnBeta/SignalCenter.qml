import QtQuick 1.1

QtObject {
    id: signalCenter;

    function showMessage(msg){
        if (msg||false){
            infoBanner.text = msg;
            infoBanner.open();
        }
    }
}
