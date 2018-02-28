// Copyright (c) 2016-2018 LG Electronics, Inc.
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

#include "textfile.h"

#include <QUrl>
#include <QFileInfo>
#include <QDir>

TextFile::TextFile(QObject *parent) :
    QObject(parent)
{
}

TextFile::TextFile(const QString &name, QObject *parent) :
    QObject(parent), m_file(name)
{
}

void TextFile::initData(){

}

void TextFile::setName(const QString &name)
{
    // fix to convert URL's to local file names
    QUrl url(name);
    QString localName = url.isLocalFile() ? url.toLocalFile() : name;
    if (m_file.fileName() != localName) {
        m_file.setFileName(localName);
        emit nameChanged(localName);
    }
}

QString TextFile::fileName() const
{
    return QFileInfo(m_file).fileName();
}

QString TextFile::relativeFilePath(const QString &dir) const
{
    return QDir(dir).relativeFilePath(m_file.fileName());
}

bool TextFile::rename(const QString &newName)
{
    bool success = m_file.rename(newName);
    if (success) {
        emit nameChanged(newName);
    }
    return success;
}

bool TextFile::write(const QString &data)
{
    if (m_file.fileName().isEmpty()) {
        m_error = tr("empty name");
        return false;
    }

    if (data.isEmpty()) {
        m_error = tr("empty data");
        return false;
    }

    QDir().mkpath(QFileInfo(m_file).path());
    QByteArray text = data.toUtf8();
    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        m_error = tr("cannot open file '%1' for writing: %2")
                .arg(m_file.fileName()).arg((m_file.errorString()));
        return false;
    }
    bool success = m_file.write(text) == text.size();
    m_file.close();
    return success;
}

QVariant TextFile::read()
{
    if (m_file.fileName().isEmpty()) {
        m_error = tr("empty name");
        return QVariant(); //QVariant();
    }

    if (!m_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_error = tr("cannot open file '%1' for reading: %2")
                .arg(m_file.fileName()).arg((m_file.errorString()));
        return QVariant(); //QVariant();
    }

    QVariant text = (QString)m_file.readAll();

    m_file.close();

    return text;
}
