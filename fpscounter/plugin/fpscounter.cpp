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

#include "fpscounter.h"
#include <unistd.h>
#include <QTimer>
#include <QQuickWindow>
#include <QFileInfo>

FpsCounter::FpsCounter(QQuickItem *parent)
        : QQuickItem(parent)
        , frameCounter(0)
        , framePerSecond(0)
        , displayFps(true)
        , displayTouch(false)
        , viewWindow(NULL)
{
    setVisible(false);
    QTimer::singleShot(3000, this, SLOT(checkFpsEnabled()));
}

void FpsCounter::checkFpsEnabled()
{
    bool showTouch = (!qgetenv("WEBOS_DISPLAY_TOUCH").isEmpty() && QFileInfo::exists(QString("/var/luna/preferences/devmode_enabled")));
    if (showTouch != displayTouch) {
      displayTouch = showTouch;
      emit touchChanged();
    }

    displayFps = (qgetenv("WEBOS_NO_DISPLAY_FPS").isEmpty() && QFileInfo::exists(QString("/var/luna/preferences/devmode_enabled")));

    if(!displayFps) {
        if(viewWindow != NULL) disconnect(viewWindow, SIGNAL(frameSwapped()), this, SLOT(countFps()));
        return;
    }
    setVisible(true);
    runFpsCounter();
}
void FpsCounter::runFpsCounter()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(measureFps()));
    timer->start(1000);
}

void FpsCounter::itemChange(ItemChange change, const ItemChangeData & value)
{
    if(!displayFps) return;
    // Only once it's added to the scene we get to know about the window
    // associated with the scene. So we can only connect frameSwapped signal
    // at this point of time:
    if (change == QQuickItem::ItemSceneChange) {
        // Connect frameSwapped signal when the Item is added to a window.
        if (value.window) {
            // Connect the frameSwapped signal
            viewWindow = window();
            connect(viewWindow, SIGNAL(frameSwapped()), SLOT(countFps()));
        }
        // Disconnect the frameSwapped signal once the Item is removed from the window.
        if (!value.window) {
            viewWindow = window();
            disconnect(viewWindow, SIGNAL(frameSwapped()), this, SLOT(countFps()));
        }
    }
}

int FpsCounter::fps()
{
    if(!displayFps) return 0;
    return framePerSecond;
}

bool FpsCounter::touch()
{
    return displayTouch;
}

void FpsCounter::countFps()
{
    frameCounter++;
}

void FpsCounter::measureFps()
{
    framePerSecond = frameCounter;
    frameCounter = 0;
    emit fpsChanged();
}
