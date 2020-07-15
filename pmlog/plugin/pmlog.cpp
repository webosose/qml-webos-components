// Copyright (c) 2013-2020 LG Electronics, Inc.
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

#include "pmlog.h"

#include <iostream>
#include <syslog.h>

#include <QJsonDocument>

using namespace std;

PmLog::PmLog(QObject *parent)
            : QObject(parent),
              m_context(QString("qml"))
{
}

QString PmLog::context() const
{
    return m_context;
}

void PmLog::setContext(const QString &context)
{
    m_context = context;
}

void PmLog::critical(const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
#ifndef HAS_PMLOGLIB
    LogString(LOG_CRIT, messageId, keyVals, freeText);
#else
    LogString(kPmLogLevel_Critical, messageId, keyVals, freeText);
#endif
}

void PmLog::error(const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
#ifndef HAS_PMLOGLIB
    LogString(LOG_ERR, messageId, keyVals, freeText);
#else
    LogString(kPmLogLevel_Error, messageId, keyVals, freeText);
#endif
}

void PmLog::warning(const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
#ifndef HAS_PMLOGLIB
    LogString(LOG_WARNING, messageId, keyVals, freeText);
#else
    LogString(kPmLogLevel_Warning, messageId, keyVals, freeText);
#endif
}

void PmLog::info(const QString &messageId, const QJsonObject &keyVals, const QString &freeText, bool withClock)
{
#ifndef HAS_PMLOGLIB
    LogString(LOG_INFO, messageId, keyVals, freeText);
#else
    LogString(kPmLogLevel_Info, messageId, keyVals, freeText, withClock);
#endif
}

void PmLog::debug(const QString &freeText)
{
#ifndef HAS_PMLOGLIB
    LogString(LOG_DEBUG, NULL, PmLog::getDefaultKeyValues(), freeText);
#else
    LogString(kPmLogLevel_Debug, NULL, PmLog::getDefaultKeyValues(), freeText);
#endif
}

#ifndef HAS_PMLOGLIB
void PmLog::LogString(int level, const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
    QJsonDocument keyValsDoc(keyVals);
    QString keyValsStr(keyValsDoc.toJson(QJsonDocument::Compact));

    openlog(m_context.toStdString().c_str(), 0, 0);
    if (level == LOG_DEBUG)
        syslog(m_context.toStdString().c_str(), "%s", freeText.toStdString().c_str());
    else if (messageId != NULL)
        syslog(m_context.toStdString().c_str(), "%s %s %s", messageId.toStdString().c_str(), keyValsStr.toStdString().c_str(), freeText.toStdString().c_str());
    closelog();
}

#else

void PmLog::LogString(int level, const QString &messageId, const QJsonObject &keyVals, const QString &freeText, bool withClock)
{
    PmLogContext qmlContext;
    PmLogGetContext(m_context.toStdString().c_str(), &qmlContext);
    if (level == kPmLogLevel_Debug) {
        PmLogString(qmlContext, (PmLogLevel)level, NULL, NULL, freeText.toStdString().c_str());
    } else if (messageId != NULL) {
        QJsonObject keyValPairs = keyVals;
        if (withClock) {
            struct timespec currentTime;
            if (clock_gettime(CLOCK_MONOTONIC, &currentTime) == 0) {
                QString clockStr = QString("%1.%2")
                                   .arg(currentTime.tv_sec)
                                   .arg(currentTime.tv_nsec / 1000000, 3, 10, QChar('0'));
                keyValPairs.insert("CLOCK", QJsonValue(clockStr.toDouble()));
            }
        }
        if (keyValPairs.empty() == false) {
            QJsonDocument keyValsDoc(keyValPairs);
            PmLogString(qmlContext, (PmLogLevel)level, messageId.toStdString().c_str(), keyValsDoc.toJson(QJsonDocument::Compact).constData(), freeText.toStdString().c_str());
        } else {
            PmLogString(qmlContext, (PmLogLevel)level, messageId.toStdString().c_str(), NULL, freeText.toStdString().c_str());
        }
    }
}
#endif
