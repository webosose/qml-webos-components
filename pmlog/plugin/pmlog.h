// Copyright (c) 2013-2019 LG Electronics, Inc.
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

#ifndef PMLOG_H
#define PMLOG_H

#include <QString>
#include <QObject>
#include <QJsonObject>

#if HAS_PMLOGLIB
#include <PmLogLib.h>
#endif

/*!
 * \class PmLog class
 *
 * \brief This class provides a pmlog interface implementation
 */

class PmLog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString context READ context WRITE setContext NOTIFY contextChanged)

public:
    PmLog(QObject *parent = 0);
    /*!
     * \brief Get and Set PmLog Context.
     *
     * The context is a setting for each log.
     * And context name is added to log string.
     */
    QString context() const;
    void setContext(const QString &context);

private:
    QString m_context;
    void LogString(int level, const QString &messageId, const QJsonObject &keyVals, const QString &freeText, bool withClock = false);
    static const QJsonObject getDefaultKeyValues() {
        static const QJsonObject obj;
        return obj;
    }

signals:
    void contextChanged();

public slots:
    /*
     * logging interfaces
     */
    void critical(const QString &messageId, const QJsonObject &keyVals = PmLog::getDefaultKeyValues(), const QString &freeText = NULL);
    void error(const QString &messageId, const QJsonObject &keyVals = PmLog::getDefaultKeyValues(), const QString &freeText = NULL);
    void warning(const QString &messageId, const QJsonObject &keyVals = PmLog::getDefaultKeyValues(), const QString &freeText = NULL);
    void info(const QString &messageId, const QJsonObject &keyVals = PmLog::getDefaultKeyValues(), const QString &freeText = NULL, bool withClock = false);
    void debug(const QString &freeText = NULL);
};

#endif // PMLOG_H
