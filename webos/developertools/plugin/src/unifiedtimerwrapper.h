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

#ifndef DEBUGCONTEXT_H
#define DEBUGCONTEXT_H

#include <QObject>

class UnifiedTimerWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY (bool slowMode READ slowMode WRITE setSlowMode NOTIFY slowModeChanged)
    Q_PROPERTY (qint64 framerateThreshold READ framerateThreshold WRITE setFramerateThreshold NOTIFY framerateThresholdChanged)

public:
    explicit UnifiedTimerWrapper(QObject *parent = 0);
    bool slowMode() { return m_slowMode; }
    void setSlowMode(bool);
    qint64 framerateThreshold() const;
    void setFramerateThreshold(qint64);

signals:
    void slowModeChanged();
    void framerateThresholdChanged();
    void framerateDropDetected(qint64 delta);

private:
    static void cb(qint64 delta);
    bool m_slowMode;
    qint64 m_framerateThreshold;
    static UnifiedTimerWrapper *g_self;
};

#endif // DEBUGCONTEXT_H
