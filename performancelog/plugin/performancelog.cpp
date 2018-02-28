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

#include "performancelog.h"
#include "performancelogtracer.h"
#include <iostream>
#ifdef HAS_PMLOGLIB
#include <PmLogLib.h>
#else
#include <syslog.h>
#endif

#include <QJsonDocument>
#include <QJsonValue>

using namespace std;

static QMap<QString, struct timespec> s_performanceMap;

PerformanceLog::PerformanceLog(QObject *parent)
        : QObject(parent),
        m_context(QString("qmlPerformance"))
{
}

QString PerformanceLog::context() const
{
    return m_context;
}

void PerformanceLog::setContext(const QString &context)
{
    m_context = context;
}

QString PerformanceLog::measurePoint(const QString &messageId, const QJsonObject &keyVals)
{
    QString point;
    if(keyVals.isEmpty()) {
        point = messageId + "{}";
    } else {
        QByteArray keyValsJson(QJsonDocument(keyVals).toJson(QJsonDocument::Compact));
        PMTRACE_LOG(keyValsJson.constData());
        point = messageId + "{" + keyValsJson + "}";
    }
    return point;
}

void PerformanceLog::time(const QString &messageId, const QJsonObject &keyVals)
{
    PMTRACE_BEFORE(messageId.toStdString().c_str());
    QString point = measurePoint(messageId, keyVals);
    struct timespec currentTime;
    if (clock_gettime(CLOCK_MONOTONIC, &currentTime) == 0)
    {
        s_performanceMap[point] = currentTime;
    }
}

void PerformanceLog::timeEnd(const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
    PMTRACE_AFTER(messageId.toStdString().c_str());

    QString point = measurePoint(messageId, keyVals);
    if (s_performanceMap.contains(point)) {
        struct timespec currentTime;
        if (clock_gettime(CLOCK_MONOTONIC, &currentTime) == 0)
        {
            QJsonObject keyValPairs = keyVals;
            struct timespec startTime = s_performanceMap.take(point);
            int msec = timeDiff(startTime, currentTime);

            keyValPairs.insert("TIME", QJsonValue(msec));
            keyValPairs.insert("UNIT", QJsonValue(QString("ms")));

            timeLog(messageId, keyValPairs, freeText);
        }
    }
}

void PerformanceLog::logInfoWithClock(const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
    struct timespec currentTime;
    QJsonObject keyValPairs = keyVals;

    if (clock_gettime(CLOCK_MONOTONIC, &currentTime))
        return;

    QString clockStr =  QString("%1.%2")
        .arg(currentTime.tv_sec)
        .arg(currentTime.tv_nsec / 1000000, 3, 10, QChar('0'));
    keyValPairs.insert("CLOCK", QJsonValue(clockStr.toDouble()));

    timeLog(messageId, keyValPairs, freeText);
}

int PerformanceLog::timeDiff(const struct timespec& startTime, const struct timespec& endTime)
{
    return (endTime.tv_sec - startTime.tv_sec)*1000 +     // convert seconds to milliseconds
           (endTime.tv_nsec - startTime.tv_nsec)/1000000; // convert nanoseconds to milliseconds
}

#ifndef HAS_PMLOGLIB
void PerformanceLog::timeLog(const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
    QJsonObject keyValPairs;
    if(&keyVals != NULL) {
        keyValPairs = keyVals;
    }

    openlog(m_context.toStdString().c_str(), 0, 0);
    if(level == LOG_DEBUG) {
        syslog(m_context.toStdString().c_str(), "%s", freeText.toStdString().c_str());
    } else if(messageId != NULL) {
        QJsonDocument keyValPairsDoc(keyValPairs);
        syslog(m_context.toStdString().c_str(), "%s %s %s", messageId.toStdString().c_str(), keyVals.toStdString().c_str(), freeText.toStdString().c_str());
    }
    closelog();
}
#else
void PerformanceLog::timeLog(const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
    PmLogContext qmlContext;
    PmLogGetContext(m_context.toStdString().c_str(), &qmlContext);

    QJsonObject keyValPairs;
    if(&keyVals != NULL) {
        keyValPairs = keyVals;
    }

    QJsonDocument keyValPairsDoc(keyValPairs);
    PmLogString(qmlContext, kPmLogLevel_Info, messageId.toStdString().c_str(), keyValPairsDoc.toJson().constData(), freeText.toStdString().c_str());
}
#endif
