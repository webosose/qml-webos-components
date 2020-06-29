// Copyright (c) 2015-2020 LG Electronics, Inc.
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

#include <qweboskeyextension.h>

#include <QObject>
#include <QtQml/qqml.h>

class WebOSAttachedType : public QObject
{
    Q_OBJECT

    // QML directory
    Q_PROPERTY(QString qmlDir READ qmlDir NOTIFY qmlDirChanged)

    // Qt localization data directory
    Q_PROPERTY(QString localizationDir READ localizationDir NOTIFY localizationDirChanged)

    // Qt localization data directory
    Q_PROPERTY(QString applicationDir READ applicationDir NOTIFY applicationDirChanged)

public:
    WebOSAttachedType(QObject *parent);

    QString qmlDir() const;
    QString localizationDir() const;
    QString applicationDir() const;

signals:
    void qmlDirChanged();
    void localizationDirChanged();
    void applicationDirChanged();
};

class WebOS : public QObject {

    Q_OBJECT

public:
    WEBOS_KEY_ENUM_DECLARATION;
    Q_ENUMS(KeyWebOS);

    static WebOSAttachedType *qmlAttachedProperties(QObject *object) { return new WebOSAttachedType(object); }
};

QML_DECLARE_TYPEINFO(WebOS, QML_HAS_ATTACHED_PROPERTIES)
