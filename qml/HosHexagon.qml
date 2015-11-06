//import QtQuick 2.0

import QtQuick 1.0

Image {
    property double sideHexagon : 10;
    property double imageWidth: sideHexagon*2
    property double imageHeight: sideHexagon/Math.tan(Math.PI/6)
    property int xCentered : imageWidth/2
    property int yCentered : imageHeight/2

    property string hexText : "NO_SET"

    //------------------------
    // Name : getAphotemeHexagon
    // Overview : gets the aphoteme from the hexagon
    // -----------------------
    function getAphotemeHexagon(side){
        return side/(2*Math.tan(Math.PI/6));
    }

    //------------------------
    // Name : getXHexagon
    // Overview : gets the x value in a circle centered in the screen
    // -----------------------
    function getXHexagon(angle){
        var radius = 2*getAphotemeHexagon(sideHexagon)

        return xCentered+radius*Math.cos(angle)
    }

    //------------------------
    // Name : getXHexagon
    // Overview : gets the y value in a circle centered in the screen
    // -----------------------
    function getYHexagon(angle){
        var radius = 2*getAphotemeHexagon(sideHexagon)

        return yCentered+radius*Math.sin(angle)
    }


    //x : (imageWidth/2)
    //y : (imageHeight/2) - 2*getAphotemeHexagon(sideHexa)

    width: imageWidth;
    height: imageHeight;

    //source: "file:./hexagon.png"
    source: "qrc:/images/hexagon"
    smooth: true

    Component.onCompleted : onCompletedHandler()
    function onCompletedHandler(){
        //console.log("HosHexagon.qml -> xCentered: " + xCentered + " yCentered: " + yCentered)
        console.log("HosHexagon.qml -> x: " + x + " y: " + y)
    }


    // Text to Hexagon
    Text {
        text: parent.hexText
        font.family: "Helvetica"
        font.pointSize: 15
        color: "black"
        anchors.centerIn: parent
    }

}

