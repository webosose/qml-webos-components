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

#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <QObject>
#include <QFile>
#include <QVariant>

class TextFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY nameChanged)
    Q_PROPERTY(bool exists READ exists)
    Q_PROPERTY(bool writeable READ writeable)
    Q_PROPERTY(bool readable READ readable)
    Q_PROPERTY(qint64 size READ size)
    Q_PROPERTY(QString error READ error)

public:
    explicit TextFile(QObject *parent = 0);
    TextFile(const QString &name, QObject *parent = 0);

    inline QString name() const { return m_file.fileName(); }
    QString fileName() const;
    inline bool exists() const { return m_file.exists(); }
    inline bool writeable() const { return m_file.permissions().testFlag(QFileDevice::WriteUser); }
    inline bool readable() const { return m_file.permissions().testFlag(QFileDevice::ReadUser); }
    inline qint64 size() const { return m_file.size(); }
    inline QString error() const { return m_error; }

    Q_INVOKABLE QString relativeFilePath(const QString &dir = QString()) const;
    Q_INVOKABLE bool rename(const QString &newName);

    Q_INVOKABLE inline bool copy(const QString &newName) { return m_file.copy(newName); }
    Q_INVOKABLE inline bool remove() { return m_file.remove(); }

    Q_INVOKABLE bool write(const QString &data);
    Q_INVOKABLE QVariant read();

    Q_INVOKABLE void initData();

signals:
    void nameChanged(const QString &name);

public slots:
    void setName(const QString &name);

private:
    QFile m_file;
    QString m_error;
};

#endif // TEXTFILE_H
