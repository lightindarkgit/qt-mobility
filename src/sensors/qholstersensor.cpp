/****************************************************************************
**
** Copyright (C) 2012 Research In Motion <blackberry-qt@qnx.com>
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
#include <qholstersensor.h>
#include "qholstersensor_p.h"

QTM_BEGIN_NAMESPACE

IMPLEMENT_READING(QHolsterReading)

/*!
    \class QHolsterReading
    \ingroup sensors_reading
    \inmodule QtSensors

    \brief The QHolsterReading class holds readings from the holster sensor.

    \section2 QHolsterReading Units
    The holster sensor can detect if a device is holstered or not. A holster is a pocket,
    similar to a gun holster, specifically made for the device. If the device is inside of this
    pocket, it is holstered. The holster supports the device in detecting whether the device is
    holstered or not.
*/

/*!
    \property QHolsterReading::holstered
    \brief A value indicating whether the device is holstered.

    \sa {QHolsterReading Units}
*/

bool QHolsterReading::holstered() const
{
    return d->holstered;
}

/*!
    Sets the holstered value to \a holstered.
*/
void QHolsterReading::setHolstered(bool holstered)
{
    d->holstered = holstered;
}

// =====================================================================

/*!
    \class QHolsterFilter
    \ingroup sensors_filter
    \inmodule QtSensors

    \brief The QHolsterFilter class is a convenience wrapper around QSensorFilter.

    The only difference is that the filter() method features a pointer to QHolsterReading
    instead of QSensorReading.
*/

/*!
    \fn QHolsterFilter::filter(QHolsterReading *reading)

    Called when \a reading changes. Returns false to prevent the reading from propagating.

    \sa QSensorFilter::filter()
*/

char const * const QHolsterSensor::type("QHolsterSensor");

/*!
    \class QHolsterSensor
    \ingroup sensors_type
    \inmodule QtSensors

    \brief The QHolsterSensor class is a convenience wrapper around QSensor.

    The only behavioural difference is that this class sets the type properly.

    This class also features a reading() function that returns a QHolsterReading instead of a QSensorReading.

    For details about how the sensor works, see \l QHolsterReading.

    \sa QHolsterReading
*/

/*!
    Construct the sensor as a child of \a parent.
*/
QHolsterSensor::QHolsterSensor(QObject *parent)
    : QSensor(QHolsterSensor::type, parent)
{
}

/*!
    Destroy the sensor. Stops the sensor if it has not already been stopped.
*/
QHolsterSensor::~QHolsterSensor()
{
}

/*!
    \fn QHolsterSensor::reading() const

    Returns the reading class for this sensor.

    \sa QSensor::reading()
*/

#include "moc_qholstersensor.cpp"
QTM_END_NAMESPACE