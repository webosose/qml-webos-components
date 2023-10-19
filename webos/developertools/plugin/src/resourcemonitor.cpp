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

#include <QTimer>
#include <QQuickWindow>
#include <QFile>
#include <unistd.h>
#include "resourcemonitor.h"


const char * CpuStat::procStat = "/proc/stat";
const char * MemStat::procVmStat = "/proc/meminfo";

double CpuStat::getCpuUsage() {
    return cpuUsage;
}

bool CpuStat::update() {
    unsigned long consumeTime, totalTime;
    bool ret;

    if (!isInitialized) {
        isInitialized = loadStat(prev);

        return isInitialized;
    }

    ret = loadStat(cur);

    consumeTime = cur.user - prev.user +
        cur.nice - prev.nice +
        cur.sys - prev.sys;

    if (cur.idle > ULONG_MAX - consumeTime) {
        qWarning() << "Cannot increase cur.idle greater than " << ULONG_MAX;
        return false;
    }

    totalTime = consumeTime + cur.idle - prev.idle;

    if (totalTime != 0 && (consumeTime <= ULONG_MAX / 100UL)) {
        cpuUsage = 100UL * consumeTime / totalTime;
    } else {
        cpuUsage = 0.0;
    }

    prev = cur;

    return ret;
}

bool CpuStat::loadStat(struct cstat &s) {
    QFile file(procStat);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open " << procStat;
        return false;
    }

    QString tmp;
    QByteArray text = file.readAll();
    QTextStream stream(&text);
    stream >> tmp >> s.user >> s.nice >> s.sys >> s.idle;
    if (stream.status() != QTextStream::Ok) {
        qWarning() << "Failed to parse the contents of " << procStat;
        return false;
    }

    return true;
}

size_t MemStat::getMemTotal() {
    return mstat["MemTotal"];
}

size_t MemStat::getMemUsed() {
    if (mstat["MemTotal"] < mstat["MemFree"]) {
        qWarning() << "mstat for MemTotal cannot be less than " << 0U;
        return 0;
    }
    return mstat["MemTotal"] - mstat["MemFree"];
}

size_t MemStat::getBuffers() {
    return mstat["Buffers"];
}

size_t MemStat::getCached() {
    return mstat["Cached"];
}

size_t MemStat::getSwapTotal() {
    return mstat["SwapTotal"];
}

size_t MemStat::getSwapUsed() {
    if (mstat["SwapTotal"] < mstat["SwapFree"]) {
        qWarning() << "mstat for SwapTotal cannot be less than " << 0U;
        return 0;
    }
    return mstat["SwapTotal"] - mstat["SwapFree"];
}

size_t MemStat::operator [] (QString key) {
    return mstat[key];
}

bool MemStat::update() {
    char key[128];
    size_t value;
    FILE *fp;

    if ((fp = fopen(procVmStat, "r")) == NULL) {
        perror(__FUNCTION__);
        return false;
    }

    while (fscanf(fp, "%s %zu kB\n", key, &value) != EOF) {
        key[strlen(key)-1] = '\0';
        mstat[key] = value;
    }
    if (EOF == fclose(fp)) {
        qWarning() << "Faild to close " << procVmStat;
        return false;
    }

    return true;
}

ResourceMonitor::ResourceMonitor(QQuickItem *parent)
        : QQuickItem(parent)
{
    connect(this, SIGNAL(run()), this, SLOT(startResourceMonitor()));
}

void ResourceMonitor::startResourceMonitor()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(readValues()));
    timer->start(refreshInterval);
}

void ResourceMonitor::readValues()
{
    cpuStat.update();
    memStat.update();

    emit valueChanged();
}

double ResourceMonitor::cpuUsage()
{
    return cpuStat.getCpuUsage();
}

size_t ResourceMonitor::memTotal()
{
    return memStat.getMemTotal();
}

size_t ResourceMonitor::memUsed()
{
    return memStat.getMemUsed();
}

size_t ResourceMonitor::buffers()
{
    return memStat.getBuffers();
}

size_t ResourceMonitor::cached()
{
    return memStat.getCached();
}

size_t ResourceMonitor::swapTotal()
{
    return memStat.getSwapTotal();
}

size_t ResourceMonitor::swapUsed()
{
    return memStat.getSwapUsed();
}
