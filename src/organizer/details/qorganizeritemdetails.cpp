/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qorganizeritemdetails.h"

QTM_BEGIN_NAMESPACE








/* ==================== QOrganizerItemDescription ======================= */
/*!
   \class QOrganizerItemDescription
   \ingroup organizer-details
   \brief The QOrganizerItemDescription class contains some arbitrary information which is relevant to the organizer item.
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerItemDescription::DefinitionName
   The constant string which identifies the definition of details which are long descriptions.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemDescription::DefinitionName, "Description");

/*!
   \variable QOrganizerItemDescription::FieldDescription

   The constant key for which the description value is stored in details of
   the QOrganizerItemDescription type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemDescription::FieldDescription, "Description");

/*!
   \fn QOrganizerItemDescription::setDescription(const QString& description)
   Sets a description associated with an organizer item to \a description.
 */

/*!
   \fn QOrganizerItemDescription::description() const
   Returns a string for a description associated with an organizer item.
 */







/* ==================== QOrganizerItemDisplayLabel ======================= */
/*!
   \class QOrganizerItemDisplayLabel
   \ingroup organizer-details
   \brief The QOrganizerItemDisplayLabel class contains the backend-synthesized display label of the organizer item.
   \inmodule QtOrganizer
*/

/*!
   \fn void QOrganizerItemDisplayLabel::setLabel(const QString& label)
   Sets the display label of the organizer item to \a label.
 */

/*!
   \fn QString QOrganizerItemDisplayLabel::label() const
   Returns the display label of the organizer item.
 */

/*!
   \variable QOrganizerItemDisplayLabel::DefinitionName

   The constant string which identifies the definition of details
   which contain a display label of an organizer item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemDisplayLabel::DefinitionName, "DisplayLabel");

/*!
   \variable QOrganizerItemDisplayLabel::FieldLabel

   The constant key for which the display label value is stored in
   details of the QOrganizerItemDisplayLabel type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemDisplayLabel::FieldLabel, "Label");







/* ==================== QOrganizerEventTimeRange ======================= */
/*!
   \class QOrganizerEventTimeRange
   \ingroup organizer-details
   \brief The QOrganizerEventTimeRange class contains the start and end dates and times of a recurring event series,
          or occurrence of an event.
   \inmodule QtOrganizer
*/

/*!
   \variable QOrganizerEventTimeRange::DefinitionName

   The constant string which identifies the definition of details
   which contain a start and end timestamps of an event series or event occurrence.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerEventTimeRange::DefinitionName, "EventTimeRange");

/*!
   \variable QOrganizerEventTimeRange::FieldEndDateTime

   The constant key for which the end date and time value is stored in
   details of the QOrganizerEventTimeRange type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerEventTimeRange::FieldEndDateTime, "EndDateTime");

/*!
   \variable QOrganizerEventTimeRange::FieldStartDateTime

   The constant key for which the start date and time value is stored in
   details of the QOrganizerEventTimeRange type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerEventTimeRange::FieldStartDateTime, "StartDateTime");


/*!
    \fn QOrganizerEventTimeRange::startDateTime() const
    Returns the event timerange's start date and time as QDateTime.
 */

/*!
    \fn QOrganizerEventTimeRange::setStartDateTime(const QDateTime& startDateTime)
    Sets the event timerange's start date and time to \a startDateTime.
 */

/*!
    \fn QOrganizerEventTimeRange::endDateTime() const
    Returns the event timerange's due date and time as QDateTime.
 */

/*!
    \fn QOrganizerEventTimeRange::setEndDateTime(const QDateTime& dueDateTime)
    Sets the event timerange's due date and time to \a dueDateTime.
 */

/* ==================== QOrganizerItemGuid ======================= */

/*!
   \class QOrganizerItemGuid
   \ingroup organizer-details
   \brief The QOrganizerItemGuid class contains the globally unique identifier of the organizer item, which can be used for synchronization purposes.
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerItemGuid::DefinitionName
   The constant string which identifies the definition of details which are globally unique identifiers.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemGuid::DefinitionName, "Guid");

/*!
   \variable QOrganizerItemGuid::FieldGuid

   The constant key for which the globally unique identifier value is
   stored in details of the QOrganizerItemGuid type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemGuid::FieldGuid, "Guid");

/*!
   \fn QOrganizerItemGuid::guid() const

   Returns the globally unique identifier which is stored in this detail.
 */

