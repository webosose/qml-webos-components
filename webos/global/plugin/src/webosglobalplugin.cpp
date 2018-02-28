// Copyright (c) 2015-2018 LG Electronics, Inc.
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

#include "webosglobalplugin.h"
#include "webos.h"

#include <QQmlComponent>

void WebOSGlobalPlugin::registerTypes(const char *uri)
{
    qmlRegisterUncreatableType<WebOS>(uri, 1, 0, "WebOS", "Not allowed to create WebOS instance");
}
