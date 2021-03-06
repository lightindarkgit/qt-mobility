/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEGEOMAPPOLYGONOBJECT_H
#define QDECLARATIVEGEOMAPPOLYGONOBJECT_H

#include "qdeclarativegeomapobject_p.h"
#include "qdeclarativecoordinate_p.h"
#include "qdeclarativegeomapobjectborder_p.h"
#include "qgeomappolygonobject.h"

#include <QDeclarativeListProperty>

class QColor;
class QBrush;

QTM_BEGIN_NAMESPACE

class QDeclarativeGeoMapPolygonObject : public QDeclarativeGeoMapObject
{
    Q_OBJECT

    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeCoordinate> path READ declarativePath NOTIFY pathChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QDeclarativeGeoMapObjectBorder* border READ border)

public:
    QDeclarativeGeoMapPolygonObject(QDeclarativeItem *parent = 0);
    ~QDeclarativeGeoMapPolygonObject();

    QDeclarativeListProperty<QDeclarativeCoordinate> declarativePath();

    Q_INVOKABLE void addCoordinate(QDeclarativeCoordinate* coordinate);
    Q_INVOKABLE void removeCoordinate(QDeclarativeCoordinate* coordinate);

    QColor color() const;
    void setColor(const QColor &color);

    // From QDeclarativeParserStatus
    virtual void classBegin() {}
    virtual void componentComplete();

    QDeclarativeGeoMapObjectBorder* border();

Q_SIGNALS:
    void colorChanged(const QColor &color);
    void pathChanged();

private Q_SLOTS:
    void borderColorChanged(const QColor &color);
    void borderWidthChanged(int width);

private:
    static void path_append(QDeclarativeListProperty<QDeclarativeCoordinate> *prop, QDeclarativeCoordinate *coordinate);
    static int path_count(QDeclarativeListProperty<QDeclarativeCoordinate> *prop);
    static QDeclarativeCoordinate* path_at(QDeclarativeListProperty<QDeclarativeCoordinate> *prop, int index);
    static void path_clear(QDeclarativeListProperty<QDeclarativeCoordinate> *prop);
    void pathPropertyChanged();

    QGeoMapPolygonObject* polygon_;
    QList<QDeclarativeCoordinate*> path_;
    QColor color_;
    QDeclarativeGeoMapObjectBorder border_;
    bool componentCompleted_;
};

QTM_END_NAMESPACE

QML_DECLARE_TYPE(QTM_PREPEND_NAMESPACE(QDeclarativeGeoMapPolygonObject));

#endif