/*!
   \fn QOrganizerItemGuid::setGuid(const QString& guid)
   Sets the globally unique identifier which is stored in this detail to \a guid.
 */






/* ==================== QOrganizerItemInstanceOrigin.h ======================= */
/*!
   \class QOrganizerItemInstanceOrigin
   \ingroup organizer-details
   \brief The QOrganizerItemInstanceOrigin class contains information about when and from where or whom the organizer item originated.
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerItemInstanceOrigin::DefinitionName
   The constant string which identifies the definition of details which contain information which identifies
   the recurrent event of which the organizer item is a particular instance or exception of.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemInstanceOrigin::DefinitionName, "InstanceOrigin");

/*!
   \variable QOrganizerItemInstanceOrigin::FieldParentLocalId

   The constant key for the field in which the local id of the parent recurrent event
   is stored in details of the QOrganizerItemInstanceOrigin type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemInstanceOrigin::FieldParentLocalId, "ParentLocalId");


/*!
   \variable QOrganizerItemInstanceOrigin::FieldOriginalDate

   The constant key for the field in which the original date of this instance
   or exception according to the recurrent series of the parent item is stored.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemInstanceOrigin::FieldOriginalDate, "OriginalDate");

/*!
   \fn QOrganizerItemInstanceOrigin::parentLocalId() const
    Returns the local id of the item instance origin's parent item.
 */

/*!
   \fn QOrganizerItemInstanceOrigin::setParentLocalId(QOrganizerItemLocalId parentId)
    Sets the parent id of this instance origin item to \a parentId.
 */

/*!
   \fn QOrganizerItemInstanceOrigin::originalDate() const
    Returns the original date of this instance origin item.
 */

/*!
   \fn QOrganizerItemInstanceOrigin::setOriginalDate(const QDate& date)
    Sets the origin date to \a date.
 */


/* ==================== QOrganizerJournalTimeRange ======================= */
// XXX TODO: time range or just single instant in time?
/*!
   \class QOrganizerJournalTimeRange
   \ingroup organizer-details

   \brief The QOrganizerJournalTimeRange class contains information about
   the date and time for which a journal entry has been created.
   \inmodule QtOrganizer
 */

/*!
\variable QOrganizerJournalTimeRange::DefinitionName

The constant string which identifies the definition of details
which describe the date and time associated with a journal item.
*/
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJournalTimeRange::DefinitionName, "JournalTimeRange");

/*!
   \variable QOrganizerJournalTimeRange::FieldEntryDateTime

   The constant key for which the date and time of value for a journal entry is
   stored in details of the QOrganizerJournalTimeRange type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJournalTimeRange::FieldEntryDateTime, "EntryDateTime");

/*!
   \fn QOrganizerJournalTimeRange::entryDateTime() const
   Returns the journal entry date and time as QDateTime.
 */

/*!
   \fn QOrganizerJournalTimeRange::setEntryDateTime(const QDateTime& entryDateTime)
   Sets the journal entry date and time to \a entryDateTime.
 */

/* ==================== QOrganizerItemLocation ======================= */
// XXX TODO: do we include all three pieces of data into a single detail
// ie, street address + geolocation + location string
// Or, do we have three separate detail types?
/*!
   \class QOrganizerItemLocation
   \ingroup organizer-details

   \brief The QOrganizerItemLocation class contains information about a location which is related to the organizer item in some manner.
   \inmodule QtOrganizer
 */

/*!
\variable QOrganizerItemLocation::DefinitionName

The constant string which identifies the definition of details
which describe a location associated with an organizer item.
*/
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemLocation::DefinitionName, "Location");

/*!
   \variable QOrganizerItemLocation::FieldGeoLocation

   The constant key for which the geolocation value is
   stored in details of the QOrganizerItemLocation type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemLocation::FieldGeoLocation, "GeoLocation");

/*!
   \variable QOrganizerItemLocation::FieldAddress

   The constant key for which the location address value is
   stored in details of the QOrganizerItemLocation type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemLocation::FieldAddress, "Address");

/*!
   \variable QOrganizerItemLocation::FieldLocationName

   The constant key for which the location label value is
   stored in details of the QOrganizerItemLocation type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemLocation::FieldLocationName, "LocationName");

/*!
    \fn QOrganizerItemLocation::geoLocation() const
    Returns a string of geolication value.
 */

