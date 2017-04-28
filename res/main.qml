import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle {

    width: 600;
    height: 320;

    TextField {
        id: text;
    }

    Button {
        text: "Ok"
        onClicked: {
            mainView.log(text.text);
        }
        anchors.left: text.right;
    } 
}

 
