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

#ifndef RDXLOG_H
#define RDXLOG_H

#include <QString>
#include <QObject>

#include <rdx.h>

/*!
 * \class RdxLog class
 *
 * \brief This class provides a rdx interface implementation
 */

class RdxLog : public QObject
{
   Q_OBJECT
public:
    RdxLog(QObject *parent = 0);

public slots:
    /*
     * reporting interfaces
     */
    void makeReport(const QString &component, const QString &cause, const QString &detail, const QString &contents);
};

#endif // RDXLOG_H