/*!
    \fn QOrganizerItemLocation::setGeoLocation(const QString& stringCoords)
    Sets geolication value to \a stringCoords.
 */

/*!
    \fn QOrganizerItemLocation::locationName() const
    Returns a string of location name.
 */

/*!
   \fn QOrganizerItemLocation::setLocationName(const QString& locationName)
   Sets location name to \a locationName.
 */

/*!
    \fn QOrganizerItemLocation::address() const
    Returns a string of address name.
 */

/*!
   \fn QOrganizerItemLocation::setAddress(const QString& address)
   Sets address name to \a address.
 */

/* ==================== QOrganizerItemComment ======================= */
/*!
   \class QOrganizerItemComment
   \ingroup organizer-details
   \brief The QOrganizerItemComment class contains some arbitrary information which is relevant to the organizer item.
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerItemComment::DefinitionName
   The constant string which identifies the definition of details which are comments.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemComment::DefinitionName, "Comment");

/*!
   \variable QOrganizerItemComment::FieldComment

   The constant key for which the comment value is stored in details of
   the QOrganizerItemComment type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemComment::FieldComment, "Comment");

/*!
   \fn QOrganizerItemComment::setComment(const QString& comment)
   Sets a comment associated with an organizer item to \a comment.
 */

/*!
   \fn QOrganizerItemComment::comment() const
   Returns a string for a comment associated with an organizer item.
 */

/* ==================== QOrganizerItemPriority ======================= */
/*!
   \class QOrganizerItemPriority
   \ingroup organizer-details
   \brief The QOrganizerItemPriority class contains the priority of the organizer item, which may be used to resolve scheduling conflicts.
   \inmodule QtOrganizer
 */

/*!
   \enum QOrganizerItemPriority::Priority
   \value UnknownPriority
   \value HighestPriority
   \value ExtremelyHighPriority
   \value VeryHighPriority
   \value HighPriority
   \value MediumPriority
   \value LowPriority
   \value VeryLowPriority
   \value ExtremelyLowPriority
   \value LowestPriority
*/

/*!
   \variable QOrganizerItemPriority::DefinitionName
   The constant string which identifies the definition of details which contain the priority of an organizer item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemPriority::DefinitionName, "Priority");

/*!
   \variable QOrganizerItemPriority::FieldPriority

   The constant key for which the priority value is stored in details of
   the QOrganizerItemPriority type.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemPriority::FieldPriority, "Priority");

/*!
   \fn QOrganizerItemPriority::setPriority(Priority priority)

   Sets the priority associated with an organizer item to \a priority.
 */

/*!
   \fn Priority QOrganizerItemPriority::priority() const

   Returns the priority associated with an organizer item.
 */





/* ==================== QOrganizerItemRecurrence ======================= */
/*!
   \class QOrganizerItemRecurrence
   \ingroup organizer-details
   \brief The QOrganizerItemRecurrence class contains a list of rules and dates on which the recurrent item occurs,
          and a list of rules and dates on which exceptions occur.
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerItemRecurrence::DefinitionName
   The constant string which identifies the definition of details which are organizer item recurrence specifications.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemRecurrence::DefinitionName, "Recurrence");

/*!
   \variable QOrganizerItemRecurrence::FieldRecurrenceRules

   The constant key for the value which is stored in details of the
   QOrganizerItemRecurrence type which describes the rules for when an
   item should recur.

   When multiple rules are specified, the list of recurrence dates are
   calculated separately for each rule and the results are unioned.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemRecurrence::FieldRecurrenceRules, "RecurrenceRules");

/*!
   \variable QOrganizerItemRecurrence::FieldRecurrenceDates

   The constant key for the value which is stored in details of the
   QOrganizerItemRecurrence type which describes the dates on which an
   item should recur.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemRecurrence::FieldRecurrenceDates, "RecurrenceDates");

/*!
   \variable QOrganizerItemRecurrence::FieldExceptionRules

   The constant key for the value which is stored in details of the
   QOrganizerItemRecurrence type which describes the rules for when an
   item should not recur.

   If a recurrence rule or the recurrence dates list specifies that an item should occur on a
   particular date and any of the exception rules include that date, the item should not occur on
   that date.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemRecurrence::FieldExceptionRules, "ExceptionRules");

/*!
   \variable QOrganizerItemRecurrence::FieldExceptionDates

   The constant key for the value which is stored in details of the
   QOrganizerItemRecurrence type which describes the dates on which an
   item should not recur.

   If a recurrence rule or the recurrence dates list specifies that an item should occur on a
   particular date and that date appears in the exception dates list, the item should not occur on
   that date.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemRecurrence::FieldExceptionDates, "ExceptionDates");

/*!
   \fn QOrganizerItemRecurrence::recurrenceDates() const
   Returns a list of recurrence dates.
 */

