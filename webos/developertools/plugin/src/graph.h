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

#ifndef GRAPH_H
#define GRAPH_H

#include <QtQuick/QQuickItem>
#include <QQueue>
#include <QElapsedTimer>

class Graph : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int sampleCount READ sampleCount WRITE setSampleCount NOTIFY sampleCountChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Graph(QQuickItem *parent = 0);
    ~Graph();

    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    int sampleCount() const { return m_sampleCount; }
    void setSampleCount(int count);
    QColor color() { return m_color; }
    void setColor(QColor color);

    Q_INVOKABLE void addData(int data);
signals:
    void sampleCountChanged(int count);
    void colorChanged();

protected:
    QSGNode *m_node = nullptr;

private:
    int m_sampleCount;
    QColor m_color = "red";
};

#endif
