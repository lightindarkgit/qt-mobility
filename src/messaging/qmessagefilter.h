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
#ifndef QMESSAGEFILTER_H
#define QMESSAGEFILTER_H

#include <qmessageglobal.h>
#include <qmessagedatacomparator.h>
#include <qmessageid.h>
#include <qmessageaccountid.h>
#include <qmessagefolderid.h>
#include <qmessageaccountfilter.h>
#include <qmessagefolderfilter.h>
#include <qmessage.h>

QTM_BEGIN_NAMESPACE

class QMessageFilterPrivate;

class Q_MESSAGING_EXPORT QMessageFilter
{
    friend class QMessageFilterPrivate;

public:
    QMessageFilter();
    QMessageFilter(const QMessageFilter &other);
    virtual ~QMessageFilter();

    QMessageFilter& operator=(const QMessageFilter &other);

    void setMatchFlags(QMessageDataComparator::MatchFlags matchFlags);
    QMessageDataComparator::MatchFlags matchFlags() const;

    bool isEmpty() const;
    bool isSupported() const;

    QMessageFilter operator~() const;
    QMessageFilter operator&(const QMessageFilter &other) const;
    QMessageFilter operator|(const QMessageFilter &other) const;
    const QMessageFilter& operator&=(const QMessageFilter &other);
    const QMessageFilter& operator|=(const QMessageFilter &other);

    bool operator==(const QMessageFilter &other) const;
    bool operator!=(const QMessageFilter &other) const;

    static QMessageFilter byId(const QMessageId &id, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);
    static QMessageFilter byId(const QMessageIdList &ids, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);
    static QMessageFilter byId(const QMessageFilter &filter, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);

    static QMessageFilter byType(QMessage::Type type, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);
    static QMessageFilter byType(QMessage::TypeFlags type, QMessageDataComparator::InclusionComparator cmp);

    static QMessageFilter bySender(const QString &pattern, QMessageDataComparator::LikeComparator cmp);
    static QMessageFilter bySender(const QString &value, QMessageDataComparator::EqualityComparator cmp);
    static QMessageFilter bySender(const QString &value, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);

    static QMessageFilter byRecipients(const QString &pattern, QMessageDataComparator::LikeComparator cmp);
    static QMessageFilter byRecipients(const QString &value, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);

    static QMessageFilter bySubject(const QString &pattern, QMessageDataComparator::LikeComparator cmp);
    static QMessageFilter bySubject(const QString &value, QMessageDataComparator::EqualityComparator cmp);
    static QMessageFilter bySubject(const QString &value, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);

    static QMessageFilter byTimeStamp(const QDateTime &value, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);
    static QMessageFilter byTimeStamp(const QDateTime &value, QMessageDataComparator::RelationComparator cmp);

    static QMessageFilter byReceptionTimeStamp(const QDateTime &value, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);
    static QMessageFilter byReceptionTimeStamp(const QDateTime &value, QMessageDataComparator::RelationComparator cmp);

    static QMessageFilter byStatus(QMessage::Status value, QMessageDataComparator::EqualityComparator cmp);
    static QMessageFilter byStatus(QMessage::StatusFlags mask, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);

    static QMessageFilter byPriority(QMessage::Priority priority, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);

    static QMessageFilter bySize(int value, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);
    static QMessageFilter bySize(int value, QMessageDataComparator::RelationComparator cmp);

    static QMessageFilter byParentAccountId(const QMessageAccountId &id, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);
    static QMessageFilter byParentAccountId(const QMessageAccountFilter &filter, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);

    static QMessageFilter byStandardFolder(QMessage::StandardFolder folder, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);

    static QMessageFilter byParentFolderId(const QMessageFolderId &id, QMessageDataComparator::EqualityComparator cmp = QMessageDataComparator::Equal);
    static QMessageFilter byParentFolderId(const QMessageFolderFilter &filter, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);

    static QMessageFilter byAncestorFolderIds(const QMessageFolderId &id, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);
    static QMessageFilter byAncestorFolderIds(const QMessageFolderFilter &filter, QMessageDataComparator::InclusionComparator cmp = QMessageDataComparator::Includes);

    static QMessageFilter byTo(const QString &pattern, QMessageDataComparator::LikeComparator cmp);

    static QMessageFilter byCc(const QString &pattern, QMessageDataComparator::LikeComparator cmp);

    static QMessageFilter byBcc(const QString &pattern, QMessageDataComparator::LikeComparator cmp);

private:
    QMessageFilterPrivate *d_ptr;
};

QTM_END_NAMESPACE
#endif
