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

ResourceMonitor {
    id: resMon
    width: 300
    height: 120

    Rectangle {
        color: "black"
        anchors.fill: parent

        Column {
            id: mainLayout
            anchors.fill: parent
            anchors.margins: 10

            property int secondColRightMargin: 55
            property int fontSize: 15

            // Cpu usage
            Item {
                width: parent.width
                height: childrenRect.height


                Text {
                    anchors.left: parent.left
                    color: "white"
                    font.pixelSize: mainLayout.fontSize
                    font.bold: true
                    text: "Cpu usage:"
                }
                Text {
                    anchors.right: parent.right

                    color: {return resMon.cpuUsage > 90?"red":"white"}
                    font.pixelSize: mainLayout.fontSize
                    horizontalAlignment: Text.AlignRight
                    text: resMon.cpuUsage + " %"
                }
            }

            // Mem usage
            Item {
                width: parent.width
                height: childrenRect.height

                Text {
                    anchors.left: parent.left

                    color: "white"
                    font.pixelSize: mainLayout.fontSize
                    font.bold: true
                    text: "Mem usage:"
                }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: mainLayout.secondColRightMargin

                    color: "white"
                    font.pixelSize: mainLayout.fontSize
                    horizontalAlignment: Text.AlignRight
                    text: mainLayout.toMB(resMon.memUsed) + " / " + mainLayout.toMB(resMon.memTotal) + " MB"
                }
                Text {
                    anchors.right: parent.right

                    color: "white"
                    font.pixelSize: mainLayout.fontSize
                    horizontalAlignment: Text.AlignRight
                    text: {
                        var memPerc = resMon.memTotal ? (resMon.memUsed / resMon.memTotal * 100) : 0.0
                        return memPerc.toFixed(1).toString() + " %"
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
                    font.pixelSize: mainLayout.fontSize
                    font.bold: true
                    text: "Swap usage:"
                }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: mainLayout.secondColRightMargin

                    color: "white"
                    font.pixelSize: mainLayout.fontSize
                    horizontalAlignment: Text.AlignRight
                    text: mainLayout.toMB(resMon.swapUsed) + " / " + mainLayout.toMB(resMon.swapTotal) + " MB"
                }
                Text {
                    anchors.right: parent.right

                    color: "white"
                    font.pixelSize: mainLayout.fontSize
                    horizontalAlignment: Text.AlignRight
                    text: {
                        var swapPerc = resMon.swapTotal ? (resMon.swapUsed / resMon.swapTotal * 100) : 0.0
                        return swapPerc.toFixed(1).toString() + " %"
                    }
                }
            }

            // Buffers
            Item {
                width: parent.width
                height: childrenRect.height

                Text {
                    anchors.left: parent.left

                    color: "white"
                    font.pixelSize: mainLayout.fontSize
                    font.bold: true
                    text: "Buffers:"
                }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: mainLayout.secondColRightMargin

                    color: "white"
                    font.pixelSize: mainLayout.fontSize
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
                    font.pixelSize: mainLayout.fontSize
                    font.bold: true
                    text: "Cached:"
                }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: mainLayout.secondColRightMargin

                    color: "white"
                    font.pixelSize: mainLayout.fontSize
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
