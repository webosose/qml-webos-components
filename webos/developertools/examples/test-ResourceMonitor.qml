// Copyright (c) 2015-2018 LG Electronics, Inc.
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

Rectangle {
    width: 330
    height: 100
    color: "black"

    ResourceMonitor {
        id: resMon
        anchors.fill: parent

        Column {
            id: mainLayout
            anchors.fill: parent

            property int secondColRightMargin: 50

            // Cpu usage
            Item {
                width: parent.width
                height: childrenRect.height


                Text {
                    anchors.left: parent.left
                    color: "white"
                    font.pixelSize: 13
                    font.bold: true
                    text: "Cpu usage:"
                }
                Text {
                    anchors.right: parent.right

                    color: {return resMon.cpuUsage > 90?"red":"white"}
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    text: resMon.cpuUsage + "%"
                }
            }

            // Mem usage
            Item {
                width: parent.width
                height: childrenRect.height

                Text {
                    anchors.left: parent.left

                    color: "white"
                    font.pixelSize: 13
                    font.bold: true
                    text: "Mem usage:"
                }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: mainLayout.secondColRightMargin

                    color: "white"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    text: mainLayout.toMB(resMon.memUsed) + " / " + mainLayout.toMB(resMon.memTotal) + " MB"
                }
                Text {
                    anchors.right: parent.right

                    color: "white"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    text: {
                        return (resMon.memUsed / resMon.memTotal * 100).toFixed(1).toString() + " %"
                    }
                }
            }

            // Swap usage
            Item {
                width: parent.width
                height: childrenRect.height

                Text {
                    anchors.left: parent.left

                    color: "white"
                    font.pixelSize: 13
                    font.bold: true
                    text: "Swap usage:"
                }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: mainLayout.secondColRightMargin

                    color: "white"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    text: mainLayout.toMB(resMon.swapUsed) + " / " + mainLayout.toMB(resMon.swapTotal) + " MB"
                }
                Text {
                    anchors.right: parent.right

                    color: "white"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    text: {return (resMon.swapUsed / resMon.swapTotal * 100).toFixed(1).toString() + " %"}
                }
            }

            // Buffers
            Item {
                width: parent.width
                height: childrenRect.height

                Text {
                    anchors.left: parent.left

                    color: "white"
                    font.pixelSize: 13
                    font.bold: true
                    text: "Buffers:"
                }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: mainLayout.secondColRightMargin

                    color: "white"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    text: mainLayout.toMB(resMon.buffers) + " MB"
                }
            }

            // Cached
            Item {
                width: parent.width
                height: childrenRect.height

                Text {
                    anchors.left: parent.left

                    color: "white"
                    font.pixelSize: 13
                    font.bold: true
                    text: "Cached:"
                }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: mainLayout.secondColRightMargin

                    color: "white"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignRight
                    text: mainLayout.toMB(resMon.cached) + " MB"
                }
            }

            function toMB(kb) {
                return parseInt(kb / 1024)
            }
        }
    }
}
