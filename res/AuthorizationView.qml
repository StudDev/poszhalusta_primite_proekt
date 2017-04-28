import QtQuick 2.7
import QtQuick.Controls 2.0
import QtWebEngine 1.4

ApplicationWindow {

    width: 600;
    height: 320;
    visible:true;

    WebEngineView {
        anchors.fill: parent
        url: "http://iu3.bmstu.ru"
    }

    footer:
        Label {
            text: "Please grant access."
        }
}

 
