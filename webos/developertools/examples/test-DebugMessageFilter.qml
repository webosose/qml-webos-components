// Copyright (c) 2014-2021 LG Electronics, Inc.
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
    width: 360
    height: 360

    Text {
        id: out
    }

    DebugMessageFilter {
        onDebug: (message) => { out.text = out.text + '\nDebug: ' + message.text; }
        onWarning: (message) => { out.text = out.text + '\nWarning: ' + message.text; }
        onCritical: (message) => { out.text = out.text + '\nCritical: ' + message.text; }
        onFatal: (message) => { out.text = out.text + '\nFatal: ' + message.text; }
        onQmlError: (message) => { out.text = out.text + '\nQmlError: ' + message.text; }
    }

    Component.onCompleted: {
        console.debug("# test debug")
        console.log("$ test log")
        console.warn("test warn")
        try {
            foo.bar()
        } catch (e) {
            // will emit 2 critical messages
            console.exception("catched: foo.bar")
        }
        // will emit 1 warning and 1 qml error messages
        foo.bar()
    }
}
