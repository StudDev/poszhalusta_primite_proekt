import QtQuick 2.7
import QtQuick.Controls 2.0
import QtWebEngine 1.4

ApplicationWindow {

    width: 640
    height: 720
    visible: true
    color: "#f6f6f6"

    WebEngineView {
        anchors.fill: parent
        url: "https://oauth.yandex.ru/authorize?response_type=code&client_id=20beb8f54f66490fa4f21b42f7af7145"
    }

    footer: Label {
        text: "Please grant access."
        font.pixelSize: 22
        horizontalAlignment: Text.AlignHCenter
        padding: 10
    }
  
}

 
