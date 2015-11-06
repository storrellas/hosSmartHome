import QtQuick 1.0



Rectangle {
    id: mainScreen

    // signal definition
    signal getAngleFrameQMLSignal(string angleFrame);
    signal handleUpdateAngleQMLSignal(string updateAngle);

    property int centerScreenX : width/2;
    property int centerScreenY : height/2;
    property variant hexagonList : [hexagon0, hexagon1, hexagon2, hexagon3, hexagon4, hexagon5]

    width: 1000; height: 1000
    //color: "#800000FF"
    color: "transparent"

    property bool declarative : false


    // ---------------
    // WiiMoteStatus
    // ---------------
    Rectangle{
        id: wiiMoteStatus

        property int padding    : mainScreen.height/40;
        property string text    : "Wii Status";
        property real fontSize  : 20
        property string enabledColor : "#80FF00"
        property string disabledColor : "#FA5858"
        property string statusColor: disabledColor;

        color : statusColor;
        width : mainScreen.width /7;
        height: mainScreen.height/10;

        radius: 10
        border.width : 3; border.color : "#2E2E2E"

        y: mainScreen.height- height - padding;
        x: mainScreen.width - width - padding;

        Text {
            text: parent.text
            font.family: "Helvetica"
            font.pointSize: parent.fontSize
            color: "black"
            anchors.centerIn: parent
        }
    }



    //Component.onCompleted : updateHexagonSet();

    // ---------------
    // HosHand Section
    // ---------------
    Timer {
        interval: 1000; running: true; repeat: true;
        onTriggered: mainScreen.timerEllapsed()
    }

    property int currentAngle   : 0
    property int angleFrame     : 30
    property int currentHexagon : 0

    //------------------------
    // Name     : timerEllapsed
    // Overview : handles the timer ellapsed function
    // -----------------------
    function timerEllapsed() {

        // Increment currentAngle with AngleFrame
        currentAngle += angleFrame

        // Updates the Hexagon
        updateHexagonSet();

    }

    //------------------------
    // Name     : setAngleFrameQMLSlot
    // Overview : slot for Qt communication
    // -----------------------
    function setAngleFrameQMLSlot(_angleFrame){
        /*
        angleFrame = _angleFrame;
        console.log("From Change Angle Frame: ", angleFrame);
        getAngleFrameQMLSignal(angleFrame);
        /**/
    }

    //------------------------
    // Name     : setEnabledWiiMoteSlot
    // Overview : Enables or disabled wii status
    // -----------------------
    function setWiiMoteStatusQMLSlot(status){
        console.log("from setEnabledWiiMoteSlot " + status);

        if(status == "enabled"){
            wiiMoteStatus.statusColor=wiiMoteStatus.enabledColor;
        }else{
            wiiMoteStatus.statusColor=wiiMoteStatus.disabledColor;
        }
    }

    //------------------------
    // Name     : updateHexagonSet
    // Overview : updates the Hexagons according to angleFrame
    // -----------------------
    function updateHexagonSet(){

        for( var i = 0; i<6; i++){
            hexagonList[i].colorText="black"
            hexagonList[i].hexSource=mainScreen.declarative?"hexagon.png":"qrc:/images/hexagon"
            hexagonList[i].hexFontSize = hexagonList[i].width/10;

        }

        currentAngle = currentAngle%360;
        // Select the specific hexagon
        currentHexagon = currentAngle*6/360; // Assume 6 hexagons presents
        currentHexagon = currentHexagon%6;
        console.log("currentHexagon:" + currentHexagon + " currentAngle: " + currentAngle)

        hexagonList[currentHexagon].colorText="white"
        hexagonList[currentHexagon].hexSource=mainScreen.declarative?"hexagonWhite.png":"qrc:/images/hexagonWhite"


        // Signal to Qt Handler
        handleUpdateAngleQMLSignal(currentHexagon);

    }

// Comments
Item{
//    MouseArea {
//        anchors.fill: parent
//        onClicked: clickedHandler()
//    }
//    function clickedHandler(){
//        console.log("Clicked Hexagon");
//        hexagon0.source="qrc:/images/hexagonWhite"
//
//        var component = Qt.createComponent("qrc:/files/HosInner");
//        if (component.status == Component.Ready){
//            component.createObject(mainScreen, {"x": 20, "y": 20, "backColor" : "red"});
//        }
//
//
//        if(component.status == Component.Null)
//            console.log("error loading1");
//
//        if(component.status == Component.Error)
//            console.log("error loading ->" + component.errorString());
//
//        if(component.status == Component.Loading)
//            console.log("still loading");
//
//    }
}

    // ---------------
    // HosHand Section
    // ---------------
    Image {

        id: hosHand

        property int imageWidth: parent.width/100
        property int imageHeight: parent.height/10

        //property double imageWidth: 15
        //property double imageHeight: 78

        x : mainScreen.centerScreenX-(imageWidth/2)
        y : mainScreen.centerScreenY-imageHeight
        width  : imageWidth;
        height : imageHeight;

        //source: "qrc:/images/arrowBlack"
        //source: "arrowBlack.png"
        smooth: true
        transformOrigin: Item.Center

        Component.onCompleted : source=mainScreen.declarative?"arrowBlack.png":"qrc:/images/arrowBlack"

        transform: Rotation {
            id: handRotation
            origin.x: hosHand.width/2; origin.y: hosHand.height;
            angle: mainScreen.currentAngle
            Behavior on angle {
                SpringAnimation { spring: 2; damping: 0.17; modulus: 360 }
            }
        }
    }





    // ---------------
    // Hexagon Section
    // ---------------

    // Properties
    property double sideHexagon : mainScreen.width/17;
    property double imageWidth  : sideHexagon*2
    property double imageHeight : sideHexagon/Math.tan(Math.PI/6)
    property double xCentered   : centerScreenX - (imageWidth/2)
    property double yCentered   : centerScreenY - (imageHeight/2)
    //

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
    function getXHexagon(angle, sideHexagon){
        var radius = 2*getAphotemeHexagon(sideHexagon)

        return xCentered+radius*Math.cos(angle)
    }

    //------------------------
    // Name : getXHexagon
    // Overview : gets the y value in a circle centered in the screen
    // -----------------------
    function getYHexagon(angle, sideHexagon){
        var radius = 2*getAphotemeHexagon(sideHexagon)
        return yCentered+radius*Math.sin(angle)
    }


    Image {
        id: hexagon0
        property string hexText   : "Up"
        property string colorText : "black"
        property string hexSource : ""
//      property string hexSource : "qrc:/images/hexagon"
//      property string hexSource : "./hexagon.png"
        property real hexFontSize : 15

        x : getXHexagon(3*Math.PI/2 + 0*2*Math.PI/6, parent.sideHexagon)
        y : getYHexagon(3*Math.PI/2 + 0*2*Math.PI/6, parent.sideHexagon)

        width: imageWidth;
        height: imageHeight;
        //cache : false

        source: hexSource
        smooth: true

        Component.onCompleted : hexSource=mainScreen.declarative?"./hexagon.png":"qrc:/images/hexagon"


        Text {
            text: parent.hexText
            font.family: "Helvetica"
            font.pointSize: parent.hexFontSize
            color: parent.colorText
            anchors.centerIn: parent
        }

    }


    Image {
        id: hexagon1
        property string hexText : "Left"
        property string colorText : "black"
        property string hexSource : ""        
//        property string hexSource : "qrc:/images/hexagon"
//        property string hexSource : "./hexagon.png"
        property real hexFontSize : 15


        x : getXHexagon(3*Math.PI/2 + 1*2*Math.PI/6, parent.sideHexagon)
        y : getYHexagon(3*Math.PI/2 + 1*2*Math.PI/6, parent.sideHexagon)

        width: imageWidth;
        height: imageHeight;
        source: hexSource
        Component.onCompleted : hexSource=mainScreen.declarative?"./hexagon.png":"qrc:/images/hexagon"


        smooth: true

        Text {
            text: parent.hexText
            font.family: "Helvetica"
            font.pointSize: parent.hexFontSize
            color: parent.colorText
            anchors.centerIn: parent
        }

    }


    Image {
        id: hexagon2
        property string hexText : "Connect"
        property string colorText : "black"
        property string hexSource : ""
//        property string hexSource : "qrc:/images/hexagon"
//        property string hexSource : "./hexagon.png"
        property real hexFontSize : 15


        x : getXHexagon(3*Math.PI/2 + 2*2*Math.PI/6, parent.sideHexagon)
        y : getYHexagon(3*Math.PI/2 + 2*2*Math.PI/6, parent.sideHexagon)

        width: imageWidth;
        height: imageHeight;
        source: hexSource
        smooth: true
        Component.onCompleted : hexSource=mainScreen.declarative?"./hexagon.png":"qrc:/images/hexagon"


        Text {
            text: parent.hexText
            font.family: "Helvetica"
            font.pointSize: parent.hexFontSize
            color: parent.colorText
            anchors.centerIn: parent
        }

    }


    Image {
        id: hexagon3
        property string hexText   : "Down"
        property string colorText : "black"
        property string hexSource : ""
//        property string hexSource : "qrc:/images/hexagon"
//        property string hexSource : "./hexagon.png"
        property real hexFontSize : 15

        x : getXHexagon(3*Math.PI/2 + 3*2*Math.PI/6, parent.sideHexagon)
        y : getYHexagon(3*Math.PI/2 + 3*2*Math.PI/6, parent.sideHexagon)

        width: imageWidth;
        height: imageHeight;
        source: hexSource
        smooth: true
        Component.onCompleted : hexSource=mainScreen.declarative?"./hexagon.png":"qrc:/images/hexagon"


        Text {
            text: parent.hexText
            font.family: "Helvetica"
            font.pointSize: parent.hexFontSize
            color: parent.colorText
            anchors.centerIn: parent
        }

    }


    Image {
        id: hexagon4
        property string hexText : " "
        property string colorText : "black"
        property string hexSource : ""
//        property string hexSource : "qrc:/images/hexagon"
//        property string hexSource : "./hexagon.png"
        property real hexFontSize : 15

        x : getXHexagon(3*Math.PI/2 + 4*2*Math.PI/6, parent.sideHexagon)
        y : getYHexagon(3*Math.PI/2 + 4*2*Math.PI/6, parent.sideHexagon)

        width: imageWidth;
        height: imageHeight;
        source: hexSource
        smooth: true
        Component.onCompleted : hexSource=mainScreen.declarative?"./hexagon.png":"qrc:/images/hexagon"


        Text {
            text: parent.hexText
            font.family: "Helvetica"
            font.pointSize: parent.hexFontSize
            color: parent.colorText
            anchors.centerIn: parent
        }

    }

    Image {
        id: hexagon5
        property string hexText : "Right"
        property string colorText : "black"
        property string hexSource : ""
//        property string hexSource : "qrc:/images/hexagon"
//        property string hexSource : "./hexagon.png"
        property real hexFontSize : 15

        x : getXHexagon(3*Math.PI/2 + 5*2*Math.PI/6, parent.sideHexagon)
        y : getYHexagon(3*Math.PI/2 + 5*2*Math.PI/6, parent.sideHexagon)

        width: imageWidth;
        height: imageHeight;
        source: hexSource
        smooth: true
        Component.onCompleted : hexSource=mainScreen.declarative?"./hexagon.png":"qrc:/images/hexagon"

        Text {
            text: parent.hexText
            font.family: "Helvetica"
            font.pointSize: parent.hexFontSize
            color: parent.colorText
            anchors.centerIn: parent
        }

    }
/**/




}
