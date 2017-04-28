import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {

    width: 600;
    height: 320;
    visible:true;

    header: TabBar {
        id: bar
        width: parent.width

        TabButton {
            text: qsTr("General")
        }
        TabButton {
            text: qsTr("Files")
        }
        TabButton {
            text: qsTr("Other")
        }
    }


    StackLayout {
        width: parent.width
        currentIndex: bar.currentIndex

        Item {
            id: generalTab

            TextField {
                id: text;
            }

            Button {
                text: "Ok"
                onClicked: {
                    controller.log(text.text);
                }
            }
        }
        Item {
            id: filesTab
        }
        Item {
            id: otherTab
        }
    }
}

 
