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
import RdxLog 1.0

Rectangle {
    width: 100; height: 100; color: "black"

    Rectangle {
        x: 5; y: 5; width: 90; height: 90
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: (mouse) => { RdxLog.makeReport("component", "cause", "detail", "contents"); }
        }
    }
}
