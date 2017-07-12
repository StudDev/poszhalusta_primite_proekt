import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import Qt.labs.platform 1.0

Item {
    SystemTrayIcon {
        visible: true
        iconSource: "qrc:/icon.png"

        menu: Menu {
            MenuItem {
                text: qsTr("Synced ") + "78%"
            }

            MenuItem {
                text: qsTr("Preferences")
                onTriggered: {
                    preferencesView.show()
                    preferencesView.raise()
                    preferencesView.requestActivate()
                }
            }

            MenuItem {
                text: qsTr("Quit")
                onTriggered: app.quit()
            }
        }
    }
    
}


