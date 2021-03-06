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

#include "qorganizeritemoccurrencefetchrequest.h"
#include "qorganizeritemrequests_p.h"

QTM_BEGIN_NAMESPACE

/*!
  \class QOrganizerItemOccurrenceFetchRequest
  \brief The QOrganizerItemOccurrenceFetchRequest class allows a client to asynchronously
    request occurrences generated by a recurring item.
  \inmodule QtOrganizer
  \since 1.1

  For a QOrganizerItemOccurrenceFetchRequest, the resultsAvailable() signal will be emitted when the resultant
  organizer items (which may be retrieved by calling items()), are updated, as well as if
  the overall operation error (which may be retrieved by calling error()) is updated.

  \ingroup organizer-requests
 */

/*! Constructs a new organizer item fetch request whose parent is the specified \a parent
    \since 1.1
*/
QOrganizerItemOccurrenceFetchRequest::QOrganizerItemOccurrenceFetchRequest(QObject* parent)
    : QOrganizerAbstractRequest(new QOrganizerItemOccurrenceFetchRequestPrivate, parent)
{
}

/*! Frees memory in use by this request
    \since 1.2
*/
QOrganizerItemOccurrenceFetchRequest::~QOrganizerItemOccurrenceFetchRequest()
{
    QOrganizerAbstractRequestPrivate::notifyEngine(this);
}

/*! Sets the parent item, whose occurrences are to be fetched to \a item.
    \since 1.1
*/
void QOrganizerItemOccurrenceFetchRequest::setParentItem(const QOrganizerItem &item)
{
    Q_D(QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_generator = item;
}

/*! Sets the start date of the request to \a date.  All occurrences fetched will have a date on or
 * after \a date.
   \since 1.1
*/
void QOrganizerItemOccurrenceFetchRequest::setStartDate(const QDateTime &date)
{
    Q_D(QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_startDate = date;
}

/*! Sets the end period of the request to \a date.  All occurrences fetched will have a date on or
 * before \a date.
   \since 1.1
*/
void QOrganizerItemOccurrenceFetchRequest::setEndDate(const QDateTime &date)
{
    Q_D(QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_endDate = date;
}

/*! Sets the maximum number of items to fetch to \a maxCount.

  A negative value denotes that no limit will be imposed on the number of items to fetch.
   \since 1.1
 */
void QOrganizerItemOccurrenceFetchRequest::setMaxOccurrences(int maxCount)
{
    Q_D(QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_maxOccurrences = maxCount;
}

/*! Sets the fetch hint which the manager can use to optimize occurrence retrieval to \a hint.
    The fetch hint may be ignored by the manager, in which case each occurrence will include
    all available information.
   \since 1.1
*/
void QOrganizerItemOccurrenceFetchRequest::setFetchHint(const QOrganizerItemFetchHint& hint)
{
    Q_D(QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_fetchHint = hint;
}

/*! Returns the parent item, whose occurrences are to be fetched.
   \since 1.1
*/
QOrganizerItem QOrganizerItemOccurrenceFetchRequest::parentItem() const
{
    Q_D(const QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_generator;
}

/*! Returns the start date of the request.
    \since 1.1
*/
QDateTime QOrganizerItemOccurrenceFetchRequest::startDate() const
{
    Q_D(const QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_startDate;
}

/*! Returns the end date of the request.
    \since 1.1
*/
QDateTime QOrganizerItemOccurrenceFetchRequest::endDate() const
{
    Q_D(const QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_endDate;
}

/*! Returns the maximum number of items to fetch.

  A negative value denotes that no limit will be imposed on the number of items to fetch.

  The default value is -1.
  \since 1.1
 */
int QOrganizerItemOccurrenceFetchRequest::maxOccurrences() const
{
    Q_D(const QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_maxOccurrences;
}

/*! Returns the list of organizer item occurrences retrieved by this request
    \since 1.1
*/
QList<QOrganizerItem> QOrganizerItemOccurrenceFetchRequest::itemOccurrences() const
{
    Q_D(const QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_organizeritems;
}

/*! Returns the fetch hint associated with the request
    \since 1.1
*/
QOrganizerItemFetchHint QOrganizerItemOccurrenceFetchRequest::fetchHint() const
{
    Q_D(const QOrganizerItemOccurrenceFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_fetchHint;
}

#include "moc_qorganizeritemoccurrencefetchrequest.cpp"

QTM_END_NAMESPACE
