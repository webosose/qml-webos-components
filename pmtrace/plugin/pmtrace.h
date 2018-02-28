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

#ifndef PMTRACE_H
#define PMTRACE_H

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QTime>


/*!
 * \class PmTrace class
 *
 * \brief This class provides a pmtrace interface implementation
 */

class PmTrace : public QObject
{
    Q_OBJECT

public:
    PmTrace(QObject *parent = 0);

public slots:
    /*
     * PmTrace interfaces
     */
    void traceLog(const QString &key);
    void traceLogEventTypeContextData(const QString &eventType, const QString &contextData);
    void traceLogWithPosition(const QString &key, const int &xPos, const int &yPos);
    void traceBefore(const QString &key);
    void traceAfter(const QString &key);
};

#endif // PMTRACE_H
