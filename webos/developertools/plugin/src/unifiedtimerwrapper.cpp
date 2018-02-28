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

#include "unifiedtimerwrapper.h"
#include <private/qabstractanimation_p.h>

UnifiedTimerWrapper *UnifiedTimerWrapper::g_self = NULL;

UnifiedTimerWrapper::UnifiedTimerWrapper(QObject *parent) :
    QObject (parent),
    m_slowMode (false),
    m_framerateThreshold (0)
{
    g_self = this;
}


void UnifiedTimerWrapper::setSlowMode(bool enabled)
{
    QUnifiedTimer::instance()->setSlowModeEnabled(enabled);
    m_slowMode = enabled;
    emit slowModeChanged();
}


qint64 UnifiedTimerWrapper::framerateThreshold() const
{
    return m_framerateThreshold;
}


void UnifiedTimerWrapper::setFramerateThreshold(qint64 framerateThreshold)
{
    if (m_framerateThreshold == framerateThreshold)
        return;

    QUnifiedTimer::instance()->registerProfilerCallback(framerateThreshold > 0 ? &cb : NULL);
    m_framerateThreshold = framerateThreshold;
    emit framerateThresholdChanged();
}


void UnifiedTimerWrapper::cb(qint64 delta)
{
    if (g_self && g_self->m_framerateThreshold > 0 && g_self->m_framerateThreshold < delta)
        emit g_self->framerateDropDetected(delta);
}
