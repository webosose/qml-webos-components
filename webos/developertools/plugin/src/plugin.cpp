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

#include "plugin.h"
#include "debugmessagefilter.h"
#include "unifiedtimerwrapper.h"
#include "memoryinfo.h"
#include "fpsgraph.h"
#include "resourcemonitor.h"

#include <qqml.h>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>

void DeveloperToolsPlugin::registerTypes(const char *uri)
{
    // @uri WebOS.DeveloperTools
    qmlRegisterType<DebugMessageFilter>(uri, 1, 0, "DebugMessageFilter");
    qmlRegisterType<MemoryInfo>(uri, 1, 0, "MemoryInfo");
    qmlRegisterType<MemoryInfo::Mapping>(uri, 1, 0, "Mapping");
    qmlRegisterType<FpsGraph>(uri, 1, 0, "FpsGraph");
    qmlRegisterType<ResourceMonitor>(uri, 1, 0, "ResourceMonitor");
}


void DeveloperToolsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED (uri)
    engine->rootContext()->setContextProperty("unifiedTimer", new UnifiedTimerWrapper(engine));
}
