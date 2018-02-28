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

#include "pmtrace.h"

#include <iostream>
#include <syslog.h>

#include <QJsonDocument>

#include "QmlPluginTracer.h"

using namespace std;

PmTrace::PmTrace(QObject *parent)
            : QObject(parent)
{
}

void PmTrace::traceLog(const QString &key)
{
    PMTRACE_LOG(key.toStdString().c_str());
}

void PmTrace::traceLogEventTypeContextData(const QString &eventType, const QString &contextData)
{
    PMTRACE_KEY_VALUE_LOG(eventType.toStdString().c_str(), contextData.toStdString().c_str());
}

void PmTrace::traceLogWithPosition(const QString &key, const int &xPos, const int &yPos)
{
    PMTRACE_POSITION(key.toStdString().c_str(), xPos, yPos);
}

void PmTrace::traceBefore(const QString &key)
{
    PMTRACE_BEFORE(key.toStdString().c_str());
}

void PmTrace::traceAfter(const QString &key)
{
    PMTRACE_AFTER(key.toStdString().c_str());
}
