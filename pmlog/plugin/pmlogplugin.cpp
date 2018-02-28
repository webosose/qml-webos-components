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

#include "pmlogplugin.h"

#include <QQmlComponent>

#include "pmlog.h"

void PmLogPlugin::registerTypes(const char *uri)
{
    Q_UNUSED(uri);
    qmlRegisterType<PmLog>("PmLog", 1, 0, "PmLog");
    qmlRegisterSingletonType<PmLog>("PmLog", 1, 0, "PmLogger", pmLogProvider);
}

static QObject *PmLogPlugin::pmLogProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    PmLog *pmlog = new PmLog();
    return pmlog;
}
