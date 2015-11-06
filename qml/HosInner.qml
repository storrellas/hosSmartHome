import QtQuick 1.0

Rectangle {
    id: test
    width: 10
    height: 10

    property string backColor : "green"
    color: "yellow";


    Rectangle{
        width : 40
        height : 40

        color: test.backColor;
        anchors.centerIn: parent

    }
/**/
}
