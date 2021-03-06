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
#ifndef ORGANIZERSYMBIANCOLLECTION_H_
#define ORGANIZERSYMBIANCOLLECTION_H_

#include <calchangecallback.h>
#include <calprogresscallback.h>
#include "qorganizercollection.h"

QTM_USE_NAMESPACE

QTM_BEGIN_NAMESPACE
class QOrganizerManagerEngine;
QTM_END_NAMESPACE
class CCalSession;
class CCalEntryView;
class CCalInstanceView;
class CCalCalendarInfo;

class OrganizerSymbianCollectionPrivate : public QSharedData,
                                          public MCalProgressCallBack,
                                          public MCalChangeCallBack2
{
public:
    OrganizerSymbianCollectionPrivate();
    ~OrganizerSymbianCollectionPrivate();
    
    void Progress(TInt aPercentageCompleted);
    void Completed(TInt aError);
    TBool NotifyProgress();
    void CalChangeNotification(RArray<TCalChangeEntry>& aChangeItems);
    
    QOrganizerManagerEngine *m_engine;
    CCalSession *m_calSession;
    CCalEntryView *m_calEntryView;
    CCalInstanceView *m_calInstanceView;
    QOrganizerCollectionId m_id;
    quint64 m_calCollectionId;
    CActiveSchedulerWait *m_activeSchedulerWait;
    QString m_fileName;
    int m_error;
};

class OrganizerSymbianCollection
{
public:
    OrganizerSymbianCollection();
    OrganizerSymbianCollection(QOrganizerManagerEngine *engine);
    OrganizerSymbianCollection(const OrganizerSymbianCollection &other);
    
#ifdef SYMBIAN_CALENDAR_V2
    void openL(const TDesC &fileName, const CCalCalendarInfo *calInfo = 0);
#else
    void openL(const TDesC &fileName);
#endif
    void createViewsL();
    QOrganizerCollectionId id() const;
    quint64 calCollectionId() const;
    CCalSession *calSession() const; 
    CCalEntryView *calEntryView() const;
    CCalInstanceView *calInstanceView() const;
    QString fileName() const;
    bool isValid() const;
    bool isMarkedForDeletionL() const;
    QOrganizerCollection toQOrganizerCollectionL() const;
    
	// Custom metadata keys for a collection
    Q_DECLARE_LATIN1_CONSTANT(KeyIsValid, "IsValid");
    Q_DECLARE_LATIN1_CONSTANT(KeyFileName, "FileName");
    Q_DECLARE_LATIN1_CONSTANT(KeyEnabled, "Enabled");
    Q_DECLARE_LATIN1_CONSTANT(KeyFolderLUID, "FolderLUID");
    Q_DECLARE_LATIN1_CONSTANT(KeyCreationTime, "CreationTime");
    Q_DECLARE_LATIN1_CONSTANT(KeyModificationTime, "ModificationTime");
    Q_DECLARE_LATIN1_CONSTANT(KeySyncStatus, "SyncStatus");
    Q_DECLARE_LATIN1_CONSTANT(KeyIsSharedFolder, "IsSharedFolder");
    Q_DECLARE_LATIN1_CONSTANT(KeyGlobalUUID, "GlobalUUID");
    Q_DECLARE_LATIN1_CONSTANT(KeyDeviceSyncServiceOwner, "DeviceSyncServiceOwner");
    Q_DECLARE_LATIN1_CONSTANT(KeyOwnerName, "OwnerName");
    Q_DECLARE_LATIN1_CONSTANT(KeyMarkAsDelete, "MarkAsDelete");
    
private:
    QExplicitlySharedDataPointer<OrganizerSymbianCollectionPrivate> d;
};

#endif // ORGANIZERSYMBIANCOLLECTION_H_
