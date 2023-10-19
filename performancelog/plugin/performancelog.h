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

#ifndef PERFORMANCELOG_H
#define PERFORMANCELOG_H

#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QString>

#include <time.h>

/*!
 * \class PerformanceLog class
 *
 * \brief This class provides a performancelog interface implementation
 */

class PerformanceLog : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QString context READ context WRITE setContext NOTIFY contextChanged)
public:
    PerformanceLog(QObject *parent = 0);
    QString context() const;
    void setContext(const QString &context);

private:
    QString m_context;
    /*
     * identify measure point
     */
    QString measurePoint(const QString &messageId, const QJsonObject &keyVals);
    /*
     * time difference calculation
     */
    long int timeDiff(const struct timespec& startTime, const struct timespec& endTime);
    /*
     * performance time logging
     */
    void timeLog(const QString &messageId, const QJsonObject &keyVals, const QString &freeText);

signals:
    void contextChanged();

public slots:
    /*
     * performance logging interfaces
     */
    void time(const QString &messageId, const QJsonObject &keyVals = QJsonObject());
    void timeEnd(const QString &messageId, const QJsonObject &keyVals = QJsonObject(), const QString &freeText = QString());
    void logInfoWithClock(const QString &messageId, const QJsonObject &keyVals = QJsonObject(), const QString &freeText = QString());
};
#endif // PERFORMANCELOG_H
