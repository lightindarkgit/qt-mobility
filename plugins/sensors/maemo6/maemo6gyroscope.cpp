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

#include "maemo6gyroscope.h"

char const * const maemo6gyroscope::id("maemo6.gyroscope");
const float maemo6gyroscope::MILLI = 0.001;
bool maemo6gyroscope::m_initDone = false;

maemo6gyroscope::maemo6gyroscope(QSensor *sensor)
    : maemo6sensorbase(sensor)
{
//    initSensor<GyroscopeSensorChannelInterface>(m_initDone);
    setDescription(QLatin1String("angular velocities around x, y, and z axis in degrees per second"));
    setRanges(MILLI);
    setReading<QGyroscopeReading>(&m_reading);
}

void maemo6gyroscope::slotDataAvailable(const XYZ& data)
{
    m_reading.setX((qreal)(data.x()*MILLI));
    m_reading.setY((qreal)(data.y()*MILLI));
    m_reading.setZ((qreal)(data.z()*MILLI));
    m_reading.setTimestamp(data.XYZData().timestamp_);
    newReadingAvailable();
}

void maemo6gyroscope::slotFrameAvailable(const QVector<XYZ>&  frame)
{
    for (int i=0, l=frame.size(); i<l; i++){
        slotDataAvailable(frame.at(i));
    }
}

bool maemo6gyroscope::doConnect(){
    if (m_bufferSize==1){
        QObject::disconnect(m_sensorInterface, SIGNAL(frameAvailable(const QVector<XYZ>& )));
        if (!(QObject::connect(m_sensorInterface, SIGNAL(dataAvailable(const XYZ&)),
                               this, SLOT(slotDataAvailable(const XYZ&))))){
            qWarning() << "Unable to connect "<< sensorName();
            return false;
        }
        return true;
    }
    QObject::disconnect(m_sensorInterface, SIGNAL(dataAvailable(const XYZ&)));
    if (!(QObject::connect(m_sensorInterface,SIGNAL(frameAvailable(const QVector<XYZ>& )),
                           this, SLOT(slotFrameAvailable(const QVector<XYZ>& ))))){
        qWarning() << "Unable to connect "<< sensorName();
        return false;
    }
    return true;
}

const QString maemo6gyroscope::sensorName(){
    return "gyroscopesensor";
}