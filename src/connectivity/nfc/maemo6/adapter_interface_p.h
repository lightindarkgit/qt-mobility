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

/*
 * This file was generated by qdbusxml2cpp version 0.7 and then hand edited
 * Command line was: qdbusxml2cpp -p adapter_interface_p.h:adapter_interface.cpp com.nokia.nfc.Adapter.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 */

#ifndef ADAPTER_INTERFACE_P_H
#define ADAPTER_INTERFACE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.nokia.nfc.Adapter
 */
class ComNokiaNfcAdapterInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.nokia.nfc.Adapter"; }

public:
    ComNokiaNfcAdapterInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~ComNokiaNfcAdapterInterface();

    Q_PROPERTY(bool Discovering READ discovering)
    inline bool discovering() {
        QDBusReply<QVariantMap> reply = GetProperties();
        if (!reply.isValid())
            return false;

        return reply.value().value(QLatin1String("Discovering")).toBool();
    }

    Q_PROPERTY(QString State READ state WRITE setState)
    inline QString state() {
        QDBusReply<QVariantMap> reply = GetProperties();
        if (!reply.isValid())
            return QString();

        return reply.value().value(QLatin1String("State")).toString();
    }

    inline void setState(const QString &value)
    { SetProperty(QLatin1String("State"), QDBusVariant(qVariantFromValue(value))); }

    Q_PROPERTY(QStringList TagTechnologies READ tagTechnologies)
    inline QStringList tagTechnologies()
    {
        QDBusReply<QVariantMap> reply = GetProperties();
        if (!reply.isValid())
            return QStringList();

        return reply.value().value(QLatin1String("TagTechnologies")).toStringList();
    }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> CancelAccessRequest(const QDBusObjectPath &in0, const QString &in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QLatin1String("CancelAccessRequest"), argumentList);
    }

    inline QDBusPendingReply<> CancelHandoverRequest(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QLatin1String("CancelHandoverRequest"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetProperties()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetProperties"), argumentList);
    }

    inline QDBusPendingReply<> RegisterUIAgent(const QDBusObjectPath &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QLatin1String("RegisterUIAgent"), argumentList);
    }

    inline QDBusPendingReply<> RequestAccess(const QDBusObjectPath &in0, const QString &in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QLatin1String("RequestAccess"), argumentList);
    }

    inline QDBusPendingReply<> RequestHandover(const QDBusObjectPath &in0, const QString &in1, const QString &in2)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1) << QVariant::fromValue(in2);
        return asyncCallWithArgumentList(QLatin1String("RequestHandover"), argumentList);
    }

    inline QDBusPendingReply<> SetProperty(const QString &in0, const QDBusVariant &in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QLatin1String("SetProperty"), argumentList);
    }

    inline QDBusPendingReply<> UnregisterUIAgent(const QDBusObjectPath &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QLatin1String("UnregisterUIAgent"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void HandoverCarrierSelected(const QString &in0);
    void HandoverCompleted(const QString &in0, const QDBusVariant &in1);
    void HandoverFailed(const QString &in0);
    void HandoverStarted();
    void PropertyChanged(const QString &in0, const QDBusVariant &in1);
    void TargetDetected(const QDBusObjectPath &in0);
    void TargetLost(const QDBusObjectPath &in0);
};

namespace com {
  namespace nokia {
    namespace nfc {
      typedef ::ComNokiaNfcAdapterInterface Adapter;
    }
  }
}
#endif
