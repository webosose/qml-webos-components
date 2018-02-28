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
    property alias updateInterval: timer.interval

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "red"
        opacity: 0.4
    }

    MemoryInfo {
        id: mem

        mappings: [
            // The order matters as the parser will match in this order
            // and if the first globs too much then the latter Mappings
            // do not matter
            Mapping {
                id: gfx
                name: "/dev/kgsl-3d0" // TODO: Nexus specific
            },
            Mapping {
                id: libs
                name: "/.*/lib.*"
            }
        ]
    }

    Column {
        anchors.centerIn: parent
        Text {
            id: cpu
            text: "CPU [PD+PC]: " + (mem.totals.Private_Dirty + mem.totals.Private_Clean + gfx.data.Private_Clean + gfx.data.Private_Dirty) + "kB"
            font.pixelSize: 30
            font.bold: true
        }
        Text {
            id: gfx_privates
            text: "GFX [PD+PC]: " + (gfx.data.Private_Dirty + gfx.data.Private_Clean) + "kB"
            font.pixelSize: 30
            font.bold: true
        }
        Text {
            id: gfx_ref
            text: "GFX [Referenced]: " + gfx.data.Referenced + "kB"
            font.pixelSize: 30
            font.bold: true
        }
    }

    Timer {
        id: timer
        interval: 0 //0 means do it one time when it is opening
        repeat: interval > 0 ? true : false
        running: root.enabled
        onTriggered: if (root.enabled) mem.update();
    }

    Component.onCompleted: timer.start();
}