/*!
   \fn QOrganizerItemRecurrence::setRecurrenceDates(const QList<QDate>& rdates)
   Sets a list of recurrence dates to \a rdates.
 */

/*!
   \fn QOrganizerItemRecurrence::exceptionRules() const
   Returns a list of exception rules.
 */

/*!
   \fn QOrganizerItemRecurrence::setExceptionRules(const QList<QOrganizerItemRecurrenceRule>& xrules)
   Sets a list of exception rules to \a xrules.
 */

/*!
   \fn QOrganizerItemRecurrence::recurrenceRules() const
   Returns a list of recurrence rules.
 */

/*!
   \fn QOrganizerItemRecurrence::setRecurrenceRules(const QList<QOrganizerItemRecurrenceRule>& rrules)
   Sets a list of recurrence rules to \a rrules.
 */

/*!
   \fn QOrganizerItemRecurrence::exceptionDates() const
   Returns a list of exception dates.
 */

/*!
   \fn QOrganizerItemRecurrence::setExceptionDates(const QList<QDate>& exdates)
   Sets a list of exception dates to \a exdates.
 */






/* ==================== QOrganizerItemTimestamp ======================= */
/*!
   \class QOrganizerItemTimestamp
   \ingroup organizer-details
   \brief The QOrganizerItemTimestamp class contains the creation and last-modified timestamp associated with the organizer item.  XXX TODO: what about last accessed?
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerItemTimestamp::DefinitionName
   The constant string which identifies the definition of details which are organizer synchronisation timestamps.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemTimestamp::DefinitionName, "Timestamp");

/*!
   \variable QOrganizerItemTimestamp::FieldModificationTimestamp

   The constant key for the value which is stored in details of the
   QOrganizerItemTimestamp type which describes the last modification date
   and time of an organizer item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemTimestamp::FieldModificationTimestamp, "ModificationTimestamp");

/*!
   \variable QOrganizerItemTimestamp::FieldCreationTimestamp

   The constant key for the value which is stored in details of the
   QOrganizerItemTimestamp type which describes the creation date and time
   of an organizer item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemTimestamp::FieldCreationTimestamp, "CreationTimestamp");

/*!
   \fn QOrganizerItemTimestamp::created() const
   Returns the creation timestamp saved in this detail.
 */

/*!
   \fn QOrganizerItemTimestamp::lastModified() const
   Returns the last-modified timestamp saved in this detail.
 */

/*!
   \fn QOrganizerItemTimestamp::setCreated(const QDateTime& dateTime)
   Sets the creation timestamp saved in this detail to \a dateTime.
 */

/*!
   \fn QOrganizerItemTimestamp::setLastModified(const QDateTime& dateTime)
   Sets the last-modified timestamp saved in this detail to \a dateTime.
 */





/* ==================== QOrganizerTodoProgress ======================= */
/*!
   \class QOrganizerTodoProgress
   \ingroup organizer-details
   \brief The QOrganizerTodoProgress class contains information about the progress of a todo item.
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerTodoProgress::DefinitionName
   The constant string which identifies the definition of details which contain progress information about a todo item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerTodoProgress::DefinitionName, "TodoProgress");

/*!
   \variable QOrganizerTodoProgress::FieldStatus

   The constant key of the value which describes the current completion status of the
   todo item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerTodoProgress::FieldStatus, "Status");

/*!
   \variable QOrganizerTodoProgress::FieldPercentageComplete

   The constant key of the value which contains the current completion percentage of the
   todo item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerTodoProgress::FieldPercentageComplete, "PercentageComplete");

/*!
   \variable QOrganizerTodoProgress::FieldFinishedDateTime

   The constant key of the date time value which contains the date and time at which the
   todo item was completed.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerTodoProgress::FieldFinishedDateTime, "FinishedDateTime");

/*!
  \enum QOrganizerTodoProgress::Status
  Enumerates the various possible types of todo item status
  \value StatusNotStarted The todo item hasn't been started yet
  \value StatusInProgress The todo item is current in progress
  \value StatusComplete The todo item has finished
 */

