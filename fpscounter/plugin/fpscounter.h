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

#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <QQuickItem>

/*!
 * \class FpsCounter class
 *
 * \brief This class provides a FpsCounter implementation
 */

class FpsCounter : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(bool touch READ touch NOTIFY touchChanged)

public:
    FpsCounter(QQuickItem *parent = 0);
    /*!
     * \brief Get fps number
     */
    int fps();
    bool touch();

private:
    int frameCounter;
    int framePerSecond;
    bool displayFps;
    bool displayTouch;
    QQuickWindow *viewWindow;
    /*!
     * \brief Run timer for measuring frame per seconds
     */
    void runFpsCounter();
protected:
    /*!
     * \brief Reimplemented from QQuickItem to be informed about the
     * presence of a window. Only once we know which window we use
     * for connecting frameSwapped signal
     */
    void itemChange(ItemChange change, const ItemChangeData &value);

signals:
    /*!
     * \brief Signal when interval is changed
     */
    void fpsChanged();
    void touchChanged();

private slots:
    /*!
     * \brief check that FPS is enabled.
     */
    void checkFpsEnabled();

    /*!
     * \brief Count fps when frameSwapped
     */
    void countFps();
    /*!
     * \brief Measure frame per second
     */
    void measureFps();
};
#endif // FPSCOUNTER_H
