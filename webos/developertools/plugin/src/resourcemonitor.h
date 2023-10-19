// Copyright (c) 2015-2018 LG Electronics, Inc.
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

#ifndef _RESOURCEMONITOR_H_
#define _RESOURCEMONITOR_H_

#include <QQuickItem>
#include <QMap>
#include <QString>

class CpuStat
{
public:
    CpuStat ()
        : isInitialized(false)
        , cpuUsage(0.0) {}
    virtual ~CpuStat () {}

    double getCpuUsage();
    bool update();

private:
    struct cstat {
        unsigned long user;
        unsigned long nice;
        unsigned long sys;
        unsigned long idle;

        cstat()
            : user(0), nice(0), sys(0), idle(0) {}

    } prev, cur;

    static const char *procStat;
    bool isInitialized;
    double cpuUsage;

    bool loadStat(struct cstat &s);
};

class MemStat
{
public:
    MemStat () {}
    virtual ~MemStat () {}

    size_t getMemTotal();
    size_t getMemUsed();
    size_t getBuffers();
    size_t getCached();
    size_t getSwapTotal();
    size_t getSwapUsed();
    bool update();

    size_t operator [] (QString key);

private:
    static const char *procVmStat;
    QMap<QString, size_t> mstat;
};

class ResourceMonitor : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(double cpuUsage READ cpuUsage NOTIFY valueChanged)
    Q_PROPERTY(size_t memTotal READ memTotal NOTIFY valueChanged)
    Q_PROPERTY(size_t memUsed READ memUsed NOTIFY valueChanged)
    Q_PROPERTY(size_t buffers READ buffers NOTIFY valueChanged)
    Q_PROPERTY(size_t cached READ cached NOTIFY valueChanged)
    Q_PROPERTY(size_t swapTotal READ swapTotal NOTIFY valueChanged)
    Q_PROPERTY(size_t swapUsed READ swapUsed NOTIFY valueChanged)

public:
    ResourceMonitor(QQuickItem *parent = 0);
    double cpuUsage();
    size_t memTotal();
    size_t memUsed();
    size_t buffers();
    size_t cached();
    size_t swapTotal();
    size_t swapUsed();

private:
    static const int refreshInterval = 1000;
    CpuStat cpuStat;
    MemStat memStat;

signals:
    void valueChanged();
    void run();

private slots:
    void startResourceMonitor();
    void readValues();
};
#endif // _RESOURCEMONITOR_H_
