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

/* WebOSMouseArea:
 * A custom MouseArea element being aware of cursor visible state.
 *
 * Purpose:
 * Provide a replacement to make work-around for unintended behavior of
 * MouseArea due to cursor visible state changes.
 *
 * Implementation:
 * Re-implement the original operation of some attributes of MouseArea
 * to take account of mouse hovering events and cursor visible state together.
 * All attributes of WebOSMouseArea is transparent to MouseArea
 * by wrapping and overriding the WebOSMouseAreaInternal.
 *
 * Difference from the original operation:
 * 1. When the cursor disappears with the MouseArea being hovered:
 *    - Signal exited.
 *    - Set containsMouse to false.
 * 2. When the MouseArea is created where the cursor was disappeared:
 *    - Do not signal entered.
 *    - Do not change containsMouse.
 *
 * Usage:
 * When a certain MouseArea needs to be considered with cursor visibility,
 * replace it with this element, WebOSMouseArea and set a binding to cursorVisible.
 */

import QtQuick 2.4

MouseArea {
    // The component that is using WebOSMouseArea must set a binding to this
    // in order to reflect the cursor visibility properly.
    property bool cursorVisible: false

    // New attributes to re-implement corresponding original attributes.
    property bool containsMouseInternal: false

    signal enteredInternal
    signal exitedInternal

    onCursorVisibleChanged: if (!cursorVisible) containsMouseInternal = false

    onContainsMouseInternalChanged: (containsMouseInternal) ? enteredInternal() : exitedInternal()

    // onPositionChanged is needed to signal "entered" when re-entering into the
    // mouse area after the cursor hides ("exited" is not signaled).
    // Original containsMouse condition is also needed to cover the situation of
    // dragging out of the mouse area.
    onPositionChanged: (mouse) => { if (containsMouse) containsMouseInternal = cursorVisible; }

    // For the "Difference 2" situation described above,
    // containsMouseInternal should depend on current cursor visible state
    // to prevent signaling "entered" in that situation.
    onEntered: containsMouseInternal = cursorVisible

    onExited: containsMouseInternal = false
}
