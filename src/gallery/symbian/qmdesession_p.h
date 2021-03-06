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

#ifndef QMDESESSION_P_H
#define QMDESESSION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qmobilityglobal.h>

#include "qgalleryqueryrequest.h"
#include "qmdegalleryresultset_p.h"

#include <QObject>
#include <QEventLoop>
#include <mdesession.h>
#include <mdequery.h>
#include <e32std.h>

QTM_BEGIN_NAMESPACE

class QGalleryAbstractResponse;

class QMdeSession : public QObject, public MMdESessionObserver
{
    Q_OBJECT
public:

    QMdeSession(QObject *parent = 0);
    virtual ~QMdeSession();

public: // From MMdESessionObserver
    /**
     * For checking MdE initialization status
     * @param aSession  MdE Session which was opened
     * @param aError  Error code from the opening
     */
    void HandleSessionOpened( CMdESession &aSession, TInt aError );

    /**
     * For checking MdE session errors
     * @param aSession  MdE Session which was opened
     * @param aError  Error which has occurred
     */
    void HandleSessionError( CMdESession &aSession, TInt aError );

    CMdENamespaceDef& GetDefaultNamespaceDefL();

    CMdEObject* GetFullObjectL( const unsigned int id );

    CMdEObject* OpenObjectL( const unsigned int id, CMdEObjectDef& objectDef );
    void CommitObjectL( CMdEObject& object );

    CMdEObjectQuery* NewObjectQueryL(MMdEQueryObserver *observer,
        QGalleryQueryRequest *request,
        int &error);

    CMdEObjectQuery *NewObjectQuery(
            CMdENamespaceDef &namespaceDef,
            CMdEObjectDef &objectDef,
            MMdEQueryObserver *observer = 0);

    int RemoveObject( const unsigned int itemId );

    void AddItemAddedObserverL( MMdEObjectObserver &observer, CMdELogicCondition &condition );
    void AddItemChangedObserverL( MMdEObjectObserver &observer, RArray<TItemId> &idArray );
    void RemoveObjectObserver( MMdEObjectObserver &observer );

    void AddTrackedResultSet( QMDEGalleryResultSet* aResultSet );
    void RemoveTrackedResultSet( QMDEGalleryResultSet* aResultSet );

private:

    QEventLoop m_eventLoop;
    CMdESession *m_cmdeSession;

    RPointerArray<QMDEGalleryResultSet> m_resultSetList;
};
QTM_END_NAMESPACE

#endif // QMDESESSION_H
