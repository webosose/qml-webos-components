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

#include "performancelogplugin.h"

#include <QQmlComponent>

#include "performancelog.h"

void PerformanceLogPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);
    qmlRegisterType<PerformanceLog>("PerformanceLog", 1, 0, "PerformanceLog");
    qmlRegisterSingletonType<PerformanceLog>("PerformanceLog", 1, 0, "PerformanceLogger", performanceLogProvider);
}

static QObject *PerformanceLogPlugin::performanceLogProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    PerformanceLog *performancelog = new PerformanceLog();
    return performancelog;
}
