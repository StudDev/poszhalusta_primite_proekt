import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtWebEngine 1.4
import QtQuick.Controls.Material 2.1

Page {
    anchors.fill: parent

    Material.accent: "#000"
    Material.primary: "#000"
    Material.background: "#FFDF48"
    Material.foreground: "#000"

    WebEngineView {
        id: webView
        anchors.fill: parent
        url: authUrl
        
        // onLoadingChanged: {
        //     controller.log(url);
        // }
    }

    footer: Label {
        Material.elevation: 6

        text: qsTr("Please grant access.")
        font.pixelSize: 22
        horizontalAlignment: Text.AlignHCenter
        padding: 10
    }
  
}
