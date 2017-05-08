import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.2

Page {
    id: page
    anchors.fill: parent
    property int margin: 11

    header: TabBar {
        id: bar
        width: parent.width

        Material.accent: "#000"
        Material.primary: "#000"
        Material.background: "#FFDF48"
        Material.foreground: "#000"
        Material.elevation: 2

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
            anchors.fill: parent

            ColumnLayout {
                anchors.margins: margin

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right

                GroupBox {
                    anchors.margins: margin
                    Layout.fillWidth: true

                    title: "Test"

                    RowLayout {
                        anchors.fill: parent
                        
                        TextField {
                            id: text;

                            Layout.fillWidth: true
                        }

                        Button {
                            //anchors.left: text.right
                            //anchors.right: parent.right
                            text: "Ok"
                            onClicked: {
                                controller.log(text.text);
                            }
                        }
                    }

                }
            }
        }

        Item {
            id: filesTab
            //anchors.fill: parent

            ColumnLayout {
                anchors.margins: margin

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right

                GroupBox {
                    anchors.margins: margin
                    Layout.fillWidth: true

                    title: qsTr("Sync root")


                    RowLayout {
                        anchors.fill: parent
                        
                        FileDialog {
                            id: fileDialog

                            folder: shortcuts.home
                            selectFolder: true

                            onAccepted: {
                                path.text = decodeURIComponent(
                                    fileDialog.fileUrl
                                              .toString()
                                              .replace(/^(file:\/{2})/,"")
                                );

                                controller.changeRoot(fileDialog.fileUrl)
                            }
                        }

                        TextField {
                            Layout.fillWidth: true
                            id: path;

                            text: controller.getRootPath()

                            readOnly: true
                            activeFocusOnPress: false
                        }

                        Button {
                            text: qsTr("Select sync folder")
                            onClicked: fileDialog.open()
                        }
                    }
                } 
            }    
        }

        Item {
            id: otherTab
        }

    }
}

 
