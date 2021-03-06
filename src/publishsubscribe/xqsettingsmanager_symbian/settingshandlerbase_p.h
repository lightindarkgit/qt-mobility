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

#ifndef SETTINGSHANDLERBASE_H
#define SETTINGSHANDLERBASE_H

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

// INCLUDES
#include <QVariant>
#include "xqsettingsmanager.h"
#include <e32base.h>

// FORWARD DECLARATIONS
class XQSettingsKey;

class MSettingsHandlerObserver
{
public:
    virtual void valueChanged(const XQSettingsKey& key, XQSettingsManager::Type type) = 0;
    virtual void itemDeleted(const XQSettingsKey& key) = 0;
};

class SettingsHandlerBase
{
public:
    virtual ~SettingsHandlerBase() {};
    virtual void setObserver(MSettingsHandlerObserver* observer) = 0;
    QVariant handleReadItemValue(const XQSettingsKey& key, XQSettingsManager::Type type, TInt& error);
    bool handleWriteItemValue(const XQSettingsKey& key, const QVariant& value, TInt& error);
    virtual bool handleStartMonitoring(const XQSettingsKey& key, XQSettingsManager::Type type, MSettingsHandlerObserver& observer, TInt& error) = 0;
    virtual bool handleStopMonitoring(const XQSettingsKey& key, TInt& error) = 0;

protected:
    virtual TInt getValue(unsigned long key, TInt& value) = 0;
    virtual TInt getValue(unsigned long key, TReal& value) = 0;
    virtual void getValueL(unsigned long key, RBuf8& value) = 0;
    virtual void getValueL(unsigned long key, RBuf16& value) = 0;

    virtual TInt setValue(unsigned long key, const TInt& value) = 0;
    virtual TInt setValue(unsigned long key, const TReal& value) = 0;
    virtual TInt setValue(unsigned long key, const TDesC8& value) = 0;
    virtual TInt setValue(unsigned long key, const TDesC16& value) = 0;
};

#endif //SETTINGSHANDLERBASE_H
