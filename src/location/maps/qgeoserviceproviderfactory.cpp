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

#include "qgeoserviceproviderfactory.h"

QTM_BEGIN_NAMESPACE

/*!
    \class QGeoServiceProviderFactory

    \brief The QGeoServiceProviderFactory class is a factory class used as the
    plugin interface for services related to geographical information.


    \inmodule QtLocation
    \since 1.1

    \ingroup maps-impl

    Implementers must provide a unique combination of providerName() and
    providerVersion() per plugin.

    The other functions should be overridden if the plugin supports the
    associated set of functionality.
*/

/*!
\fn QGeoServiceProviderFactory::~QGeoServiceProviderFactory()

Destroys this QGeoServiceProviderFactory instance.
*/


/*!
\fn QString QGeoServiceProviderFactory::providerName() const

Returns the string used to identify the service provider behind this implementation.

The combination of providerName() and providerVersion() should be unique
amongst the plugins.
\since 1.1
*/

/*!
\fn int QGeoServiceProviderFactory::providerVersion() const

Returns the version of the plugin.

The combination of providerName() and providerVersion() should be unique
amongst the plugins.
\since 1.1
*/

/*!
    Returns a new QGeoSearchManagerEngine instance, initialized with \a
    parameters, which implements as much of the places searching functionality
    as the service provider supports.

    If \a error is not 0 it should be set to QGeoServiceProvider::NoError on
    success or an appropriate QGeoServiceProvider::Error on failure.

    If \a errorString is not 0 it should be set to a string describing any
    error which occurred.

    The default implementation returns 0, which causes a
    QGeoServiceProvider::NotSupportedError in QGeoServiceProvider.
    \since 1.1
*/
QGeoSearchManagerEngine* QGeoServiceProviderFactory::createSearchManagerEngine(const QMap<QString, QVariant> &parameters,
        QGeoServiceProvider::Error *error,
        QString *errorString) const
{
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    return 0;
}

/*!
    Returns a new QGeoMappingManagerEngine instance, initialized with \a
    parameters, which implements as much of the places searching functionality
    as the service provider supports.

    If \a error is not 0 it should be set to QGeoServiceProvider::NoError on
    success or an appropriate QGeoServiceProvider::Error on failure.

    If \a errorString is not 0 it should be set to a string describing any
    error which occurred.

    The default implementation returns 0, which causes a
    QGeoServiceProvider::NotSupportedError in QGeoServiceProvider.
    \since 1.1
*/
QGeoMappingManagerEngine* QGeoServiceProviderFactory::createMappingManagerEngine(const QMap<QString, QVariant> &parameters,
        QGeoServiceProvider::Error *error,
        QString *errorString) const
{
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    return 0;
}

/*!
    Returns a new QGeoRoutingManagerEngine instance, initialized with \a
    parameters, which implements as much of the places searching functionality
    as the service provider supports.

    If \a error is not 0 it should be set to QGeoServiceProvider::NoError on
    success or an appropriate QGeoServiceProvider::Error on failure.

    If \a errorString is not 0 it should be set to a string describing any
    error which occurred.

    The default implementation returns 0, which causes a
    QGeoServiceProvider::NotSupportedError in QGeoServiceProvider.
    \since 1.1
*/
QGeoRoutingManagerEngine* QGeoServiceProviderFactory::createRoutingManagerEngine(const QMap<QString, QVariant> &parameters,
        QGeoServiceProvider::Error *error,
        QString *errorString) const

{
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    return 0;
}

QTM_END_NAMESPACE
