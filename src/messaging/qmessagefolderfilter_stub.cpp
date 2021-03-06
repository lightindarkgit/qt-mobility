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
#include "qmessagefolderfilter.h"
#include "qmessagefolderfilter_p.h"

QTM_BEGIN_NAMESPACE

QMessageFolderFilter::QMessageFolderFilter()
{
}

QMessageFolderFilter::QMessageFolderFilter(const QMessageFolderFilter &other)
{
    Q_UNUSED(other)
}

QMessageFolderFilter::~QMessageFolderFilter()
{
}

QMessageFolderFilter& QMessageFolderFilter::operator=(const QMessageFolderFilter& other)
{
    Q_UNUSED(other)
    return *this; // stub
}

void QMessageFolderFilter::setMatchFlags(QMessageDataComparator::MatchFlags matchFlags)
{
    Q_UNUSED(matchFlags)
}

QMessageDataComparator::MatchFlags QMessageFolderFilter::matchFlags() const
{
    return 0;
}

bool QMessageFolderFilter::isEmpty() const
{
    return false; // stub
}

bool QMessageFolderFilter::isSupported() const
{
    return true; // TODO: Implement
}

QMessageFolderFilter QMessageFolderFilter::operator~() const
{
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::operator&(const QMessageFolderFilter& other) const
{
    Q_UNUSED(other)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::operator|(const QMessageFolderFilter& other) const
{
    Q_UNUSED(other)
    return QMessageFolderFilter(); // stub
}

const QMessageFolderFilter& QMessageFolderFilter::operator&=(const QMessageFolderFilter& other)
{
    Q_UNUSED(other)
    return *this;
}

const QMessageFolderFilter& QMessageFolderFilter::operator|=(const QMessageFolderFilter& other)
{
    Q_UNUSED(other)
    return *this;
}

bool QMessageFolderFilter::operator==(const QMessageFolderFilter& other) const
{
    Q_UNUSED(other)
    return false; // stub
}

QMessageFolderFilter QMessageFolderFilter::byId(const QMessageFolderId &id, QMessageDataComparator::EqualityComparator cmp)
{
    Q_UNUSED(id)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byId(const QMessageFolderIdList &ids, QMessageDataComparator::InclusionComparator cmp)
{
    Q_UNUSED(ids)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byId(const QMessageFolderFilter &filter, QMessageDataComparator::InclusionComparator cmp)
{
    Q_UNUSED(filter)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byName(const QString &value, QMessageDataComparator::EqualityComparator cmp)
{
    Q_UNUSED(value)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byName(const QString &value, QMessageDataComparator::InclusionComparator cmp)
{
    Q_UNUSED(value)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byPath(const QString &value, QMessageDataComparator::EqualityComparator cmp)
{
    Q_UNUSED(value)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byPath(const QString &value, QMessageDataComparator::InclusionComparator cmp)
{
    Q_UNUSED(value)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byParentAccountId(const QMessageAccountId &id, QMessageDataComparator::EqualityComparator cmp)
{
    Q_UNUSED(id)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byParentAccountId(const QMessageAccountFilter &filter, QMessageDataComparator::InclusionComparator cmp)
{
    Q_UNUSED(filter)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byParentFolderId(const QMessageFolderId &id, QMessageDataComparator::EqualityComparator cmp)
{
    Q_UNUSED(id)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byParentFolderId(const QMessageFolderFilter &filter, QMessageDataComparator::InclusionComparator cmp)
{
    Q_UNUSED(filter)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byAncestorFolderIds(const QMessageFolderId &id, QMessageDataComparator::InclusionComparator cmp)
{
    Q_UNUSED(id)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QMessageFolderFilter QMessageFolderFilter::byAncestorFolderIds(const QMessageFolderFilter &filter, QMessageDataComparator::InclusionComparator cmp)
{
    Q_UNUSED(filter)
    Q_UNUSED(cmp)
    return QMessageFolderFilter(); // stub
}

QTM_END_NAMESPACE
