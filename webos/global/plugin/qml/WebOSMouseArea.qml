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

/* This is just a wrapper element for WebOSMouseAreaInternal which contains
 * real implementation.
 * This overrides some existing attributes of MouseArea to make this element
 * transparent to MouseArea.
 */

import QtQuick 2.4

WebOSMouseAreaInternal {
    // Override existing attributes
    property bool containsMouse: containsMouseInternal
    property bool touchPress: touchPressedInternal
    property bool touchPressedInternal: false

    // Provides highlight property for this area.
    // The basic concept is,
    // 1. Disable the highlight when cursor is visible.
    // 2. If the cursor is not visible, enable the highlight.
    property bool highlight: (cursorVisible && containsMouse) ||
                             (!cursorVisible && (parent.activeFocus||touchPress))

    signal entered
    signal exited

    onEnteredInternal: entered()
    onExitedInternal: exited()
}
