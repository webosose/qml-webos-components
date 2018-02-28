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

#include "pmtraceplugin.h"

#include <QQmlComponent>

#include "pmtrace.h"

void PmTracePlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);
    qmlRegisterType<PmTrace>("PmTrace", 1, 0, "PmTrace");
    qmlRegisterSingletonType<PmTrace>("PmTrace", 1, 0, "PmTracer", pmTraceProvider);
}

static QObject *PmTracePlugin::pmTraceProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    PmTrace *pmtrace = new PmTrace();
    return pmtrace;
}
