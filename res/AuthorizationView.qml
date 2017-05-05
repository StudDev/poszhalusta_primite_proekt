import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtWebEngine 1.4

Page {
    anchors.fill: parent

    WebEngineView {
        id: webView
        anchors.fill: parent
        url: authUrl
        
        // onLoadingChanged: {
        //     controller.log(url);
        // }
    }

    footer: Label {
        text: "Please grant access."
        font.pixelSize: 22
        horizontalAlignment: Text.AlignHCenter
        padding: 10
    }
  
}
