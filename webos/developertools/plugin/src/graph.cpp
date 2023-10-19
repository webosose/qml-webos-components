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

#include "graph.h"

#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QtGui/qopengl.h>
#include <QDebug>

class graphGeometryNode:
    public QSGGeometryNode
{
public:
    graphGeometryNode(): QSGGeometryNode() {
        setFlag(QSGNode::UsePreprocess);
        m_timer.start();
    }

    void preprocess() override {
        if (!m_customDataEnabled) {
            int64_t delta = m_timer.elapsed();
            m_timer.restart();
            m_dataQueue.append(delta);
        }

        int vertexCount = geometry()->vertexCount();
        while (m_dataQueue.size() > vertexCount) m_dataQueue.dequeue();

        QSGGeometry::Point2D *vertices = geometry()->vertexDataAsPoint2D();
        QList<int64_t>::const_iterator i;
        int j=0;
        for (i = m_dataQueue.constBegin(); i != m_dataQueue.constEnd() && j < vertexCount; ++i, ++j) {
            qreal x = m_bounds.x() + qreal(j) / qreal(vertexCount - 1) * m_bounds.width();
            float y = m_bounds.bottom() - *i;
            vertices[j].set(x, y);
        }
        for(; j < vertexCount; j++) {
            qreal x = m_bounds.x() + qreal(j) / qreal(vertexCount - 1) * m_bounds.width();
            float y = m_bounds.bottom();
            vertices[j].set(x, y);
        }
        markDirty(DirtyGeometry);
    }

    void setBounds(QRectF b) { m_bounds = b; }

    // Maybe: consider mutex for thread-safety
    void addData(int64_t data) {
        if (Q_UNLIKELY(!m_customDataEnabled))
            m_customDataEnabled = true;
        m_dataQueue.append(data);
    }
private:
    bool m_customDataEnabled = false; // use fps interval for graph data as default when user mode is disabled.
    QQueue<int64_t> m_dataQueue;
    QElapsedTimer m_timer;
    QRectF m_bounds;
};

Graph::Graph(QQuickItem *parent)
    : QQuickItem(parent)
    , m_sampleCount(960)
{
    setFlag(ItemHasContents, true);
}

Graph::~Graph()
{
}

void Graph::setSampleCount(int count)
{
    if (m_sampleCount == count)
        return;

    m_sampleCount = count;
    emit sampleCountChanged(count);
    update();
}

void Graph::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged();
    update();
}

QSGNode *Graph::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    graphGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    if (!oldNode) {
        node = new graphGeometryNode();
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_sampleCount);
        geometry->setLineWidth(1);
        geometry->setDrawingMode(GL_LINE_STRIP);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        // Consider what if color is set after first updatePaintNode
        material->setColor(m_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        // Consider its life cycle if we want to delete the node in someday
        m_node = node;
    } else {
        node = static_cast<graphGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(m_sampleCount);
    }

    node->setBounds(boundingRect());
    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

void Graph::addData(int64_t data)
{
    if (!m_node)
        return;
    graphGeometryNode *node = static_cast<graphGeometryNode *>(m_node);
    node->addData(data);
}
