import QtQuick 2.0

Rectangle {
    width: 1000
    height: 1000

    color : "blue"

/*
        Rectangle {
                height: parent.height*0.9
                width : parent.width*0.9
                radius: 10
                color: "yellow"
                anchors.centerIn : parent
                border.width : 3
                border.color : "black"

                HosArrow{
                    anchors.centerIn : parent
                    height: parent.height; width : parent.width
                    color: "transparent"
                }


                Rectangle{
                    id: groupBoxTitle

                    x: 10; y : -label.height/2
                    color : "yellow"
                    //border.width : 3
                    width: label.width +5
                    height: label.height + 10

                    Text{
                        id: label
                        text: "Hex-O-Select"
                        font.family: "Helvetica"
                        font.pointSize: 15
                        anchors.centerIn : parent
                    }
                }


        }
/**/
    Rectangle{
        id: container
        color: "grey"
        anchors.centerIn :parent

        height: parent.height*0.9
        width : parent.width*0.9


        Column {
            spacing: 20

            Rectangle {
                    property string backgroundColor : "red"
                    height: container.height*0.6
                    width : container.width

                    color: backgroundColor
                    radius: 10
                    border.width : 3; border.color : "black"

                    Rectangle{
                        id: groupBoxTitleHoS

                        x: 10; y : -label.height/2
                        color : parent.backgroundColor
                        //border.width : 3
                        width: label.width +5
                        height: label.height + 10

                        Text{
                            id: label
                            text: "Hex-O-Select"
                            font.family: "Helvetica"
                            font.pointSize: 15
                            anchors.centerIn : parent
                        }
                    }

                    HosArrow{
                        //anchors.centerIn : parent
                        height: parent.height; width : parent.width
                        color: "transparent"
                    }
                    /**/
            }


            Rectangle {
                property string backgroundColor : "purple"

                color: backgroundColor;
                radius: 10
                border.width : 3; border.color : "black"

                height: container.height*0.3
                width : container.width

                Rectangle{
                    id: groupBoxTitleConsole

                    x: 10; y : -labelConsole.height/2
                    color : parent.backgroundColor
                    //border.width : 3
                    width: labelConsole.width +5
                    height: labelConsole.height + 10

                    Text{
                        id: labelConsole
                        text: "Console"
                        font.family: "Helvetica"
                        font.pointSize: 15
                        anchors.centerIn : parent
                    }
                }

                Rectangle{
                    color : "white"
                    anchors. centerIn : parent
                    width : parent.width*0.95
                    height : parent.height*0.78


                    TextEdit {
                        height : parent.height
                        width: parent.width
                        text: "This is just a stupid post"
                        font.family: "Helvetica"
                        font.pointSize: 10
                        color: "black"
                        focus: true
                        anchors.centerIn : parent
                    }

                }


            }
/*
            Rectangle { color: "green"; width: container.width; height: 50 }
            Rectangle { color: "black"; width: container.width; height: 20 }
/**/
        }
    }

}
