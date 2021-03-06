/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#ifndef CNT_SYMBIAN_FILTER_SQL_H
#define CNT_SYMBIAN_FILTER_SQL_H

#ifdef SYMBIAN_BACKEND_USE_CNTMODEL_V2

#include "cntabstractcontactfilter.h"
#include "qcontactmanagerengine.h"

class CContactDatabase;
class CntFilterAbstract;
class CntTransformContact;
class CntSymbianSrvConnection;
class CntDbInfo;
QTM_USE_NAMESPACE
class CntSymbianFilter : public CntAbstractContactFilter
{
public:
    CntSymbianFilter(QContactManagerEngine& manager, CContactDatabase& contactDatabase, CntSymbianSrvConnection &srvConnection, const CntTransformContact &transformContact);
    ~CntSymbianFilter();

    /* from CntAbstractContactFilter */
    QList<QContactLocalId> contacts(
            const QContactFilter& filter,
            const QList<QContactSortOrder>& sortOrders,
            bool &filterSupportedflag,
            QContactManager::Error* error) ;
    
    void initializeFilters();
  
    bool filterSupported(const QContactFilter& filter) ;
protected:
    void createSelectQuery(const QContactFilter& /*detailFilter*/,
                                  QString& /*sqlQuery*/,
                                  QContactManager::Error* /*error*/){};
private:
    CContactDatabase& m_contactDatabase;
    CntDbInfo* m_dbInfo;
    const CntTransformContact &m_transformContact;
    CntSymbianSrvConnection& m_srvConnection;
    QMap<QContactFilter::FilterType, CntAbstractContactFilter*> m_filterMap; 

};

#endif /* SYMBIAN_BACKEND_USE_CNTMODEL_V2 */

#endif /* CNT_SYMBIAN_FILTER_SQL_H */