/*!
  \fn QOrganizerTodoProgress::status() const
  Returns the todo progress item's current status as QOrganizerTodoProgress::Status.
 */

/*!
  \fn QOrganizerTodoProgress::setStatus(Status status)
  Sets the todo progress item's current status to \a status.
 */

/*!
  \fn QOrganizerTodoProgress::finishedDateTime() const
  Returns the todo progress item's finished date and timeas QDateTime.
 */

/*!
  \fn QOrganizerTodoProgress::setFinishedDateTime(const QDateTime& finishedDateTime)
  Sets the todo progress item's finished date and time to \a finishedDateTime.
 */

/*!
  \fn QOrganizerTodoProgress::percentageComplete() const
  Returns the todo progress item's completion percentage.
 */

/*!
  \fn QOrganizerTodoProgress::setPercentageComplete(int percentage)
  Sets the todo progress item's completion percentage to \a percentage.
 */
/* ==================== QOrganizerTodoTimeRange ======================= */
/*!
   \class QOrganizerTodoTimeRange
   \ingroup organizer-details
   \brief The QOrganizerTodoTimeRange class contains information about the time range of a todo item.
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerTodoTimeRange::DefinitionName
   The constant string which identifies the definition of details which contain time-range information about a todo item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerTodoTimeRange::DefinitionName, "TodoTimeRange");

/*!
   \variable QOrganizerTodoTimeRange::FieldStartDateTime

   The constant key of the date time value which describes the date at which
   the todo should be started.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerTodoTimeRange::FieldStartDateTime, "StartDateTime");

/*!
   \variable QOrganizerTodoTimeRange::FieldDueDateTime

   The constant key of the date time value which describes the latest date at which
   the todo should be completed.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerTodoTimeRange::FieldDueDateTime, "DueDateTime");

/*!
    \fn QOrganizerTodoTimeRange::startDateTime() const
    Returns the todo timerange's start date and time as QDateTime.
 */

/*!
    \fn QOrganizerTodoTimeRange::setStartDateTime(const QDateTime& startDateTime)
    Sets the todo timerange's start date and time to \a startDateTime.
 */

/*!
    \fn QOrganizerTodoTimeRange::dueDateTime() const
    Returns the todo timerange's due date and time as QDateTime.
 */

/*!
    \fn QOrganizerTodoTimeRange::setDueDateTime(const QDateTime& dueDateTime)
    Sets the todo timerange's due date and time to \a dueDateTime.
 */

/* ==================== QOrganizerItemType ======================= */
/*!
   \class QOrganizerItemType
   \ingroup organizer-details
   \brief The QOrganizerItemType class describes the type of the organizer item.  This detail may be automatically synthesized by the backend depending on other details in the organizer item.
   \inmodule QtOrganizer
 */

/*!
   \variable QOrganizerItemType::DefinitionName
   The constant string which identifies the definition of details which identify the type of the organizer item.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::DefinitionName, "Type");

/*!
   \variable QOrganizerItemType::FieldType

   The constant key for the type value which is stored in details of
   the QOrganizerItemType definition.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::FieldType, "Type");

/*!
   \variable QOrganizerItemType::TypeEvent

   The constant attribute value which describes the organizer item as being an event.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeEvent, "Event");

/*!
   \variable QOrganizerItemType::TypeEventOccurrence

   The constant attribute value which describes the organizer item as being an occurrence of an event.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeEventOccurrence, "EventOccurrence");

/*!
   \variable QOrganizerItemType::TypeJournal

   The constant attribute value which describes the organizer item as being a journal.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeJournal, "Journal");

/*!
   \variable QOrganizerItemType::TypeNote

   The constant attribute value which describes the organizer item as being a note.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeNote, "Note");

/*!
   \variable QOrganizerItemType::TypeTodo

   The constant attribute value which describes the organizer item as being a todo.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeTodo, "Todo");

/*!
   \variable QOrganizerItemType::TypeTodoOccurrence

   The constant attribute value which describes the organizer item as being an occurrence of a todo.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeTodoOccurrence, "TodoOccurrence");

/*!
   \fn QOrganizerItemType::type() const
   Returns the organizer item type value stored in this detail.
 */

/*!
   \fn QOrganizerItemType::setType(const QString& type)
   Sets the type of the organizer item to be the give \a type.
 */


















QTM_END_NAMESPACE