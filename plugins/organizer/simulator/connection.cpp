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

#include "connection_p.h"

#include "qorganizerdata_simulator_p.h"
#include "qorganizersimulator_p.h"

#include <mobilityconnection_p.h>
#include <private/qsimulatordata_p.h>

#include <QtNetwork/QLocalSocket>

QTM_USE_NAMESPACE
using namespace QtSimulatorPrivate;
using namespace Simulator;

Connection::Connection(MobilityConnection *mobilityCon)
    : QObject(mobilityCon)
    , mConnection(mobilityCon)
    , mRegisteredWithSimulator(false)
    , mInitialDataReceived(false)
    , mManager(QLatin1String("simulator"))
{
    qt_registerOrganizerTypes();

    mobilityCon->addMessageHandler(this);
}

Connection *Connection::instance()
{
    static Connection *connection = 0;

    if (!connection)
        connection = new Connection(MobilityConnection::instance());
    return connection;
}

void Connection::setEngine(QOrganizerSimulatorEngine *engine)
{
    Q_ASSERT(!mRegisteredWithSimulator);

    mEngine = engine;

    RemoteMetacall<void>::call(mConnection->sendSocket(), NoSync, "setRequestsOrganizer");
    mRegisteredWithSimulator = true;

    getInitialData();
}

QLocalSocket *Connection::sendSocket()
{
    return mConnection->sendSocket();
}

void Connection::translateItemIds(QOrganizerItem *item, const QString &managerUri, const LocalIdTranslation &idTranslation)
{
    // translate the main id
    {
        const QOrganizerItemLocalId previousLocalId = item->localId();
        QOrganizerItemId id;
        if (idTranslation.items.contains(previousLocalId)) {
            id.setManagerUri(managerUri);
            id.setLocalId(idTranslation.items.value(previousLocalId));
        }
        item->setId(id);
    }

    // translate the collection id
    {
        QOrganizerCollectionLocalId previousLocalId = item->collectionId().localId();
        QOrganizerCollectionId id;
        if (idTranslation.collections.contains(previousLocalId)) {
            id.setManagerUri(managerUri);
            id.setLocalId(idTranslation.collections.value(previousLocalId));
        }
        item->setCollectionId(id);
    }

    // translate any ids and localids appearing in details
    foreach (const QOrganizerItemDetail &detail, item->details()) {
        QOrganizerItemDetail detailCopy(detail);
        QMapIterator<QString, QVariant> it(detail.variantValues());
        bool modified = false;
        while (it.hasNext()) {
            it.next();
            const QVariant &value = it.value();
            const int type = value.userType();
            if (type == qMetaTypeId<QOrganizerItemLocalId>()) {
                QOrganizerItemLocalId oldId = value.value<QOrganizerItemLocalId>();
                detailCopy.setValue(it.key(), QVariant::fromValue(idTranslation.items.value(oldId)));
                modified = true;
            }
            if (type == qMetaTypeId<QOrganizerItemId>()) {
                QOrganizerItemId id = value.value<QOrganizerItemId>();
                id.setManagerUri(managerUri);
                id.setLocalId(idTranslation.items.value(id.localId()));
                detailCopy.setValue(it.key(), QVariant::fromValue(id));
                modified = true;
            }
        }
        if (modified) {
            item->saveDetail(&detailCopy);
        }
    }

    // ### Any other ids in an item?
}

void Connection::translateCollectionIds(QOrganizerCollection *collection, const QString &managerUri, const LocalIdTranslation &idTranslation)
{
    // translate the main id
    const QOrganizerCollectionLocalId previousLocalId = collection->localId();
    QOrganizerCollectionId id;
    if (idTranslation.collections.contains(previousLocalId)) {
        id.setManagerUri(managerUri);
        id.setLocalId(idTranslation.collections.value(previousLocalId));
    }
    collection->setId(id);

    // ### ids in metadata
}

void Connection::getInitialData()
{
    mInitialDataReceived = false;
    RemoteMetacall<void>::call(mConnection->sendSocket(), NoSync, "triggerOrganizerDataResend");

    // blockingly handle all remote calls until the simulator signals the initial data
    // sending is done
    QByteArray readBuffer;
    QLocalSocket *readSocket = mConnection->receiveSocket();
    while (!mInitialDataReceived) {
        readSocket->waitForReadyRead(100);
        readBuffer += readSocket->readAll();
        forever {
            IncomingRemoteMetacall rpc;
            if (rpc.read(&readBuffer)) {
                if (rpc.call(readSocket, this))
                    continue;
                qWarning("Ignoring a call to %s,", rpc.signature().data());
            }
            break;
        }
    }
}

