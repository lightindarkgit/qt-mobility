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

#include "qgraphicsgeomap.h"
#include "qgraphicsgeomap_p.h"

#include "qgeocoordinate.h"
#include "qgeoboundingbox.h"
#include "qgeomapobject.h"

#include "qgeoserviceprovider.h"
#include "qgeomappingmanager.h"
#include "qgeomapdata.h"

#include <QGraphicsSceneResizeEvent>
#include <QTimer>

#include <QNetworkProxyFactory>

#include <QDebug>

QTM_BEGIN_NAMESPACE

/*!
    \class QGraphicsGeoMap
    \brief The QGraphicsGeoMap class is used to display a map and manager the
    interactions between the user and the map.

    \inmodule QtLocation

    \ingroup maps-mapping

    Most of the functionality is provided by QGeoMappingManager, which
    handles most aspects of the display.

    The map viewport can be panned, the zoom level can be changed and the
    center coordinate of the viewport can be set explicitly.

    The screenPositionToCoordinate() and coordinateToScreenPoisition()
    functions can be used to convert between positions on the screen and
    global coordinates.

    Mouse and keyboard events should be handled by subclassing QGraphicsGeoMap
    and providing implementations of the event handling functions present in
    QGraphicsWidget.
*/

// DESIGN TODO do we need a signal for when the user pans the map?

/*!
\enum QGraphicsGeoMap::MapType

Describes a type of map data.

\value NoMap
Indicates a lack of map valid data.

\value StreetMap
The map data is a graphical representation of streets and building boundaries.

\value SatelliteMapDay
The map data is composed of images collected by satellites during the daytime.

\value SatelliteMapNight
The map data is composed of images collected by satellites during the nighttime.

\value TerrainMap
The map data is a graphical representation of terrain features.  This may also
include some of the information provided by QGraphicsGeoMap::StreetMap.
*/

// Temporary constructor, for use by QML bindings until we come up
// with the right QML / service provider mapping
QGraphicsGeoMap::QGraphicsGeoMap(QGraphicsItem *parent)
        : QGraphicsWidget(parent),
        d_ptr(new QGraphicsGeoMapPrivate())
{
    QNetworkProxyFactory::setUseSystemConfiguration(true);

    d_ptr->serviceProvider = new QGeoServiceProvider("nokia");
    d_ptr->manager = d_ptr->serviceProvider->mappingManager();

    setMappingManager(d_ptr->manager);

//    d_ptr->mapData = d_ptr->manager->createMapData(this);
//    setMapType(QGraphicsGeoMap::StreetMap);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    setMinimumSize(QSizeF(0, 0));
    setPreferredSize(QSizeF(500, 500));
//    d_ptr->mapData->setWindowSize(QSizeF(300, 300));
}

/*!
    Creates a new mapping widget, with the mapping operations managed by
    \a manager, and the specified \a parent.

    Note that the \a manager will typically be accessed from an instance of
    QGeoServiceProvider:
    \code
        QGeoServiceProvider serviceProvider("nokia");
        QGeoMappingManager *manager = serviceProvider.mappingManager();
        QGraphicsGeoMap *widget = new QGraphicsGeoMap(manager);
    \endcode
*/
QGraphicsGeoMap::QGraphicsGeoMap(QGeoMappingManager *manager, QGraphicsItem *parent)
        : QGraphicsWidget(parent),
        d_ptr(new QGraphicsGeoMapPrivate(manager))
{
    //d_ptr->mapData = d_ptr->manager->createMapData(this);
    //setMapType(QGraphicsGeoMap::StreetMap);
    setMappingManager(d_ptr->manager);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    setMinimumSize(QSizeF(0, 0));
    setPreferredSize(QSizeF(500, 500));
    //d_ptr->mapData->setWindowSize(QSizeF(300, 300));
}

/*!
    Destroys this map widget.
*/
QGraphicsGeoMap::~QGraphicsGeoMap()
{
    delete d_ptr;
}

/*!
    \internal
*/
void QGraphicsGeoMap::setMappingManager(QGeoMappingManager *manager)
{
    MapType type = QGraphicsGeoMap::StreetMap;

    d_ptr->manager = manager;

    if (d_ptr->mapData) {
        type = d_ptr->mapData->mapType();
        delete d_ptr->mapData;
    }

    d_ptr->mapData = d_ptr->manager->createMapData(this);
    d_ptr->mapData->setup();

    setMapType(type);
    d_ptr->mapData->setWindowSize(QSizeF(300, 300));

    connect(d_ptr->mapData,
           SIGNAL(zoomLevelChanged(qreal)),
           this,
           SIGNAL(zoomLevelChanged(qreal)));
    connect(d_ptr->mapData,
            SIGNAL(mapTypeChanged(QGraphicsGeoMap::MapType)),
            this,
            SIGNAL(mapTypeChanged(QGraphicsGeoMap::MapType)));
    connect(d_ptr->mapData,
            SIGNAL(centerChanged(QGeoCoordinate)),
            this,
            SIGNAL(centerChanged(QGeoCoordinate)));
}

