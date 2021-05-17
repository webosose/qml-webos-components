// Copyright (c) 2013-2021 LG Electronics, Inc.
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
import PmTrace 1.0

Rectangle {
    width: 1920; height: 1080;
    color: "black"

    PmTrace {
        id: trace
    }

    Rectangle {
        id: redRectangle

        x: 0;
        y: 0;
        width: (parent.width / 2);
        height: (parent.height / 2);

        color: "red"

        Text {
            text: "start"
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: (mouse) => {
                redRectangle.color = "green";
                blueRectangle.color = "blue";
                trace.traceLogEventTypeContextData("EventType1", "RedRectangle");
                trace.traceLogEventTypeContextData("EventType2", "RedRectangle");
                trace.traceLogEventTypeContextData("EventType3", "RedRectangle");
                trace.traceLog("QmlTraceRedRectangleTest");
                trace.traceLogWithPosition("QmlTraceRedRectangleTest", mouse.x, mouse.y);
                trace.traceBefore("QmlTraceTest");
            }
        }
    }
    Rectangle {
        id: blueRectangle

        x: (parent.width / 2);
        y: (parent.height / 2);
        width: (parent.width / 2);
        height: (parent.height / 2);

        color: "blue"

        Text {
            text: "print"
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: (mouse) => {
                redRectangle.color = "red";
                blueRectangle.color = "green";
                trace.traceLogEventTypeContextData("EventType1", "BlueRectangle");
                trace.traceLogEventTypeContextData("EventType2", "BlueRectangle");
                trace.traceLogEventTypeContextData("EventType3", "BlueRectangle");
                trace.traceLog("QmlTraceBlueRectangleTest");
                trace.traceLogWithPosition("QmlTraceBlueRectangleTest", mouse.x, mouse.y);
                trace.traceAfter("QmlTraceTest");
            }
        }
    }
}