void Connection::setOrganizerManagerUri(QString uri)
{
    mManagerUri = uri;
}

void Connection::initialOrganizerDataSent()
{
    mInitialDataReceived = true;
}

void Connection::clearOrganizerData()
{
    mLocalToRemote.items.clear();
    mRemoteToLocal.items.clear();
    mLocalToRemote.collections.clear();
    mRemoteToLocal.collections.clear();

    // map default collections to each other
    QOrganizerCollectionLocalId defaultCollectionId = mManager.defaultCollection().localId();
    mLocalToRemote.collections.insert(defaultCollectionId, defaultCollectionId);
    mRemoteToLocal.collections.insert(defaultCollectionId, defaultCollectionId);

    mNotifySimulator = false;

    // items
    mManager.removeItems(mManager.itemIds());

    // collections
    foreach (const QOrganizerCollection &collection, mManager.collections())
        mManager.removeCollection(collection.localId());

    mNotifySimulator = true;
}

void Connection::saveOrganizerItem(QtMobility::QOrganizerItem item)
{
    const QOrganizerItemLocalId remoteLocalId = item.localId();
    translateItemIds(&item, mManager.managerUri(), mRemoteToLocal);
    bool newItem = item.id().isNull();

    mNotifySimulator = false;
    //qDebug() << "Saving item" << item << " to collection " << item.collectionId();
    mManager.saveItem(&item);
    mNotifySimulator = true;
    if (mManager.error()) {
        qDebug() << "Error saving item:" << mManager.error();
        return;
    }

    // if this is a new item, save the new id in the maps
    if (newItem) {
        mRemoteToLocal.items.insert(remoteLocalId, item.localId());
        mLocalToRemote.items.insert(item.localId(), remoteLocalId);
    }
}

void Connection::removeOrganizerItem(Simulator::OrganizerItemId id)
{
    QOrganizerItemLocalId remoteLocalId = id.id.localId();
    if (!mRemoteToLocal.items.contains(remoteLocalId))
        return;

    QOrganizerItemLocalId localId = mRemoteToLocal.items.value(remoteLocalId);
    mRemoteToLocal.items.remove(remoteLocalId);
    mLocalToRemote.items.remove(localId);

    mNotifySimulator = false;
    mManager.removeItem(localId);
    mNotifySimulator = true;
}

void Connection::saveOrganizerCollection(QtMobility::QOrganizerCollection collection)
{
    const QOrganizerCollectionLocalId remoteLocalId = collection.localId();
    translateCollectionIds(&collection, mManager.managerUri(), mRemoteToLocal);
    bool newItem = collection.id().isNull();

    mNotifySimulator = false;
    //qDebug() << "Saving collection" << collection << " original id " << remoteLocalId;
    mManager.saveCollection(&collection);
    mNotifySimulator = true;
    if (mManager.error())
        qDebug() << "Error saving collection:" << mManager.error();

    // if this is a new collection, save the new id in the maps
    if (newItem) {
        mRemoteToLocal.collections.insert(remoteLocalId, collection.localId());
        mLocalToRemote.collections.insert(collection.localId(), remoteLocalId);
    }
}

void Connection::removeOrganizerCollection(Simulator::OrganizerCollectionId id)
{
    QOrganizerCollectionLocalId remoteLocalId = id.id.localId();
    if (!mRemoteToLocal.collections.contains(remoteLocalId))
        return;

    QOrganizerCollectionLocalId localId = mRemoteToLocal.collections.value(remoteLocalId);
    mRemoteToLocal.collections.remove(remoteLocalId);
    mLocalToRemote.collections.remove(localId);

    mNotifySimulator = false;
    //qDebug() << "Removing collection" << id.localId() << localId;
    mManager.removeCollection(localId);
    mNotifySimulator = true;
}

void Connection::saveOrganizerDetailDefinition(QtMobility::QOrganizerItemDetailDefinition definition, QString itemType)
{
    QOrganizerItemDetailDefinition existing = mManager.detailDefinition(definition.name(), itemType);
    if (existing != definition) {
        mNotifySimulator = false;
        //qDebug() << "Saving" << definition.name() << itemType;
        mManager.saveDetailDefinition(definition, itemType);
        mNotifySimulator = true;
    }
}

void Connection::removeOrganizerDetailDefinition(QString definitionName, QString itemType)
{
    mNotifySimulator = false;
    //qDebug() << "Removing" << definitionName << itemType;
    mManager.removeDetailDefinition(definitionName, itemType);
    mNotifySimulator = true;
}

#include "moc_connection_p.cpp"