/*!
  \reimp
*/
void QGraphicsGeoMap::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    if (d_ptr->mapData) {
        d_ptr->mapData->setWindowSize(event->newSize());
    }
}

/*!
  \reimp
*/
QPainterPath QGraphicsGeoMap::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

/*!
  \reimp
*/
void QGraphicsGeoMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (d_ptr->mapData)
        d_ptr->mapData->paint(painter, option);
}

/*!
    \property QGraphicsGeoMap::minimumZoomLevel
    \brief This property holds the minimum zoom level supported by the
    QGeoMappingManager associated with this widget.

    Larger values of the zoom level correspond to more detailed views of the
    map.
*/
qreal QGraphicsGeoMap::minimumZoomLevel() const
{
    if (d_ptr->manager)
        return d_ptr->manager->minimumZoomLevel();

    return -1;
}

/*!
    \property QGraphicsGeoMap::maximumZoomLevel
    \brief This property holds the maximum zoom level supported by the
    QGeoMappingManager associated with this widget.

    Larger values of the zoom level correspond to more detailed views of the
    map.
*/
qreal QGraphicsGeoMap::maximumZoomLevel() const
{
    if (d_ptr->manager)
        return d_ptr->manager->maximumZoomLevel();

    return -1;
}

/*!
    \property QGraphicsGeoMap::zoomLevel
    \brief This property holds the zoom level of the map.

    Larger values of the zoom level correspond to more detailed views of the
    map.

    If zoomLevel is less than minimumZoomLevel then minimumZoomLevel
    will be used, and if zoomLevel is  larger than
    maximumZoomLevel then maximumZoomLevel will be used.
*/
void QGraphicsGeoMap::setZoomLevel(qreal zoomLevel)
{
    if (d_ptr->mapData)
        d_ptr->mapData->setZoomLevel(zoomLevel);
}

qreal QGraphicsGeoMap::zoomLevel() const
{
    if (d_ptr->mapData)
        return d_ptr->mapData->zoomLevel();

    return -1;
}

/*!
    Pans the map view \a dx pixels in the x direction and \a dy pixels
    in the y direction.

    The x and y axes are specified in Graphics View Framework coordinates.
    By default this will mean that positive values of \a dx move the
    viewed area to the right and that positive values of \a dy move the
    viewed area down.
*/
void QGraphicsGeoMap::pan(int dx, int dy)
{
    if (d_ptr->mapData) {
        d_ptr->mapData->pan(dx, dy);
        update();
        emit panned(QPoint(dx, dy));
    }
}

/*!
    \property QGraphicsGeoMap::center
    \brief This property holds the coordinate at the center of the map viewport.

    Panning the map can be more efficient than changing the center by small
    increments.
*/
void QGraphicsGeoMap::setCenter(const QGeoCoordinate &center)
{
    if (d_ptr->mapData)
        d_ptr->mapData->setCenter(center);
}

QGeoCoordinate QGraphicsGeoMap::center() const
{
    if (d_ptr->mapData)
        return d_ptr->mapData->center();

    return QGeoCoordinate();
}

/*!
    Returns the map types supported by the QGeoMappingManager associated with
    this widget.
*/
QList<QGraphicsGeoMap::MapType> QGraphicsGeoMap::supportedMapTypes() const
{
    if (d_ptr->manager)
        return d_ptr->manager->supportedMapTypes();

    return QList<QGraphicsGeoMap::MapType>();
}

/*!
  \property QGraphicsGeoMap::mapType
  \brief This property holds the type of map data displayed by the map.

  Setting mapType to a type not present in supportedMapTypes() will do
  nothing.
*/
void QGraphicsGeoMap::setMapType(QGraphicsGeoMap::MapType mapType)
{
    if (d_ptr->mapData && d_ptr->manager) {
        if (!d_ptr->manager->supportedMapTypes().contains(mapType))
            return;

        d_ptr->mapData->setMapType(mapType);
    }
}

QGraphicsGeoMap::MapType QGraphicsGeoMap::mapType() const
{
    if (d_ptr->mapData)
        return d_ptr->mapData->mapType();

    return QGraphicsGeoMap::NoMap;
}

/*!
    Returns the map objects associated with this map.
*/
QList<QGeoMapObject*> QGraphicsGeoMap::mapObjects() const
{
    if (!d_ptr->mapData)
        return QList<QGeoMapObject*>();

    return d_ptr->mapData->mapObjects();
}

/*!
    Adds \a mapObject to the list of map objects managed by this widget.

    If \a mapObject is within the viewport of the map and
    QGeoMapObject::isVisible() returns true then the map will display the map
    object immediately.

    The map will take ownership of the \a mapObject.
*/
void QGraphicsGeoMap::addMapObject(QGeoMapObject *mapObject)
{
    if (!mapObject || !d_ptr->mapData)
        return;

    d_ptr->mapData->addMapObject(mapObject);

    this->update();
}

