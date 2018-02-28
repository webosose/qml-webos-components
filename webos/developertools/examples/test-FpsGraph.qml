// Copyright (c) 2014-2018 LG Electronics, Inc.
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
import WebOS.DeveloperTools 1.0

Item {
    width:  1920
    height:  1080
    Rectangle {
        height: 16
        color: "green"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
    Rectangle {
        height: 1
        color: "yellow"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin:  100
        Text { text: "100ms"; color: "yellow" }
    }
    Rectangle {
        height: 1
        color: "red"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin:  500
        Text { text: "500ms"; color: "red" }
    }
    FpsGraph {
        anchors.fill: parent
        z: 1000
    }
    Rectangle {
        width: 10
        height: 10
        RotationAnimation on rotation {
            from: 0; to: 360;
            duration: 10000
            loops: Animation.Infinite
        }
    }
}
