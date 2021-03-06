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
#include "organizeritemguidtransform.h"
#include "qorganizeritemguid.h"

const TInt KGuidLength = 30;

OrganizerItemGuidTransform::OrganizerItemGuidTransform()
{
    // Set seed for qrand()
    TTime homeTime;
    homeTime.HomeTime();
    uint seed = (uint) homeTime.Int64();
    qsrand(seed);
}

void OrganizerItemGuidTransform::transformToDetailL(const CCalEntry& entry, QOrganizerItem *item)
{
    QString guid = toQString(entry.UidL());
    if (!guid.isEmpty())
        item->setGuid(guid);
}

void OrganizerItemGuidTransform::transformToDetailPostSaveL(const CCalEntry& entry, QOrganizerItem *item)
{
    transformToDetailL(entry, item);
}

void OrganizerItemGuidTransform::transformToEntryL(const QOrganizerItem& item, CCalEntry* entry)
{
    Q_UNUSED(item);
    Q_UNUSED(entry);
    // Not used. Guid is already set when CCalEntry was created.
}

QString OrganizerItemGuidTransform::detailDefinitionName()
{
    return QOrganizerItemGuid::DefinitionName;    
}

HBufC8 *OrganizerItemGuidTransform::guidLC(const QOrganizerItem &item)
{
    // Read guid from organizer item
    QString globalUidString = item.guid();

    if (globalUidString.isEmpty()) {
        // Generate a new quid
        // TODO: is this long enough for us? We could add imei or second random number to it...
        HBufC8* globalUidBuf = HBufC8::NewLC(KGuidLength);
        TPtr8 ptr = globalUidBuf->Des();
        ptr.Num(qrand());
        return globalUidBuf; // ownership passed
    } else {
        // Use the given guid
        HBufC8* globalUidBuf = HBufC8::NewLC(globalUidString.length());
        TPtr8 ptr = globalUidBuf->Des();
        QByteArray bytes = globalUidString.toUtf8();
        ptr = toPtrC8(bytes);
        return globalUidBuf; // ownership passed
    }
}
