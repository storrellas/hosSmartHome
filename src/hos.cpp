/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
#include "hos.h"

#include <QtGui>
#include <cmath>

#define SELECTION_NUMBER 8
#define FULL_CIRCLE 360



/********************************************************************
 Function:  HoS
 Overview:  Public constructor
********************************************************************/
HoS::HoS(QWidget *parent) : QWidget(parent)
{

    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    // Initialise the angle to 0
    this->currentAngle = 0;
}

/********************************************************************
 Function:  getHoSState
 Overview:  gets whether the HoS is enabled or disabled
********************************************************************/
bool HoS::getHoSState(){
    return timer->isActive();
}

/********************************************************************
 Function:  enableTurn
 Overview:  Starts the Hex-O-Select
********************************************************************/
bool HoS::enableTurn(){
    this->timer->start(200);
    return true;
}

/********************************************************************
 Function:  disableTurn
 Overview:  Stops the Hex-O-Select
********************************************************************/
bool HoS::disableTurn(){
    this->timer->stop();
    return true;
}

/********************************************************************
 Function:  getCurrentAngle
 Overview:  returns the current angle
********************************************************************/
int HoS::getCurrentAngle(){

    return this->currentAngle;
}

/********************************************************************
 Function:  getCurrentSelection
 Overview:  returns the current selected option
********************************************************************/
int HoS::getCurrentSelection(){

    int anglePerSelection = FULL_CIRCLE/SELECTION_NUMBER;
    int selection =(int) this->currentAngle/anglePerSelection;

    return selection;
}

/********************************************************************
 Function:  paintEvent
 Overview:  Paints forms in the widget
********************************************************************/
void HoS::paintEvent(QPaintEvent *)
{    
    // Iterator
    int i = 0;

    // Configure canvas
    int side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 250.0, side / 250.0);


//    // Draw Circle
//    // -----------
//    static const int radiusHOS = 80;

///*
//    QPoint square[4];
//    painter.save();
//    painter.setBrush(Qt::transparent);
//    painter.setPen(Qt::black);

//    square[0] = QPoint(0,0);
//    square[1] = QPoint(radiusHOS,0);
//    square[2] = QPoint(radiusHOS,radiusHOS);
//    square[3] = QPoint(0,radiusHOS);

//    painter.drawConvexPolygon(square, 4);

//    painter.restore();
///**/

//    int nPoints = 82;
//    QPoint circle[nPoints];
//    painter.save();
//    painter.setBrush(Qt::transparent);
//    painter.setPen(Qt::black);

//    circle[0] = QPoint(0,0);
//    for ( i = 1; i < nPoints-1; i++){
//        circle[i] = QPoint(i, sqrt( (radiusHOS*radiusHOS)-(i*i) ));
//        qDebug() << "i:" << i << " y:" << sqrt( (radiusHOS*radiusHOS)-(i*i) );
//    }

//    painter.drawConvexPolygon(circle, nPoints);

//    painter.restore();


//    return;

    // Arrow painting
    // --------------
    static const QColor arrowColor(0, 0, 0,197);
    painter.setPen(arrowColor);
    painter.setBrush(arrowColor);

    static const QPoint arrow[] = {
        QPoint(5, 8),
        QPoint(5, -40),
        QPoint(10, -40),
        QPoint(0, -50),
        QPoint(-10, -40),
        QPoint(-5, -40),
        QPoint(-5, 8)

    };

    painter.save();
    this->currentAngle+=1;
    if(this->currentAngle >= 360) this->currentAngle = 0;
    painter.rotate(this->currentAngle);
    painter.drawConvexPolygon(arrow, 7);
    painter.restore();

}
