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
import PmLog 1.0

Rectangle {
    width: 100; height: 300; color: "black"

    PmLog {
        id: pmlog
        context: "lsm"
    }

    Rectangle {
        id: top_rect
        x: 5; y: 5; width: 90; height: 90
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: PmLogger.error("TEST", {"color":"red", "x":1, "y":5}, "test box");
        }
    }

    Rectangle {
        id: center_rect
        x: 5; y: 105; width: 90; height: 90
        color: "yellow"
        MouseArea {
            anchors.fill: parent
            onClicked: pmlog.debug("Debug: Test");
        }
    }

    Rectangle {
        id: bottom_rect
        x: 5; y: 205; width: 90; height: 90
        color: "green"
        MouseArea {
            anchors.fill: parent
            onClicked: pmlog.info("TEST", {"color":"green", "x":5, "y":205}, "test box");
        }
    }
}
