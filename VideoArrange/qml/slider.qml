/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
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
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

Rectangle {
    color: "lightGray"
    property alias rectangle1: rectangle1


    Text {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 15
        text: "QQuickFramebufferObject with animated clear color"
        color: "white"
    }

    Rectangle {
        id: rectangle
        color: "#c7fdff"
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 30
        anchors.topMargin: 10
        anchors.fill: parent

        MouseArea {
            id: mouseArea
            anchors.fill: parent
			onWheel: {
		      if (wheel.angleDelta.y > 0)
                {
					parent.anchors.leftMargin += 5;
//		          adjustZoom(wheel.angleDelta.y / 120);
              }
              else
              {
                  parent.anchors.leftMargin -= 5;
              }
            }
        }

        Rectangle {
            id: rectangle1
            x: 34
            y: 123
            width: 104
            height: 110
            color: "#ffffff"
            radius: 55
            border.width: 2
            smooth: false
            enabled: false
            rotation: 0
            scale: 1
            transformOrigin: Item.Center
        }
    }
}
