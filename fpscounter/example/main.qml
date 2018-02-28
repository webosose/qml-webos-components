// Copyright (c) 2013-2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

import QtQuick 2.0
import FpsCounter 1.0

Rectangle {
    width: 200; height: 100; color: "black"
    FpsCounter {
        id: fpsCount
        anchors.top: parent.top
        anchors.topMargin: 25
        anchors.right: parent.right
        anchors.rightMargin: 25
        width: 100
        height: 30
        z: 10000
        Rectangle {
            anchors.fill: parent
            color: "Black"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 20
                font.bold: true
                color: "white"
                text: "FPS: " + fpsCount.fps
            }
        }
    }
}
