import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Page {
    anchors.fill: parent
    property int margin: 11

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
        anchors.fill: parent
        currentIndex: bar.currentIndex

        Item {
            id: generalTab

            ColumnLayout {
                anchors.margins: margin

                Rectangle {
                    anchors.margins: margin
                    color: "#777777"

                    TextField {
                        id: text;
                    }

                    Button {
                        anchors.left: text.right
                        text: "Ok"
                        onClicked: {
                            controller.log(text.text);
                        }
                    }
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

 
