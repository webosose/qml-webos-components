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
    id: root

    property var debugFilter
    property var warningFilter
    property var criticalFilter
    property var fatalFilter

    property int scrollback: 128

    Rectangle {
        anchors.fill: parent
        visible: debugConsoleView.visible
        color: "black"
        opacity: 0.65
    }

    ListView {
        id: debugConsoleView
        anchors { fill: parent; margins: 16 }
        interactive: false

        delegate: Item {
            width: debugConsoleView.width
            height: col.height
            Column {
                id: col
                spacing: -4
                Text {
                    text: model.text || ""
                    color: model.color || ""
                    font.pixelSize: 15
                }
                Text {
                    text: model.details ? [model.file, model.line].join(':') : ""
                    color: "gray"
                    font.pixelSize: 15
                }
            }
        }

        model: ListModel {
            id: debugConsole
            onRowsInserted: cleanupTimer.restart()
            function appendMessage(filter, message, color, details) {
                // The try-catch block needed here to avoid any runtime errors to be logged
                // while tryingg to call `filter()`. Otherwise it will create an infinite
                // loop of log messages.
                try {
                    message.color = color
                    message.details = details
                    if (filter === undefined || filter === true || filter(message))
                        append(message)
                } catch (e) {}
            }
        }

        Timer {
            id: cleanupTimer
            interval: 0
            onTriggered: {
                while (debugConsole.count > scrollback)
                    debugConsole.remove(0)
                debugConsoleView.positionViewAtEnd()
            }
        }

        DebugMessageFilter {
            onDebug: debugConsole.appendMessage(debugFilter, message, "gray")
            onWarning: debugConsole.appendMessage(warningFilter, message, "white")
            onCritical: debugConsole.appendMessage(criticalFilter, message, "orange")
            onFatal: debugConsole.appendMessage(fatalFilter, message, "red")
            onQmlError: debugConsole.appendMessage(true, message, "magenta", true)
        }
    }

    Row {
        anchors { right: parent.right; top: parent.top }
        MouseArea {
            width: 64; height: 64
            visible: debugConsoleView.visible
            onClicked: debugConsole.clear()
            Text { anchors.centerIn: parent; text: "\u00D7"; color: "white"; font.pixelSize: 48 }
        }
        MouseArea {
            width: 64; height: 64
            visible: debugConsoleView.visible
            onClicked: unifiedTimer.slowMode = !unifiedTimer.slowMode
            Text { anchors.centerIn: parent; text: "\u2155"; color: unifiedTimer.slowMode ? "cyan" : "white"; font.pixelSize: 48 }
        }
        MouseArea {
            width: 64; height: 64
            onClicked: debugConsoleView.visible = !debugConsoleView.visible
            Text { anchors.centerIn: parent; text: "#"; color: "white"; font.pixelSize: 48; style: Text.Outline; styleColor: "black" }
        }
    }
}
