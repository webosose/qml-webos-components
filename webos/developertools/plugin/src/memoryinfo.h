// Copyright (c) 2014-2021 LG Electronics, Inc.
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

#include <QTextStream>
#include <QQmlPropertyMap>
#include <QSet>
#include <QQmlListProperty>

class QTextStream;

class MemoryInfo : public QObject
{

    Q_OBJECT
    Q_PROPERTY(QString pid READ pid WRITE setPid NOTIFY pidChanged)
    /* Breakdown of the separate entries in smaps */
    Q_PROPERTY(QQmlListProperty<MemoryInfo::Mapping> mappings READ mappings)
    Q_PROPERTY(QObject* totals READ totals NOTIFY totalsChanged)

public:
    class Mapping;

    MemoryInfo(QObject* parent = 0);

    QString pid() { return m_pid; }
    void setPid(const QString& pid);

    QQmlListProperty<Mapping> mappings();

    QObject* totals() { return m_totals; }

public slots:
    void update();

signals:
    void pidChanged();
    void totalsChanged();

private:
    QString m_pid;
    QSet<QString> m_categories;
    QList<Mapping*> m_requestedMappings;
    QQmlPropertyMap* m_totals;

    void reset();
    void updateTotals();

    friend class Mapping;
};

class MemoryInfo::Mapping : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QObject* data READ data NOTIFY dataChanged)

public:
    Mapping(QObject* parent = 0);
    ~Mapping();

    QString name() { return m_name; }
    void setName(const QString& name);

    void read(MemoryInfo* info, QTextStream* in);
    QObject* data() { return m_map; }

    bool match(const QString& line);
    void resetMap(MemoryInfo* info);

signals:
    void nameChanged();
    void dataChanged();

private:
    MemoryInfo* m_memoryInfo;
    QString m_name;
    QQmlPropertyMap* m_map;

    friend class MemoryInfo;
};

inline void mappingAppend(QQmlListProperty<MemoryInfo::Mapping>* p, MemoryInfo::Mapping* s)
{
    // Catch the insertion into the list so that the qqmlpropertylist can be populated with
    // the correct category data before bindings will be evaluated from the qml side
    MemoryInfo* i = qobject_cast<MemoryInfo*>(p->object);
    s->resetMap(i);
    reinterpret_cast<QList<MemoryInfo::Mapping*> *>(p->data)->append(s);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline MemoryInfo::Mapping* mappingAt(QQmlListProperty<MemoryInfo::Mapping>* p, qsizetype idx)
#else
inline MemoryInfo::Mapping* mappingAt(QQmlListProperty<MemoryInfo::Mapping>* p, int idx)
#endif
{
    return reinterpret_cast<QList<MemoryInfo::Mapping*> *>(p->data)->at(idx);
}

inline void mappingClear(QQmlListProperty<MemoryInfo::Mapping>* p)
{
    reinterpret_cast<QList<MemoryInfo::Mapping*> *>(p->data)->clear();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline qsizetype mappingCount(QQmlListProperty<MemoryInfo::Mapping>* p)
#else
inline int mappingCount(QQmlListProperty<MemoryInfo::Mapping>* p)
#endif
{
    return reinterpret_cast<QList<MemoryInfo::Mapping*> *>(p->data)->count();
}