/*!
    Removes \a mapObject from the list of map objects managed by this widget.

    If \a mapObject is within the viewport of the map and
    QGeoMapObject::isVisible() returns true then the map will stop displaying
    the map object immediately.

    The map will release ownership of the \a mapObject.
*/
void QGraphicsGeoMap::removeMapObject(QGeoMapObject *mapObject)
{
    if (!mapObject || !d_ptr->mapData)
        return;

    d_ptr->mapData->removeMapObject(mapObject);

    this->update();
}

/*!
    Clears the map objects associated with this map.

    The map objects will be deleted.
*/
void QGraphicsGeoMap::clearMapObjects()
{
    if (!d_ptr->mapData)
        return;

    d_ptr->mapData->clearMapObjects();
}

/*!
    Returns a bounding box corresponding to the physical area displayed 
    in the viewport of the map.

    The bounding box which is returned is defined by the upper left and 
    lower right corners of the visible area of the map.
*/
QGeoBoundingBox QGraphicsGeoMap::viewport() const
{
    if (!d_ptr->mapData)
        return QGeoBoundingBox();

    return d_ptr->mapData->viewport();
}

/*!
    Attempts to fit the bounding box \a bounds into the viewport of the map.

    This method will change the zoom level to the maximum zoom level such 
    that all of \bounds is visible within the resulting viewport.

    If \a preserveViewportCenter is false the map will be centered on the 
    bounding box \a bounds before the zoom level is changed, otherwise the 
    center of the map will not be changed.
*/
void QGraphicsGeoMap::fitInViewport(const QGeoBoundingBox &bounds, bool preserveViewportCenter)
{
    if (!d_ptr->mapData)
        return;

    d_ptr->mapData->fitInViewport(bounds, preserveViewportCenter);
}

/*!
    Returns the list of visible map objects managed by this widget which
    contain the point \a screenPosition within their boundaries.
*/
QList<QGeoMapObject*> QGraphicsGeoMap::mapObjectsAtScreenPosition(const QPointF &screenPosition) const
{
    if (d_ptr->mapData)
        return d_ptr->mapData->mapObjectsAtScreenPosition(screenPosition);

    return QList<QGeoMapObject*>();
}

/*!
    Returns the list of visible map objects managed by this widget which are
    displayed at least partially within the on screen rectangle
    \a screenRect.
*/
QList<QGeoMapObject*> QGraphicsGeoMap::mapObjectsInScreenRect(const QRectF &screenRect) const
{
    if (d_ptr->mapData)
        return d_ptr->mapData->mapObjectsInScreenRect(screenRect);

    return QList<QGeoMapObject*>();
}

/*!
    Returns the list of visible map objects manager by this widget which 
    are displayed at least partially within the viewport of the map.
*/
QList<QGeoMapObject*> QGraphicsGeoMap::mapObjectsInViewport() const
{
    if (d_ptr->mapData)
        return d_ptr->mapData->mapObjectsInViewport();

    return QList<QGeoMapObject*>();
}

/*!
    Returns the position on the screen at which \a coordinate is displayed.

    An invalid QPointF will be returned if \a coordinate is invalid or is not
    within the current viewport.
*/
QPointF QGraphicsGeoMap::coordinateToScreenPosition(const QGeoCoordinate &coordinate) const
{
    if (d_ptr->mapData)
        return d_ptr->mapData->coordinateToScreenPosition(coordinate);

    return QPointF();
}

/*!
    Returns the coordinate corresponding to the point in the viewport at \a
    screenPosition.

    An invalid QGeoCoordinate will be returned if \a screenPosition is invalid
    or is not within the current viewport.
*/
QGeoCoordinate QGraphicsGeoMap::screenPositionToCoordinate(QPointF screenPosition) const
{
    if (d_ptr->mapData)
        return d_ptr->mapData->screenPositionToCoordinate(screenPosition);

    return QGeoCoordinate();
}

/*!
\fn void QGraphicsGeoMap::zoomLevelChanged(qreal zoomLevel)

Indicates that the zoom level has changed to \a zoomLevel.
*/

/*!
\fn void QGraphicsGeoMap::centerChanged(const QGeoCoordinate &coordinate)

Indicates that the map has been centered on \a coordinate.

This signal will not be emitted when the user pans the map.
*/

/*!
\fn void QGraphicsGeoMap::mapTypeChanged(MapType mapType)

Indicates that the type of the map has been changed.
*/

/*******************************************************************************
*******************************************************************************/

QGraphicsGeoMapPrivate::QGraphicsGeoMapPrivate(QGeoMappingManager *manager)
        : serviceProvider(0),
        manager(manager),
        mapData(0),
        panActive(false) {}

QGraphicsGeoMapPrivate::~QGraphicsGeoMapPrivate()
{
    if (mapData)
        delete mapData;
    if (serviceProvider)
        delete serviceProvider;
}

#include "moc_qgraphicsgeomap.cpp"

QTM_END_NAMESPACE