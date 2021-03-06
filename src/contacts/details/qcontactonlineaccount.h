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


#ifndef QCONTACTONLINEACCOUNT_H
#define QCONTACTONLINEACCOUNT_H

#include <QString>

#include "qtcontactsglobal.h"
#include "qcontactdetail.h"
#include "qcontact.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_CONTACTS_EXPORT QContactOnlineAccount : public QContactDetail
{
public:
#ifdef Q_QDOC
    static const QLatin1Constant DefinitionName;
    static const QLatin1Constant FieldAccountUri;
    static const QLatin1Constant FieldServiceProvider;
    static const QLatin1Constant FieldProtocol;
    static const QLatin1Constant FieldCapabilities;
    static const QLatin1Constant FieldSubTypes;
    static const QLatin1Constant SubTypeSip;
    static const QLatin1Constant SubTypeSipVoip;
    static const QLatin1Constant SubTypeImpp;
    static const QLatin1Constant SubTypeVideoShare;
    static const QLatin1Constant ProtocolAim;
    static const QLatin1Constant ProtocolIcq;
    static const QLatin1Constant ProtocolIrc;
    static const QLatin1Constant ProtocolJabber;
    static const QLatin1Constant ProtocolMsn;
    static const QLatin1Constant ProtocolQq;
    static const QLatin1Constant ProtocolSkype;
    static const QLatin1Constant ProtocolYahoo;
#else
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactOnlineAccount, "OnlineAccount")
    Q_DECLARE_LATIN1_CONSTANT(FieldAccountUri, "AccountUri");
    Q_DECLARE_LATIN1_CONSTANT(FieldServiceProvider, "ServiceProvider");
    Q_DECLARE_LATIN1_CONSTANT(FieldProtocol, "Protocol");
    Q_DECLARE_LATIN1_CONSTANT(FieldCapabilities, "Capabilities");
    Q_DECLARE_LATIN1_CONSTANT(FieldSubTypes, "SubTypes");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeSip, "Sip");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeSipVoip, "SipVoip");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeImpp, "Impp");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeVideoShare, "VideoShare");

    // These strings are taken from
    // http://telepathy.freedesktop.org/spec/Connection_Manager.html#Simple-Type:Protocol
    Q_DECLARE_LATIN1_CONSTANT(ProtocolAim, "aim");
    Q_DECLARE_LATIN1_CONSTANT(ProtocolIcq, "icq");
    Q_DECLARE_LATIN1_CONSTANT(ProtocolIrc, "irc");
    Q_DECLARE_LATIN1_CONSTANT(ProtocolJabber, "jabber");
    Q_DECLARE_LATIN1_CONSTANT(ProtocolMsn, "msn");
    Q_DECLARE_LATIN1_CONSTANT(ProtocolQq, "qq");
    Q_DECLARE_LATIN1_CONSTANT(ProtocolSkype, "skype");
    Q_DECLARE_LATIN1_CONSTANT(ProtocolYahoo, "yahoo");
#endif

    void setAccountUri(const QString& accountUri) {setValue(FieldAccountUri, accountUri);}
    QString accountUri() const {return value(FieldAccountUri);}

    void setServiceProvider(const QString& serviceProvider) {setValue(FieldServiceProvider, serviceProvider);}
    QString serviceProvider() const {return value(FieldServiceProvider);}

    void setProtocol(const QString& protocol) {setValue(FieldProtocol, protocol);}
    QString protocol() const {return value(FieldProtocol);}

    void setCapabilities(const QStringList& capabilities) {setValue(FieldCapabilities, capabilities);}
    QStringList capabilities() const {return value<QStringList>(FieldCapabilities);}

    void setSubTypes(const QStringList& subTypes) {setValue(FieldSubTypes, subTypes);}
    void setSubTypes(const QString& subType) {setValue(FieldSubTypes, QStringList(subType));}
    QStringList subTypes() const {return value<QStringList>(FieldSubTypes);}
};

QTM_END_NAMESPACE

#endif
