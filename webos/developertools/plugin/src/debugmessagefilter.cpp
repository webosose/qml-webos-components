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

#include "debugmessagefilter.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>

QtMessageHandler GlobalMessageFilter::handler = NULL;

// Thread-safe according to §6.7.4 of the (C++11) standard:
// If control enters the declaration concurrently while the variable is being
// initialized, the concurrent execution shall wait for completion of the
// initialization.
GlobalMessageFilter &GlobalMessageFilter::instance()
{
    static GlobalMessageFilter instance;
    return instance;
}


GlobalMessageFilter::GlobalMessageFilter()
{
    handler = qInstallMessageHandler(GlobalMessageFilter::message_handler);
}


GlobalMessageFilter::~GlobalMessageFilter()
{
}

void GlobalMessageFilter::message_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    emit GlobalMessageFilter::instance().qtMessage(type, msg, context.file, context.function, context.line);

    if (handler)
        handler(type, context, msg);
}


DebugMessageFilter::DebugMessageFilter(QObject *parent):
    QObject (parent)
{
}


void DebugMessageFilter::componentComplete()
{
    QQmlContext *ctx = QQmlEngine::contextForObject(this);
    Q_ASSERT (ctx);
    if (!ctx)
        return;

    QQmlEngine *engine = ctx->engine();
    Q_ASSERT (engine);
    if (!engine)
        return;

    QObject::connect(engine, &QQmlEngine::warnings, this, &DebugMessageFilter::on_qmlWarnings);

    GlobalMessageFilter *instance = &GlobalMessageFilter::instance();
    QObject::connect(instance, &GlobalMessageFilter::qtMessage, this, &DebugMessageFilter::on_qtMessage);
}


void DebugMessageFilter::on_qmlWarnings(const QList<QQmlError> &warnings)
{
    QQmlContext *ctx = QQmlEngine::contextForObject(this);
    Q_ASSERT (ctx);
    if (!ctx)
        return;

    QQmlEngine *engine = ctx->engine();
    Q_ASSERT (engine);
    if (!engine)
        return;

    foreach (const QQmlError &qml_error, warnings) {
        QJSValue msg = engine->newObject();
        msg.setProperty("text", qml_error.description());
        msg.setProperty("file", qml_error.url().toDisplayString(QUrl::FormattingOptions(QUrl::PreferLocalFile)));
        msg.setProperty("line", qml_error.line());
        msg.setProperty("column", qml_error.column());
        emit qmlError(msg);
    }
}


void DebugMessageFilter::on_qtMessage(int type, const QString &message, const QString &file, const QString &function, int line)
{
    QQmlContext *ctx = QQmlEngine::contextForObject(this);
    Q_ASSERT (ctx);
    if (!ctx)
        return;

    QQmlEngine *engine = ctx->engine();
    Q_ASSERT (engine);
    if (!engine)
        return;

    QJSValue msg = engine->newObject();
    msg.setProperty("text", message);
    msg.setProperty("file", QUrl(file).toDisplayString(QUrl::FormattingOptions(QUrl::PreferLocalFile)));
    msg.setProperty("line", line);
    msg.setProperty("function", function);

    switch (type) {
    case QtDebugMsg:
        emit debug(msg);
        break;
    case QtWarningMsg:
        emit warning(msg);
        break;
    case QtCriticalMsg:
        emit critical(msg);
        break;
    case QtFatalMsg:
        emit fatal(msg);
    }
}
