/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "genericrotationsensor.h"
#include <QDebug>
#include <qmath.h>

#define RADIANS_TO_DEGREES 57.2957795

const char *genericrotationsensor::id("generic.rotation");

genericrotationsensor::genericrotationsensor(QSensor *sensor)
    : QSensorBackend(sensor)
{
    accelerometer = new QAccelerometer(this);
    accelerometer->addFilter(this);
    accelerometer->connect();

    setReading<QRotationReading>(&m_reading);
    setDataRates(accelerometer);

    sensor->setProperty("hasZ", false);
}

void genericrotationsensor::start()
{
    accelerometer->setUpdateInterval(sensor()->updateInterval());
    accelerometer->start();
    if (!accelerometer->isActive())
        sensorStopped();
    if (accelerometer->isBusy())
        sensorBusy();
}

void genericrotationsensor::stop()
{
    accelerometer->stop();
}

bool genericrotationsensor::filter(QSensorReading *reading)
{
    QAccelerometerReading *ar = qobject_cast<QAccelerometerReading*>(reading);
    qreal pitch = 0;
    qreal roll = 0;

    qreal x = ar->x();
    qreal y = ar->y();
    qreal z = ar->z();

    // Note that the formula used come from this document:
    // http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
    pitch = qAtan(y / sqrt(x*x + z*z)) * RADIANS_TO_DEGREES;
    roll = qAtan(x / sqrt(y*y + z*z)) * RADIANS_TO_DEGREES;
    // Roll is a left-handed rotation but we need right-handed rotation
    roll = -roll;

    // We need to fix up roll to the (-180,180] range required.
    // Check for negative theta values and apply an offset as required.
    // Note that theta is defined as the angle of the Z axis relative
    // to gravity (see referenced document). It's negative when the
    // face of the device points downward.
    qreal theta = qAtan(sqrt(x*x + y*y) / z) * RADIANS_TO_DEGREES;
    if (theta < 0) {
        if (roll > 0)
            roll = 180 - roll;
        else
            roll = -180 - roll;
    }

    m_reading.setTimestamp(ar->timestamp());
    m_reading.setX(pitch);
    m_reading.setY(roll);
    m_reading.setZ(0);
    newReadingAvailable();
    return false;
}
