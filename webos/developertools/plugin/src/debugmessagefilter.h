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

#ifndef DEBUGMESSAGEFILTER_H
#define DEBUGMESSAGEFILTER_H

#include <QtCore/QObject>
#include <QtQml/QQmlParserStatus>
#include <QtQml/QQmlError>
#include <QtQml/QJSValue>


class GlobalMessageFilter : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY (GlobalMessageFilter)

public:
    static GlobalMessageFilter &instance();

signals:
    void qtMessage(int, const QString &, const QString &, const QString &, int);

private:
    GlobalMessageFilter();
    ~GlobalMessageFilter();
    QtMessageHandler static handler;
    static void message_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};


class DebugMessageFilter : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES (QQmlParserStatus)
    Q_DISABLE_COPY (DebugMessageFilter)

public:
    DebugMessageFilter(QObject *parent = 0);

    void classBegin() {}
    void componentComplete();

signals:
    void qmlError(const QJSValue &message);
    void debug(const QJSValue &message);
    void warning(const QJSValue &message);
    void critical(const QJSValue &message);
    void fatal(const QJSValue &message);

protected slots:
    void on_qmlWarnings(const QList<QQmlError> &);
    void on_qtMessage(int, const QString &, const QString &, const QString &, int);
};

#endif // DEBUGMESSAGEFILTER_H
