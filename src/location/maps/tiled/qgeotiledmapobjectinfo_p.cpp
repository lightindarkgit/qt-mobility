/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeotiledmapobjectinfo_p.h"

#include "qgeotiledmapdata.h"
#include "qgeotiledmapdata_p.h"

#include <QGraphicsScene>

#include <QPolygonF>

#include <QDebug>

QTM_BEGIN_NAMESPACE

QGeoTiledMapObjectInfo::QGeoTiledMapObjectInfo(QGeoMapData *mapData, QGeoMapObject *mapObject)
        : QGeoMapObjectInfo(mapData, mapObject),
        graphicsItem(0)
{
    tiledMapData = static_cast<QGeoTiledMapData*>(mapData);
    tiledMapDataPrivate = static_cast<QGeoTiledMapDataPrivate*>(tiledMapData->d_ptr);
}

QGeoTiledMapObjectInfo::~QGeoTiledMapObjectInfo()
{
    if (graphicsItem)
        delete graphicsItem;
}

void QGeoTiledMapObjectInfo::addToParent()
{
    if (graphicsItem) {
        QGeoTiledMapObjectInfo *parentInfo
                = static_cast<QGeoTiledMapObjectInfo*>(parentObjectInfo());

        if (parentInfo)
            graphicsItem->setParentItem(parentInfo->graphicsItem);
        else
            tiledMapDataPrivate->scene->addItem(graphicsItem);

        tiledMapDataPrivate->itemMap.insert(graphicsItem, mapObject());
        graphicsItem->setVisible(mapObject()->isVisible());
        graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable);
        graphicsItem->setSelected(mapObject()->isSelected());
    }
}

void QGeoTiledMapObjectInfo::removeFromParent()
{
    if (graphicsItem) {
        tiledMapDataPrivate->scene->removeItem(graphicsItem);
        tiledMapDataPrivate->itemMap.remove(graphicsItem);
    }
}

void QGeoTiledMapObjectInfo::visibleChanged(bool visible)
{
    if (graphicsItem) {
        graphicsItem->setVisible(visible);
        updateItem();
    }
}

void QGeoTiledMapObjectInfo::selectedChanged(bool selected)
{
    if (graphicsItem) {
        graphicsItem->setSelected(selected);
        updateItem();
    }
}

QGeoBoundingBox QGeoTiledMapObjectInfo::boundingBox() const
{
    if (!graphicsItem)
        return QGeoBoundingBox();

    QRectF rect1 = graphicsItem->boundingRect();
    QGeoCoordinate topLeft1 = tiledMapData->worldPixelToCoordinate(rect1.topLeft().toPoint());
    QGeoCoordinate bottomRight1 = tiledMapData->worldPixelToCoordinate(rect1.bottomRight().toPoint());

    QGeoBoundingBox box1 = QGeoBoundingBox(topLeft1, bottomRight1);

    return box1;
}

bool QGeoTiledMapObjectInfo::contains(const QGeoCoordinate &coord) const
{
    QPoint point = tiledMapData->coordinateToWorldPixel(coord);

    if (graphicsItem && graphicsItem->contains(point))
        return true;

    return false;
}

void QGeoTiledMapObjectInfo::updateItem()
{
    // TODO use bounding rectangle of graphics items
    if (graphicsItem)
        tiledMapData->geoMap()->update();
}

QPolygonF QGeoTiledMapObjectInfo::createPolygon(const QList<QGeoCoordinate> &path,
        QGeoTiledMapData *tiledMapData,
        bool closedPath,
        qreal ypole)
{
    QPolygonF points;

    QGeoCoordinate lastCoord = closedPath ? path.last() : path.first();
    QPointF lastPoint = tiledMapData->coordinateToWorldPixel(lastCoord);

    int width = tiledMapData->maxZoomSize().width();

    for (int i = 0; i < path.size(); ++i) {
        const QGeoCoordinate &coord = path.at(i);

        if (!coord.isValid())
            continue;

        const qreal lng = coord.longitude();
        const qreal lastLng = lastCoord.longitude();

        // is the dateline crossed = different sign AND gap is large enough
        const bool crossesDateline = lastLng * lng < 0 && abs(lastLng - lng) > 180;

        // calculate base point
        QPointF point = tiledMapData->coordinateToWorldPixel(coord);

        // if the dateline is crossed, draw "around" the map over the chosen pole
        if (crossesDateline) {
            // is the shortest route east = dateline crossed XOR longitude is east by simple comparison
            const bool goesEast = crossesDateline != (lng > lastLng);
            // direction = positive if east, negative otherwise
            const qreal dir = goesEast ? 1 : -1;

            // lastPoint on this side
            const QPointF & L = lastPoint;

            // point on the other side
            const QPointF & P = point;

            // lastPoint on the other side
            QPointF L_ = L - QPointF(width * dir, 0);

            // point on this side
            QPointF P_ = P + QPointF(width * dir, 0);

            // TODO: make a better algorithm to make sure the off-screen points P' and L' are far enough from the dateline so the lines to the poles don't flicker through.
            // this works for now :)
            L_ += (L_ - P) * 7;
            P_ += (P_ - L) * 7;

            // pole point on this side
            QPointF O1 = QPointF(P_.x(), ypole);
            // pole point on the other side
            QPointF O2 = QPointF(L_.x(), ypole);

            //points.append(L); // implicit
            points.append(P_); // P'
            points.append(O1);
            points.append(O2);
            points.append(L_); // L'
            points.append(P);
        } else {
            // add point to polygon
            points.append(point);
        }

        lastCoord = coord;
        lastPoint = point;
    }

    return points;
}

QTM_END_NAMESPACE
