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

#include "fpsgraph.h"

#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <QtGui/qopengl.h>
#include <QDebug>

class fpsGeometryNode:
    public QSGGeometryNode
{
public:
    fpsGeometryNode(): QSGGeometryNode() {
        setFlag(QSGNode::UsePreprocess);
        timer.start();
    }
    void preprocess() Q_DECL_OVERRIDE {
        int delta = timer.elapsed();
        timer.restart();
        msecs.append(delta);
        while (msecs.size() > geometry()->vertexCount()) msecs.dequeue();

        QSGGeometry::Point2D *vertices = geometry()->vertexDataAsPoint2D();
        QList<int>::const_iterator i;
        int j=0;
        for (i = msecs.constBegin(); i != msecs.constEnd(); ++i) {
            qreal x = bounds.x() + qreal(j) / qreal(geometry()->vertexCount() - 1) * bounds.width();
            float y = bounds.bottom() - *i;
            vertices[j].set(x, y);
            j++;
        }
        for(; j < geometry()->vertexCount(); j++) {
            qreal x = bounds.x() + qreal(j) / qreal(geometry()->vertexCount() - 1) * bounds.width();
            float y = bounds.bottom();
            vertices[j].set(x, y);
        }
        markDirty(DirtyGeometry);
    }
    QRectF bounds;
    QQueue<int> msecs;
    QElapsedTimer timer;
};


FpsGraph::FpsGraph(QQuickItem *parent)
    : QQuickItem(parent)
    , m_sampleCount(960)
{
    setFlag(ItemHasContents, true);
}

FpsGraph::~FpsGraph()
{
}

void FpsGraph::setSampleCount(int count)
{
    if (m_sampleCount == count)
        return;

    m_sampleCount = count;
    emit sampleCountChanged(count);
    update();
}

QSGNode *FpsGraph::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    fpsGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    if (!oldNode) {
        node = new fpsGeometryNode();
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_sampleCount);
        geometry->setLineWidth(1);
        geometry->setDrawingMode(GL_LINE_STRIP);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(QColor(255, 0, 0));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        node = static_cast<fpsGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(m_sampleCount);
    }

    node->bounds = boundingRect();
    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
