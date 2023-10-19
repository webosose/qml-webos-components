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
#include <QDebug>

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
            long int diff = timeDiff(startTime, currentTime);
            if (diff > INT_MAX || diff < INT_MIN) {
                qWarning() << "timeDiff cannot be greater than " << INT_MAX << " or less than " << INT_MIN;
                return;
            }
            int msec = (int) diff;
            keyValPairs.insert("TIME", QJsonValue(msec));
            keyValPairs.insert("UNIT", QJsonValue(QString("ms")));

            timeLog(messageId, keyValPairs, freeText);
        }
    }
}

#ifndef HAS_PMLOGLIB
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
#else
void PerformanceLog::logInfoWithClock(const QString &messageId, const QJsonObject &keyVals, const QString &freeText)
{
    PmLogContext qmlContext;
    PmLogGetContext(m_context.toStdString().c_str(), &qmlContext);

    QJsonObject keyValPairs = keyVals;
    QString type, group;

    QString str_PerfType = QStringLiteral("PerfType");
    QString str_PerfGroup = QStringLiteral("PerfGroup");

    if (keyValPairs.contains(str_PerfType) && keyValPairs.value(str_PerfType).isString()) {
        type = keyValPairs.value(str_PerfType).toString();
        keyValPairs.remove(str_PerfType);
    }

    if (keyValPairs.contains(str_PerfGroup) && keyValPairs.value(str_PerfGroup).isString()) {
        group = keyValPairs.value(str_PerfGroup).toString();
        keyValPairs.remove(str_PerfGroup);
    }

    QJsonDocument keyValPairsDoc(keyValPairs);

    PmLogInfoWithClock(qmlContext, messageId.toStdString().c_str(), 2,
        PMLOGKS("PerfType", type.toStdString().c_str()),
        PMLOGKS("PerfGroup", group.toStdString().c_str()),
        "%s %s", keyValPairsDoc.toJson(QJsonDocument::Compact).constData(), (freeText.isEmpty() ? "" : freeText.toStdString().c_str()));
}
#endif

long int PerformanceLog::timeDiff(const struct timespec& startTime, const struct timespec& endTime)
{
    if (startTime.tv_sec > 0L && endTime.tv_sec < (LONG_MIN + startTime.tv_sec) || startTime.tv_sec < 0L && endTime.tv_sec > (LONG_MAX + startTime.tv_sec)) {
        qWarning() << "endTime.tv_sec cannot be less than " << LONG_MIN << " or greater than " << LONG_MAX;
        return 0;
    }
    if (startTime.tv_nsec > 0L && endTime.tv_nsec < (LONG_MIN + startTime.tv_nsec) || startTime.tv_nsec < 0L && endTime.tv_nsec > (LONG_MAX + startTime.tv_nsec)) {
        qWarning() << "endTime.tv_nsec cannot be less than " << LONG_MIN << " or greater than " << LONG_MAX;
        return 0;
    }

    if (endTime.tv_sec - startTime.tv_sec > (LONG_MAX / 1000L)) {
        qWarning() << "secToMs cannot be greater than " << LONG_MAX;
        return 0;
    }
    long int secToMs = (endTime.tv_sec - startTime.tv_sec) * 1000L;
    long int nsToMs = (endTime.tv_nsec - startTime.tv_nsec) / 1000000L;
    if (nsToMs > 0 && secToMs > (LONG_MAX - nsToMs) || nsToMs < 0 && secToMs < (LONG_MIN - nsToMs)) {
        qWarning() << "timeDiff cannot be greater than " << LONG_MAX << " or less than " << LONG_MIN;
        return 0;
    }
    return secToMs + nsToMs;
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
