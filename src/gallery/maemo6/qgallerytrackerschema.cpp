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

#include "qgallerytrackerschema_p.h"

#include "qgalleryabstractrequest.h"
#include "qgallerydbusinterface_p.h"
#include "qgallerytrackerresultset_p.h"
#include "qgallerytrackerlistcolumn_p.h"
#include "qgallerytrackertyperesultset_p.h"

#include <QtCore/qdatetime.h>
#include <QtCore/qdir.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qsettings.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qurl.h>
#include <QtCore/qxmlstream.h>
#include <QtCore/QDebug>

QTM_BEGIN_NAMESPACE

namespace
{

    template <typename T>
    struct QGalleryPropertyList
    {
        QGalleryPropertyList() : items(0), count(0) {}
        template <int N> QGalleryPropertyList(const T (&items)[N])
            : items(items), count(N) {}

        const T *items;
        const int count;

        int indexOfProperty(const QString &propertyName) const;

        const T &operator [](int index) const { return items[index]; }
    };


    struct QGalleryItemProperty
    {
        QLatin1String name;
        QLatin1String field;
        QVariant::Type type;
        QGalleryProperty::Attributes attributes;
    };

    typedef QGalleryPropertyList<QGalleryItemProperty> QGalleryItemPropertyList;

    struct QGalleryAggregateProperty
    {
        QLatin1String name;
        QLatin1String field;
        QLatin1String aggregate;
        QVariant::Type type;
    };

    typedef QGalleryPropertyList<QGalleryAggregateProperty> QGalleryAggregatePropertyList;

    struct QGalleryCompositeProperty
    {
        QLatin1String name;
        QVariant::Type type;
        QGalleryItemPropertyList dependencies;
        QGalleryTrackerCompositeColumn *(*createColumn)(const QVector<int> &columns);
        bool (*writeFilterCondition)(
                QDocumentGallery::Error *error,
                QString *query,
                const QGalleryCompositeProperty &property,
                const QGalleryMetaDataFilter &filter);
    };

    typedef QGalleryPropertyList<QGalleryCompositeProperty> QGalleryCompositePropertyList;

    enum UpdateId
    {
        FileId          = 0x0001,
        FolderId        = 0x0002,
        DocumentId      = 0x0004,
        AudioId         = 0x0008,
        ImageId         = 0x0010,
        VideoId         = 0x0020,
        PlaylistId      = 0x0040,
        TextId          = 0x0080,
        ArtistId = 0x0100,
        AlbumId = 0x0200,
        AlbumArtistId = ArtistId,
        PhotoAlbumId = 0x0400

    };

    enum UpdateMask
    {
        FileMask        = FileId
                        | FolderId
                        | DocumentId
                        | AudioId
                        | ImageId
                        | VideoId
                        | PlaylistId
                        | TextId,
        FolderMask      = FolderId,
        DocumentMask    = DocumentId,
        AudioMask       = AudioId,
        ImageMask       = ImageId,
        VideoMask       = VideoId,
        PlaylistMask    = PlaylistId,
        TextMask        = TextId,
        ArtistMask = ArtistId,
        AlbumMask = AlbumId,
        AlbumArtistMask = AlbumArtistId,
        PhotoAlbumMask = PhotoAlbumId

    };

    struct QGalleryTypePrefix : public QLatin1String
    {
        template <int N> QGalleryTypePrefix(const char (&prefix)[N])
            : QLatin1String(prefix), length(N - 1){}

        const int length;

        QStringRef strip(const QString &id) const {
            return QStringRef(&id, length, id.length() - length); }
    };

    struct QGalleryItemType
    {
        QLatin1String itemType;
        QLatin1String service;
        QLatin1String rdfSuffix;
        QLatin1String typeFragment;
        const char *filterFragment;
        QGalleryTypePrefix prefix;
        QGalleryItemPropertyList itemProperties;
        QGalleryCompositePropertyList compositeProperties;
        int updateId;
        int updateMask;
    };

    struct QGalleryAggregateType
    {
        QLatin1String itemType;
        QLatin1String service;
        QGalleryTypePrefix prefix;
        QGalleryItemPropertyList identity;
        QGalleryItemPropertyList properties;
        QGalleryAggregatePropertyList aggregateProperties;
        QGalleryCompositePropertyList compositeProperties;
/*        void (*writeIdCondition)(QDocumentGallery::Error *error, QXmlStreamWriter *xml, const QStringRef &itemId); */
        int updateMask;
    };

    template <typename T>
    struct QGalleryTypeList
    {
        template <int N> QGalleryTypeList(const T (&items)[N])
            : items(items), count(N) {}

        const T *items;
        const int count;

        int indexOfType(const QString &type) const;
        int indexOfItemId(const QString &itemId) const;
        int indexOfService(const QString &service) const;
        int indexOfRdfTypes(const QStringList &rdfTypes) const;
        const T &operator [](int index) const { return items[index]; }
    };

    typedef QGalleryTypeList<QGalleryItemType> QGalleryItemTypeList;
    typedef QGalleryTypeList<QGalleryAggregateType> QGalleryAggregateTypeList;

    template <typename T>
    int QGalleryPropertyList<T>::indexOfProperty(const QString &name) const
    {
        for (int i = 0; i < count; ++i) {
            if (items[i].name == name)
                return i;
        }
        return -1;
    }

    template <typename T>
    int QGalleryTypeList<T>::indexOfType(const QString &type) const
    {
        for (int i = 0; i < count; ++i) {
            if (items[i].itemType == type)
                return i;
        }
        return -1;
    }

    template <typename T>
    int QGalleryTypeList<T>::indexOfItemId(const QString &itemId) const
    {
        for (int i = 0; i < count; ++i) {
            if (itemId.startsWith(items[i].prefix))
                return i;
        }
        return -1;
    }

    template <typename T>
    int QGalleryTypeList<T>::indexOfService(const QString &service) const
    {
        for (int i = 0; i < count; ++i) {
            if (items[i].service == service)
                return i;
        }
        return -1;
    }

    template <typename T>
    int QGalleryTypeList<T>::indexOfRdfTypes(const QStringList &rdfTypes) const
    {
        int index = -1;
        int rdfIndex = -1;
        for (int i = 0; i < count; ++i) {
            for (int j = rdfTypes.count() - 1; j >= 0; --j) {
                if (rdfTypes.at(j).endsWith(items[i].rdfSuffix)) {
                    if (j > rdfIndex) {
                        index = i;
                        rdfIndex = j;
                    }
                    break;
                }
            }
        }
        return index;
    }
    // Re-declare to cut down on prefixes.
    enum
    {
        CanRead         = QGalleryProperty::CanRead,
        CanWrite        = QGalleryProperty::CanWrite,
        CanSort         = QGalleryProperty::CanSort,
        CanFilter       = QGalleryProperty::CanFilter,
        IsResource      = 0x100,
        PropertyMask    = 0xFF
    };

    class QGalleryReorderLessThan
    {
    public:
        QGalleryReorderLessThan(const QStringList &sortOrder)
            : m_sortOrder(QStringList(sortOrder)
                    .replaceInStrings(QLatin1String("+"), QString())
                    .replaceInStrings(QLatin1String("-"), QString()))
        {
        }

        bool operator ()(const QString &property1, const QString &property2) const
        {
            uint index1 = m_sortOrder.indexOf(property1);
            uint index2 = m_sortOrder.indexOf(property2);

            return index1 < index2;
        }

    private:
        QStringList m_sortOrder;
    };
}

#define QT_GALLERY_ITEM_PROPERTY(PropertyName, Field, Type, Attr) \
{ QLatin1String(PropertyName), QLatin1String(Field), QVariant::Type, QGalleryProperty::Attributes(Attr) }

#define QT_GALLERY_AGGREGATE_PROPERTY(PropertyName, Field, Aggregate, Type) \
{ QLatin1String(PropertyName), QLatin1String(Field), QLatin1String(Aggregate), QVariant::Type }

#define QT_GALLERY_COMPOSITE_PROPERTY(PropertyName, Type, Dependencies, Factory, QueryBuilder) \
{ QLatin1String(PropertyName), QVariant::Type, QGalleryItemPropertyList(Dependencies), Factory, QueryBuilder }

#define QT_GALLERY_COMPOSITE_PROPERTY_NO_DEPENDENCIES(PropertyName, Type, Factory, QueryBuilder) \
{ QLatin1String(PropertyName), QVariant::Type, QGalleryItemPropertyList(), Factory, QueryBuilder }

#define QT_GALLERY_ITEM_TYPE(Type, RdfPrefix, RdfType, Prefix, PropertyGroup) \
{ \
    QLatin1String(#Type), \
    QLatin1String(#RdfPrefix":"#RdfType), \
    QLatin1String("/"#RdfPrefix"#"#RdfType), \
    QLatin1String("{?x rdf:type "#RdfPrefix":"#RdfType"}"), \
    0, \
    QGalleryTypePrefix(#Prefix"::"), \
    QGalleryItemPropertyList(qt_gallery##PropertyGroup##PropertyList), \
    QGalleryCompositePropertyList(qt_gallery##PropertyGroup##CompositePropertyList), \
    Type##Id, \
    Type##Mask \
}

#define QT_GALLERY_ITEM_TYPE_NO_COMPOSITE(Type, RdfPrefix, RdfType, Prefix, PropertyGroup) \
{ \
    QLatin1String(#Type), \
    QLatin1String(#RdfPrefix":"#RdfType), \
    QLatin1String("/"#RdfPrefix"#"#RdfType), \
    QLatin1String("{?x rdf:type "#RdfPrefix":"#RdfType"}"), \
    0, \
    QGalleryTypePrefix(#Prefix"::"), \
    QGalleryItemPropertyList(qt_gallery##PropertyGroup##PropertyList), \
    QGalleryCompositePropertyList(), \
    Type##Id, \
    Type##Mask \
}

#define QT_GALLERY_ITEM_TYPE_NO_COMPOSITE_FILTERED(Type, RdfPrefix, RdfType, FilterType, FilterProperty, Prefix, PropertyGroup) \
{ \
    QLatin1String(#Type), \
    QLatin1String(#RdfPrefix":"#RdfType), \
    QLatin1String("/"#RdfPrefix"#"#RdfType), \
    QLatin1String("{?x rdf:type "#RdfPrefix":"#RdfType"}{?y rdf:type "#FilterType"}"), \
    #FilterProperty"(?y)=?x", \
    QGalleryTypePrefix(#Prefix"::"), \
    QGalleryItemPropertyList(qt_gallery##PropertyGroup##PropertyList), \
    QGalleryCompositePropertyList(), \
    Type##Id, \
    Type##Mask \
}

#define QT_GALLERY_AGGREGATE_TYPE(Type, RdfPrefix, RdfType, Prefix, UpdateMask) \
{ \
    QLatin1String(#Type), \
    QLatin1String(#RdfPrefix":"#RdfType), \
    QGalleryTypePrefix(#Prefix"::"), \
    QGalleryItemPropertyList(qt_gallery##Type##Identity), \
    QGalleryItemPropertyList(qt_gallery##Type##PropertyList),\
    QGalleryAggregatePropertyList(qt_gallery##Type##AggregateList), \
    QGalleryCompositePropertyList(), \
    UpdateMask \
}

static bool qt_writeCondition(
        QDocumentGallery::Error *error,
        QString *query,
        const QGalleryFilter &filter,
        const QGalleryItemPropertyList &properties,
        const QGalleryCompositePropertyList &composites);

static bool qt_writeConditionHelper(
        QDocumentGallery::Error *error,
        QString *query,
        const QList<QGalleryFilter> &filters,
        const QGalleryItemPropertyList &properties,
        const QGalleryCompositePropertyList &composites,
        const QString& op
        )
{
    if (!filters.isEmpty()) {
        *query += "(";

        int count = filters.size();
        for (QList<QGalleryFilter>::const_iterator it = filters.begin(), end = filters.end();
                it != end;
                ++it) {
            if (!qt_writeCondition(error, query, *it, properties, composites))
                return false;
            if ( --count > 0 )
                *query += op;
        }
        *query += ")";
    }
    return true;
}

static bool qt_writeCondition(
        QDocumentGallery::Error *error,
        QString *query,
        const QGalleryIntersectionFilter &filter,
        const QGalleryItemPropertyList &properties,
        const QGalleryCompositePropertyList &composites)
{
    return qt_writeConditionHelper( error, query, filter.filters(), properties, composites, "&&" );
}

static bool qt_writeCondition(
        QDocumentGallery::Error *error,
        QString *query,
        const QGalleryUnionFilter &filter,
        const QGalleryItemPropertyList &properties,
        const QGalleryCompositePropertyList &composites)
{
    return qt_writeConditionHelper( error, query, filter.filters(), properties, composites, "||" );
}

static QString qt_writePropertyFunctions( const QStringList &fields, const QString &variable )
{
    QString result;
    foreach ( const QString &field, fields )
    {

        int parenthesisCount = field.count('(');
        result += field;
        result += "(?";
        result += variable;
        result += ") ";
        while ( parenthesisCount-- > 0 )
            result += ") ";
    }

    return result.trimmed();
}

static void qt_write_comparison( const QString &field, const QString &value, const QString &op, const QString &variable_name, QString *query )
{
    *query += "("  + qt_writePropertyFunctions( QStringList() << field, variable_name ) + op + "'" + value + "')";
}

static void qt_write_function( const QString &function, const QString &field, const QString &value, const QString &variable_name, QString *query )
{
    *query += function + "(" + qt_writePropertyFunctions( QStringList() << field, variable_name )  + ",\"" + value + "\")";
}

static bool qt_writeCondition(
        QDocumentGallery::Error *error,
        QString *query,
        const QGalleryMetaDataFilter &filter,
        const QGalleryItemPropertyList &properties,
        const QGalleryCompositePropertyList &composites)
{
    if (filter.isNegated())
        *query += "!";

    const QString propertyName = filter.propertyName();

    int index;

    if ((index = properties.indexOfProperty(propertyName)) != -1) {

        switch (filter.comparator()) {
        case QGalleryFilter::Equals:
            qt_write_comparison( properties[index].field, filter.value().toString(), "=", "x", query);
            break;
        case QGalleryFilter::LessThan:
            qt_write_comparison( properties[index].field, filter.value().toString(), "<", "x", query);
            break;
        case QGalleryFilter::GreaterThan:
            qt_write_comparison( properties[index].field, filter.value().toString(), ">", "x", query);
            break;
        case QGalleryFilter::LessThanEquals:
            qt_write_comparison( properties[index].field, filter.value().toString(), "<=", "x", query);
            break;
        case QGalleryFilter::GreaterThanEquals:
            qt_write_comparison( properties[index].field, filter.value().toString(), ">=", "x", query);
            break;
        case QGalleryFilter::Contains:
            qt_write_function( "fn:contains", properties[index].field, filter.value().toString(), "x", query );
            break;
        case QGalleryFilter::StartsWith:
            qt_write_function( "fn:starts-with", properties[index].field, filter.value().toString(), "x", query );
            break;
        case QGalleryFilter::EndsWith:
            qt_write_function( "fn:ends-with", properties[index].field, filter.value().toString(), "x", query );
            break;
        case QGalleryFilter::Wildcard:
            qt_write_function( "fn:contains", properties[index].field, "*", "x", query );
            break;
        case QGalleryFilter::RegExp:
            qt_write_function( "REGEX", properties[index].field, filter.value().toString(), "x", query );
            break;
        default:
            *error = QDocumentGallery::FilterError;

            return false;
        }
        return true;
    } else if ((index = composites.indexOfProperty(propertyName)) != -1
            && composites[index].writeFilterCondition) {
        return composites[index].writeFilterCondition(error, query, composites[index], filter);
    } else {
        *error = QDocumentGallery::FilterError;
        return false;
    }
}

static bool qt_writeCondition(
        QDocumentGallery::Error *error,
        QString *query,
        const QGalleryFilter &filter,
        const QGalleryItemPropertyList &properties,
        const QGalleryCompositePropertyList &composites)
{
    switch (filter.type()) {
    case QGalleryFilter::Intersection:
        {
        bool ret = qt_writeCondition(error, query, filter.toIntersectionFilter(), properties, composites);
        if (!ret)
            *query  = "";
        return ret;
        }
    case QGalleryFilter::Union:
        {
            bool ret = qt_writeCondition(error, query, filter.toUnionFilter(), properties, composites);
            if (!ret)
                *query  = "";
            return ret;
        }
    case QGalleryFilter::MetaData:
        {
            bool ret = qt_writeCondition(error, query, filter.toMetaDataFilter(), properties, composites);
            if (!ret)
                *query  = "";
            return ret;
        }
    default:
        Q_ASSERT(filter.type() != QGalleryFilter::Invalid);
        *error = QDocumentGallery::FilterError;
        return false;
    }
}

static bool qt_writeFilePathCondition(
        QDocumentGallery::Error *error,
        QString *query,
        const QGalleryCompositeProperty &,
        const QGalleryMetaDataFilter &filter)
{
    if (filter.comparator() != QGalleryFilter::Equals) {
        *error = QDocumentGallery::FilterError;
        return false;
    } else {
        QString encodedValue(QUrl::fromLocalFile(filter.value().toString()).toEncoded());
        encodedValue.replace('\'',"\\\'");
        *query += "( nie:url(?x) ='" + encodedValue + "')";
        return true;
    }
}

///////
// File
///////

#define QT_GALLERY_FILE_MIMETYPE_PROPERTY \
    QT_GALLERY_ITEM_PROPERTY("mimeType"    , "nie:mimeType"     , String, CanRead | CanSort | CanFilter | IsResource)

// These are repeated in a few lists.
#define QT_GALLERY_FILE_PROPERTIES \
    QT_GALLERY_FILE_MIMETYPE_PROPERTY, \
    QT_GALLERY_ITEM_PROPERTY("url"         , "nie:url"             , String  , CanRead | CanSort | CanFilter), \
    QT_GALLERY_ITEM_PROPERTY("fileName"    , "nfo:fileName"        , String  , CanRead | CanSort | CanFilter), \
    QT_GALLERY_ITEM_PROPERTY("fileSize"    , "nfo:fileSize"        , Int     , QGalleryProperty::Attributes()), \
    QT_GALLERY_ITEM_PROPERTY("copyright"   , "nie:copyright"       , String  , CanRead | CanWrite | CanSort | CanFilter), \
    QT_GALLERY_ITEM_PROPERTY("lastModified", "nfo:fileLastModified", DateTime, CanRead | CanSort | CanFilter), \
    QT_GALLERY_ITEM_PROPERTY("lastAccessed", "nfo:fileLastAccessed", DateTime, CanRead | CanSort | CanFilter)
    /*QT_GALLERY_ITEM_PROPERTY(""            , "File:License"  , String  , QGalleryProperty::Attributes()),*/ \
    /*QT_GALLERY_ITEM_PROPERTY("path"        , "File:Path"     , String  , CanRead | CanSort | CanFilter),*/ \
    /*QT_GALLERY_ITEM_PROPERTY(""            , "File:Contents" , String  , QGalleryProperty::Attributes()),*/ \
    /*QT_GALLERY_ITEM_PROPERTY(""            , "File:Link"     , String  , QGalleryProperty::Attributes()),*/ \
    /*QT_GALLERY_ITEM_PROPERTY(""            , "File:Added"    , DateTime, QGalleryProperty::Attributes()),*/ \
    /*QT_GALLERY_ITEM_PROPERTY(""            , "File:Other"    , String  ,   QGalleryProperty::Attributes()),*/ \
    /*QT_GALLERY_ITEM_PROPERTY("language"    , "File:Language" , String  , CanRead | CanSort | CanFilter) */

static const QGalleryItemProperty qt_galleryFilePropertyList[] =
{
    QT_GALLERY_FILE_PROPERTIES,
    QT_GALLERY_ITEM_PROPERTY("description", "nie:description", String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("subject"    , "nie:subject"    , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("title"      , "nie:title"      , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("keywords"   , "nie:keyword" , StringList, CanRead | CanWrite | CanFilter)
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Contributor", String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Coverage"   , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY("author"     , "DC:Creator"    , String    , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Date"       , DateTime  , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Format"     , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Identifier" , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Language"   , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Publisher"  , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Relation"   , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Rights"     , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Source"     , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Keywords"   , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""         , "DC:Type"       , String    , QGalleryProperty::Attributes()),
//   QT_GALLERY_ITEM_PROPERTY("rating"     , "User:Rank"     , String    , CanRead | CanWrite | CanSort | CanFilter),
};

#define QT_GALLERY_FILE_COMPOSITE_PROPERTIES \
    QT_GALLERY_COMPOSITE_PROPERTY_NO_DEPENDENCIES("filePath", String, QGalleryTrackerFilePathColumn::create, qt_writeFilePathCondition)
//    QT_GALLERY_COMPOSITE_PROPERTY_NO_DEPENDENCIES("url", Url, QGalleryTrackerFileUrlColumn::create, qt_writeFileUrlCondition)

    static const QGalleryCompositeProperty qt_galleryFileCompositePropertyList[] =
{
    QT_GALLERY_FILE_COMPOSITE_PROPERTIES
};

////////
// Audio
////////

#define QT_GALLERY_AUDIO_ALBUMTITLE_PROPERTY \
    QT_GALLERY_ITEM_PROPERTY("albumTitle", "nmm:albumTitle(nmm:musicAlbum", String, CanRead | CanWrite | CanSort | CanFilter)
#define QT_GALLERY_AUDIO_ALBUMARTIST_PROPERTY \
    QT_GALLERY_ITEM_PROPERTY("albumArtist", "nmm:artistName(nmm:albumArtist(nmm:musicAlbum", String, CanRead | CanWrite | CanSort | CanFilter)

static const QGalleryItemProperty qt_galleryAudioPropertyList[] =
{
    QT_GALLERY_FILE_PROPERTIES,
    QT_GALLERY_AUDIO_ALBUMTITLE_PROPERTY,
    QT_GALLERY_AUDIO_ALBUMARTIST_PROPERTY,
    QT_GALLERY_ITEM_PROPERTY("title"       , "nie:title"                   , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("genre"       , "nfo:genre"                   , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("duration"    , "nfo:duration"                , Int       , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("trackNumber" , "nmm:trackNumber"             , Int       , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("description" , "nie:description"             , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("audioCodec"  , "nfo:codec"                   , String    , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("sampleRate"  , "nfo:sampleRate"              , Int       , CanRead | CanWrite | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("audioBitRate", "nfo:averageAudioBitrate"     , Int       , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("channelCount", "nfo:channels"                , Int       , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("playCount"   , "nie:usageCounter"            , Int       , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("lyrics"      , "nmm:lyrics"                  , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("artist"      , "nmm:artistName(nmm:performer", String, CanRead | CanFilter | CanSort )
//    QT_GALLERY_ITEM_PROPERTY(""          , "Audio:ReleaseDate"  , DateTime  , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY("discNumber", "Audio:DiscNo"       , Int       , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY("performer" , "Audio:Performer"    , String    , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY(""          , "Audio:TrackGain"    , Double    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""          , "Audio:AlbumGain"    , Double    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""          , "Audio:AlbumPeakGain", Double    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""          , "Audio:CodecVersion" , String    , CanRead | CanSort | CanFilter | IsResource),
//    QT_GALLERY_ITEM_PROPERTY("lastPlayed", "Audio:LastPlay"     , DateTime  , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY(""          , "Audio:DateAdded"    , DateTime  , QGalleryProperty::Attributes()),
};

static const QGalleryCompositeProperty qt_galleryAudioCompositePropertyList[] =
{
    QT_GALLERY_FILE_COMPOSITE_PROPERTIES
};

///////////
// Playlist
///////////

static const QGalleryItemProperty qt_galleryPlaylistPropertyList[] =
{
    QT_GALLERY_FILE_PROPERTIES,
    QT_GALLERY_ITEM_PROPERTY("duration"  , "nfo:listDuration"      , Int, CanRead | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("trackCount", "nfo:entryCounter"      , Int, CanRead | CanSort | CanFilter)
//    QT_GALLERY_ITEM_PROPERTY(""        , "Playlist:Name"         , String, QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""        , "Playlist:ValidDuration", String, QGalleryProperty::Attributes())
};

static const QGalleryCompositeProperty qt_galleryPlaylistCompositePropertyList[] =
{
    QT_GALLERY_FILE_COMPOSITE_PROPERTIES
};

////////
// Image
////////

static const QGalleryItemProperty qt_galleryImagePropertyList[] =
{
    QT_GALLERY_FILE_PROPERTIES,
    QT_GALLERY_ITEM_PROPERTY("height"      , "nfo:height"      , Int       , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("width"       , "nfo:width"       , Int       , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("author"      , "dc:creator"      , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("comment"     , "nie:comments"    , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("cameraModel" , "nmm:camera"      , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("orientation" , "nfo:orientation" , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("exposureTime", "nmm:exposureTime", String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("fNumber"     , "nmm:fnumber"     , Int       , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("flashEnabled", "nmm:flash"       , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("focalLength" , "nmm:focalLength" , Double    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("meteringMode", "nmm:meteringMode", String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("whiteBalance", "nmm:whiteBalance", Double    , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY("title"             , "Image:Title"          , String    , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY("keywords"          , "Image:Keywords"       , StringList, CanRead | CanWrite | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY(""                  , "Image:Album"          , String    , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY("dateTaken"         , "Image:Date"           , DateTime  , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY("description"       , "Image:Description"    , String    , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY(""                  , "Image:Software"       , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY("cameraManufacturer", "Image:CameraMake"     , String    , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY("exposureProgram"   , "Image:ExposureProgram", String    , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY(""                  , "Image:ISOSpeed"       , Double    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY("rating"            , "Image:Rating"         , Int       , CanRead | CanWrite | CanSort | CanFilter),
//    QT_GALLERY_ITEM_PROPERTY(""                  , "Image:Location"       , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""                  , "Image:Sublocation"    , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""                  , "Image:Country"        , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""                  , "Image:City"           , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""                  , "Image:HasKeywords"    , String    , QGalleryProperty::Attributes())
};

static const QGalleryCompositeProperty qt_galleryImageCompositePropertyList[] =
{
    QT_GALLERY_FILE_COMPOSITE_PROPERTIES
};

////////
// Video
////////

static const QGalleryItemProperty qt_galleryVideoPropertyList[] =
{
    QT_GALLERY_FILE_PROPERTIES,
    QT_GALLERY_ITEM_PROPERTY("author"        , "dc:creator"         , String, CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("height"        , "nfo:height"         , Int   , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("width"         , "nfo:width"          , Int   , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("duration"      , "nfo:duration"       , Int   , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("comment"       , "nie:comments"       , String, CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("frameRate"     , "nfo:frameRate"      , Double, CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("videoCodec"    , "nfo:codec"          , String, CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("videoBitRate"  , "nfo:averageBitrate" , Int   , CanRead | CanSort | CanFilter | IsResource),
    QT_GALLERY_ITEM_PROPERTY("playCount"     , "nie:usageCounter"   , Int   , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("resumePosition", "nfo:streamPosition" , Int   , CanRead | CanWrite | CanSort | CanFilter)
//    QT_GALLERY_ITEM_PROPERTY(""            , "Video:LastPlayedFrame", Int   , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""            , "Video:Source"         , String, QGalleryProperty::Attributes())
};

static const QGalleryCompositeProperty qt_galleryVideoCompositePropertyList[] =
{
    QT_GALLERY_FILE_COMPOSITE_PROPERTIES
};

///////////
// Document
///////////

static const QGalleryItemProperty qt_galleryDocumentPropertyList[] =
{
    QT_GALLERY_FILE_PROPERTIES,
    QT_GALLERY_ITEM_PROPERTY("subject"  , "nie:subject"  , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("author"   , "dc:creator"   , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("keywords" , "niec:keyword" , StringList, CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("comment"  , "nie:comments" , String    , CanRead | CanWrite | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("pageCount", "nfo:pageCount", Int       , CanRead | CanSort | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("wordCount", "nfo:wordCount", Int       , CanRead | CanSort | CanFilter)
//    QT_GALLERY_ITEM_PROPERTY(""       , "Doc:Creator"  , String    , QGalleryProperty::Attributes()),
//    QT_GALLERY_ITEM_PROPERTY(""       , "Doc:URL"      , String    , QGalleryProperty::Attributes())
};

static const QGalleryCompositeProperty qt_galleryDocumentCompositePropertyList[] =
{
    QT_GALLERY_FILE_COMPOSITE_PROPERTIES
};


static const QGalleryItemProperty qt_galleryAlbumIdentity[] =
{
    QT_GALLERY_ITEM_PROPERTY("artist", "Audio:AlbumArtist", String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("title" , "Audio:Album"      , String, CanRead | CanFilter | CanSort)
};

static const QGalleryItemProperty qt_galleryAlbumPropertyList[] =
{
    QT_GALLERY_ITEM_PROPERTY("title"      , "nmm:albumTitle"     , String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("albumTitle" , "nmm:albumTitle"     , String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("artist"     , "nmm:artistName(nmm:albumArtist", String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("albumArtist", "nmm:artistName(nmm:albumArtist", String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("trackCount" , "nmm:albumTrackCount", Int   , CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("duration"   , "nmm:albumDuration"  , Int   , CanRead | CanFilter | CanSort)
};

static const QGalleryItemProperty qt_galleryArtistPropertyList[] =
{
    QT_GALLERY_ITEM_PROPERTY("artist", "nmm:artistName", String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("title" , "nmm:artistName", String, CanRead | CanFilter | CanSort)
};

//static const QGalleryAggregateProperty qt_galleryArtistAggregateList[] =
//{
//    QT_GALLERY_AGGREGATE_PROPERTY("duration"  , "Audio:Duration", "SUM"  , Int),
//    QT_GALLERY_AGGREGATE_PROPERTY("trackCount", "*"             , "COUNT", Int),
//};

static const QGalleryItemProperty qt_galleryPhotoAlbumPropertyList[] =
{
    QT_GALLERY_ITEM_PROPERTY("title", "nie:title", String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("count", "nfo:entryCounter", String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("trackCount", "nfo:entryCounter", String, CanRead | CanFilter | CanSort)
};

/////////////
// File Types
/////////////

static const QGalleryItemType qt_galleryItemTypeList[] =
{
    QT_GALLERY_ITEM_TYPE(File      , nfo, FileDataObject, file      , File),
    QT_GALLERY_ITEM_TYPE(Folder    , nfo, Folder        , folder    , File),
    QT_GALLERY_ITEM_TYPE(Document  , nfo, Document      , document  , Document),
    QT_GALLERY_ITEM_TYPE(Audio     , nfo, Audio         , audio     , Audio),
    QT_GALLERY_ITEM_TYPE(Image     , nmm, Photo         , image     , Image),
    QT_GALLERY_ITEM_TYPE(Video     , nfo, Video         , video     , Video),
    QT_GALLERY_ITEM_TYPE(Playlist  , nmm, Playlist      , playlist  , Playlist),
    QT_GALLERY_ITEM_TYPE(Text      , nfo, TextDocument  , text      , File),
    QT_GALLERY_ITEM_TYPE_NO_COMPOSITE_FILTERED(Artist     , nmm, Artist, nmm:MusicPiece, nmm:performer  , artist     , Artist),
    QT_GALLERY_ITEM_TYPE_NO_COMPOSITE_FILTERED(AlbumArtist, nmm, Artist, nmm:MusicAlbum, nmm:albumArtist, albumArtist, Artist),
    QT_GALLERY_ITEM_TYPE_NO_COMPOSITE(Album     , nmm, MusicAlbum    , album     , Album),
    QT_GALLERY_ITEM_TYPE_NO_COMPOSITE(PhotoAlbum, nmm, ImageList     , photoAlbum, PhotoAlbum)
};

/////////
// Artist
/////////

///////////////
// Album Artist
///////////////

static const QGalleryItemProperty qt_galleryAlbumArtistIdentity[] =
{
    QT_GALLERY_ITEM_PROPERTY("title", "nmm:albumArtist", String, CanRead | CanFilter | CanSort)
};

static const QGalleryItemProperty qt_galleryAlbumArtistPropertyList[] =
{
    QT_GALLERY_ITEM_PROPERTY("artist", "nmm:albumArtist", String, CanRead | CanFilter | CanSort),
    QT_GALLERY_ITEM_PROPERTY("title" , "nmm:albumArtist", String, CanRead | CanFilter | CanSort),
};

static const QGalleryAggregateProperty qt_galleryAlbumArtistAggregateList[] =
{
    QT_GALLERY_AGGREGATE_PROPERTY("duration"  , "Audio:Duration", "SUM"  , Int),
    QT_GALLERY_AGGREGATE_PROPERTY("trackCount", "*"             , "COUNT", Int),
};

//////////////
// Music Genre
//////////////

static const QGalleryItemProperty qt_galleryAudioGenreIdentity[] =
{
    QT_GALLERY_ITEM_PROPERTY("title", "nfo:genre", String, CanRead | CanFilter)
};

static const QGalleryItemProperty qt_galleryAudioGenrePropertyList[] =
{
//    QT_GALLERY_ITEM_PROPERTY("genre", "nfo:genre", String, CanRead | CanFilter),
    QT_GALLERY_ITEM_PROPERTY("title", "nfo:genre", String, CanRead | CanFilter)
};

static const QGalleryAggregateProperty qt_galleryAudioGenreAggregateList[] =
{
    QT_GALLERY_AGGREGATE_PROPERTY("duration"  , "Audio:Duration", "SUM"  , Int),
    QT_GALLERY_AGGREGATE_PROPERTY("trackCount", "*"             , "COUNT", Int),
};

//////////////////
// Aggregate Types
//////////////////

static const QGalleryAggregateType qt_galleryAggregateTypeList[] =
{
    QT_GALLERY_AGGREGATE_TYPE(AudioGenre , nfo, Media     , audioGenre , AudioMask)
};

class QGalleryTrackerServicePrefixColumn : public QGalleryTrackerCompositeColumn
{
public:
    QGalleryTrackerServicePrefixColumn() {}

    QVariant value(QVector<QVariant>::const_iterator row) const;
};

class QGalleryTrackerServiceTypeColumn : public QGalleryTrackerCompositeColumn
{
public:
    QGalleryTrackerServiceTypeColumn() {}

    QVariant value(QVector<QVariant>::const_iterator row) const;
};

QVariant QGalleryTrackerServicePrefixColumn::value(QVector<QVariant>::const_iterator row) const
{
    QGalleryItemTypeList itemTypes(qt_galleryItemTypeList);

    const int index = itemTypes.indexOfRdfTypes((row + 2)->toString().split(QLatin1Char(',')));

    return index != -1
            ? QVariant(itemTypes[index].prefix + row->toString())
            : QVariant(QLatin1String("file::") + row->toString());
}

QVariant QGalleryTrackerServiceTypeColumn::value(QVector<QVariant>::const_iterator row) const
{
    QGalleryItemTypeList itemTypes(qt_galleryItemTypeList);

    const int index = itemTypes.indexOfRdfTypes((row + 2)->toString().split(QLatin1Char(',')));

    return index != -1
            ? QVariant(itemTypes[index].itemType)
            : QVariant(QLatin1String("File"));

}

QGalleryTrackerSchema::QGalleryTrackerSchema(const QString &itemType)
    : m_itemIndex(QGalleryItemTypeList(qt_galleryItemTypeList).indexOfType(itemType))
    , m_aggregateIndex(m_itemIndex == -1
            ? QGalleryAggregateTypeList(qt_galleryAggregateTypeList).indexOfType(itemType)
            : -1)
{
}

QGalleryTrackerSchema::~QGalleryTrackerSchema()
{
}

QGalleryTrackerSchema QGalleryTrackerSchema::fromItemId(const QString &itemId)
{
    const QGalleryItemTypeList itemTypes(qt_galleryItemTypeList);

    const int itemIndex = itemTypes.indexOfItemId(itemId);

    if (itemIndex != -1) {
        return QGalleryTrackerSchema(itemIndex, -1);
    } else {
        QGalleryAggregateTypeList aggregateTypes(qt_galleryAggregateTypeList);

        const int aggregateIndex = aggregateTypes.indexOfItemId(itemId);

        return aggregateIndex != -1
                ? QGalleryTrackerSchema(-1, aggregateIndex)
                : QGalleryTrackerSchema(-1, -1);
    }
}

QString QGalleryTrackerSchema::itemType() const
{
    if (m_itemIndex >= 0)
        return qt_galleryItemTypeList[m_itemIndex].itemType;
    else if (m_aggregateIndex >= 0)
        return qt_galleryAggregateTypeList[m_aggregateIndex].itemType;
    else
        return QString();
}

int QGalleryTrackerSchema::serviceUpdateId(const QString &service)
{
    QGalleryItemTypeList itemTypes(qt_galleryItemTypeList);

    int index = itemTypes.indexOfService(service);

    return index != -1 ? itemTypes[index].updateId : FileId;
}

QStringList QGalleryTrackerSchema::supportedPropertyNames() const
{
    QStringList propertyNames;

    if (m_itemIndex >= 0) {
        const QGalleryItemType &type = qt_galleryItemTypeList[m_itemIndex];

        for (int i = 0; i < type.itemProperties.count; ++i)
            propertyNames.append(type.itemProperties[i].name);

        for (int i = 0; i < type.compositeProperties.count; ++i)
            propertyNames.append(type.compositeProperties[i].name);
    } else if (m_aggregateIndex >= 0) {
        const QGalleryAggregateType &type = qt_galleryAggregateTypeList[m_aggregateIndex];

        for (int i = 0; i < type.properties.count; ++i)
            propertyNames.append(type.properties[i].name);

        for (int i = 0; i < type.aggregateProperties.count; ++i)
            propertyNames.append(type.aggregateProperties[i].name);

        for (int i = 0; i < type.compositeProperties.count; ++i)
            propertyNames.append(type.compositeProperties[i].name);
    }
    return propertyNames;
}

QGalleryProperty::Attributes QGalleryTrackerSchema::propertyAttributes(
        const QString &propertyName) const
{
    if (m_itemIndex >= 0) {
        const QGalleryItemType &type = qt_galleryItemTypeList[m_itemIndex];

        int index;
        if ((index = type.itemProperties.indexOfProperty(propertyName)) != -1) {
            return type.itemProperties[index].attributes & PropertyMask;
        } else if ((index = type.compositeProperties.indexOfProperty(propertyName)) != -1) {
            QGalleryProperty::Attributes attributes = QGalleryProperty::CanRead;
            if (type.compositeProperties[index].writeFilterCondition)
                attributes |= QGalleryProperty::CanFilter;
            return attributes;
        }
    } else if (m_aggregateIndex >= 0) {
        const QGalleryAggregateType &type = qt_galleryAggregateTypeList[m_aggregateIndex];

        int index;
        if ((index = type.properties.indexOfProperty(propertyName)) != -1) {
            return type.properties[index].attributes & PropertyMask;
        } else if (type.aggregateProperties.indexOfProperty(propertyName) != -1) {
            return QGalleryProperty::CanRead;
        } else if ((index = type.compositeProperties.indexOfProperty(propertyName)) != -1) {
            QGalleryProperty::Attributes attributes = QGalleryProperty::CanRead;
            if (type.compositeProperties[index].writeFilterCondition)
                attributes |= QGalleryProperty::CanFilter;
            return attributes;
        }
    }
    return QGalleryProperty::Attributes();
}

QDocumentGallery::Error QGalleryTrackerSchema::prepareItemResponse(
        QGalleryTrackerResultSetArguments *arguments,
        QGalleryDBusInterfaceFactory *dbus,
        const QString &itemId,
        const QStringList &propertyNames) const
{
    if (m_itemIndex >= 0) {
        QString query( "FILTER(?x=<" + qt_galleryItemTypeList[m_itemIndex].prefix.strip( itemId ).toString() + ">)" );
        populateItemArguments(arguments, dbus, query, propertyNames, QStringList());

        return QDocumentGallery::NoError;
    } else if (m_aggregateIndex >= 0) {
        Q_ASSERT( qt_galleryAggregateTypeList[m_aggregateIndex].identity.count == 1 );
        QString query( "FILTER(xsd:string("
                       + qt_galleryAggregateTypeList[m_aggregateIndex].identity[0].field
                       + "(?x))=\""
                       + qt_galleryAggregateTypeList[m_aggregateIndex].prefix.strip(itemId) .toString()
                       + "\")"
                     );
        populateAggregateArguments(arguments, dbus, query, propertyNames, QStringList());

        return QDocumentGallery::NoError;
    }

    return QDocumentGallery::ItemIdError;
}

QDocumentGallery::Error QGalleryTrackerSchema::prepareQueryResponse(
        QGalleryTrackerResultSetArguments *arguments,
        QGalleryDBusInterfaceFactory *dbus,
        QGalleryQueryRequest::Scope scope,
        const QString &rootItemId,
        const QGalleryFilter &filter,
        const QStringList &propertyNames,
        const QStringList &sortPropertyNames) const
{
    QString query;

    QDocumentGallery::Error error = buildFilterQuery(&query, scope, rootItemId, filter);

    if (error != QDocumentGallery::NoError) {
        return error;
    } else if (m_itemIndex >= 0) {
        populateItemArguments(arguments, dbus, query, propertyNames, sortPropertyNames);

        return QDocumentGallery::NoError;
    } else if (m_aggregateIndex >= 0) {
        populateAggregateArguments(arguments, dbus, query, propertyNames, sortPropertyNames);

        return QDocumentGallery::NoError;
    } else {
        return QDocumentGallery::ItemTypeError;
    }
}

QDocumentGallery::Error QGalleryTrackerSchema::prepareTypeResponse(
        QGalleryTrackerTypeResultSetArguments *arguments, QGalleryDBusInterfaceFactory *dbus) const
{
    if (m_itemIndex >= 0) {
        arguments->service = qt_galleryItemTypeList[m_itemIndex].service;
        arguments->accumulative = false;
        arguments->updateMask = qt_galleryItemTypeList[m_itemIndex].updateMask;

        if (qt_galleryItemTypeList[m_itemIndex].filterFragment) {
            arguments->queryInterface = dbus->metaDataInterface();
            arguments->queryMethod = QLatin1String("SparqlQuery");
            arguments->queryArguments = QVariantList()
                    << QLatin1String("SELECT COUNT(DISTINCT ?x) WHERE {")
                    + qt_galleryItemTypeList[m_itemIndex].typeFragment
                    + QLatin1String("FILTER(")
                    + QLatin1String(qt_galleryItemTypeList[m_itemIndex].filterFragment)
                    + QLatin1String(")}");
        } else {
            arguments->queryInterface = dbus->statisticsInterface();
            arguments->queryMethod = QLatin1String("Get");
            arguments->queryArguments = QVariantList();
        }

        return QDocumentGallery::NoError;
    } else if (m_aggregateIndex >= 0) {
        const QGalleryAggregateType &type = qt_galleryAggregateTypeList[m_aggregateIndex];
        arguments->service = type.service;
        arguments->accumulative = false;
        arguments->updateMask = type.updateMask;
        arguments->queryInterface = dbus->metaDataInterface();
        arguments->queryMethod = QLatin1String("SparqlQuery");

        if (type.service == "nfo:Media") {
            arguments->queryArguments = QVariantList()
                    << QLatin1String(
                            "SELECT COUNT(DISTINCT ?x) "
                            "WHERE {?urn rdf:type nfo:Media. ?urn nfo:genre ?x}");
        }

        return QDocumentGallery::NoError;
    } else {
        return QDocumentGallery::ItemTypeError;
    }
}

QDocumentGallery::Error QGalleryTrackerSchema::buildFilterQuery(
        QString *query,
        QGalleryQueryRequest::Scope scope,
        const QString &rootItemId,
        const QGalleryFilter &filter) const
{
    const QGalleryItemTypeList itemTypes(qt_galleryItemTypeList);
    const QGalleryAggregateTypeList aggregateTypes(qt_galleryAggregateTypeList);

    QDocumentGallery::Error result = QDocumentGallery::NoError;

    QString rootItemStatement;
    QString filterStatement;

    if (m_itemIndex != -1 && itemTypes[m_itemIndex].filterFragment)
        filterStatement = QLatin1String(itemTypes[m_itemIndex].filterFragment);

    Q_UNUSED(scope);
    if (!rootItemId.isEmpty()) {
        int index;
        if ((index = itemTypes.indexOfItemId(rootItemId)) != -1) {
            if (itemTypes[index].prefix == QLatin1String("artist::")) {
                rootItemStatement = QLatin1String("{?track nie:isLogicalPartOf ?x}");
                if (!filterStatement.isEmpty())
                    filterStatement += QLatin1String(" && ");
                filterStatement
                        += QLatin1String("(nmm:performer(?track) = <")
                        + itemTypes[index].prefix.strip(rootItemId).toString()
                        + QLatin1String(">)");
            } else {
                rootItemStatement
                        = QLatin1String("{?x nie:isLogicalPartOf <")
                        + itemTypes[index].prefix.strip(rootItemId).toString()
                        + QLatin1String(">}");
            }
        }
        else if ((index = aggregateTypes.indexOfItemId(rootItemId)) != -1) {
            if (aggregateTypes[index].prefix == QLatin1String("audioGenre::")) {
                rootItemStatement = QLatin1String("{?track nie:isLogicalPartOf ?x}");
                if (!filterStatement.isEmpty())
                    filterStatement += QLatin1String(" && ");
                filterStatement
                        += QLatin1String("(nfo:genre(?track) = '")
                        + aggregateTypes[index].prefix.strip(rootItemId).toString()
                        + QLatin1String("' )");
            }
        } else {
            result = QDocumentGallery::ItemIdError;
        }
    }

    if (filter.isValid()) {
        if (!filterStatement.isEmpty())
            filterStatement += QLatin1String(" && ");
        if (m_itemIndex != -1) {
            qt_writeCondition(
                    &result,
                    &filterStatement,
                    filter,
                    itemTypes[m_itemIndex].itemProperties,
                    itemTypes[m_itemIndex].compositeProperties);
        } else if (m_aggregateIndex != -1) {
            qt_writeCondition(
                    &result,
                    &filterStatement,
                    filter,
                    aggregateTypes[m_aggregateIndex].properties,
                    aggregateTypes[m_itemIndex].compositeProperties);
        } else {
            result = QDocumentGallery::ItemTypeError;
        }
    }
    if (result == QDocumentGallery::NoError) {
        *query = rootItemStatement + (filterStatement.isEmpty()
                ? QLatin1String("")
                : QLatin1String("FILTER(") + filterStatement + QLatin1String(")"));
    }
    return result;
}

static QVector<QGalleryTrackerValueColumn *> qt_createValueColumns(
        const QVector<QVariant::Type> &types)
{
    QVector<QGalleryTrackerValueColumn *> columns;

    columns.reserve(types.count());

    for (int i = 0, count = types.count(); i < count; ++i) {
        switch (types.at(i)) {
        case QVariant::String:
            columns.append(new QGalleryTrackerStringColumn);
            break;
        case QVariant::StringList:
            columns.append(new QGalleryTrackerStringListColumn);
            break;
        case QVariant::Int:
            columns.append(new QGalleryTrackerIntegerColumn);
            break;
        case QVariant::Double:
            columns.append(new QGalleryTrackerDoubleColumn);
            break;
        case QVariant::DateTime:
            columns.append(new QGalleryTrackerDateTimeColumn);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }

    return columns;
}

static QString qt_writeSorting(
        const QStringList &x, const QVector<QGalleryTrackerSortCriteria> &sortCriteria)
{
    int count = x.size();
    if (count > 0) {
        QString result(QLatin1String(" ORDER BY "));
        for (int i = 0; i < count; ++i) {
            if (sortCriteria[i].flags & QGalleryTrackerSortCriteria::Ascending ) {
                result += qt_writePropertyFunctions(QStringList()
                        << QLatin1String(" ASC(") + x[i], QLatin1String("x"));
            } else {
                result += qt_writePropertyFunctions(QStringList()
                        << QLatin1String(" DESC(") +x[i], QLatin1String("x"));
            }
        }
        return result;
    }
    return QLatin1String("");
}

void QGalleryTrackerSchema::populateItemArguments(
        QGalleryTrackerResultSetArguments *arguments,
        QGalleryDBusInterfaceFactory *dbus,
        const QString &query,
        const QStringList &propertyNames,
        const QStringList &sortPropertyNames) const
{
    const QString service = qt_galleryItemTypeList[m_itemIndex].service;
    const QString searchText;
    const QStringList keywords;
    const bool sortByService = false;

    QStringList valueNames;
    QStringList aliasNames;
    QStringList compositeNames;
    QStringList sortFieldNames;
    QVector<QGalleryProperty::Attributes> valueAttributes;
    QVector<QGalleryProperty::Attributes> aliasAttributes;
    QVector<QGalleryProperty::Attributes> compositeAttributes;
    QVector<QVariant::Type> valueTypes;
    QVector<QVariant::Type> extendedValueTypes;
    QVector<QVariant::Type> aliasTypes;
    QVector<QVariant::Type> compositeTypes;

    const QGalleryItemPropertyList &itemProperties
            = qt_galleryItemTypeList[m_itemIndex].itemProperties;
    const QGalleryCompositePropertyList &compositeProperties
            = qt_galleryItemTypeList[m_itemIndex].compositeProperties;

    for (QStringList::const_iterator it = propertyNames.begin(); it != propertyNames.end(); ++it) {
        if (valueNames.contains(*it) || aliasNames.contains(*it))
            continue;

        int propertyIndex;

        if ((propertyIndex = itemProperties.indexOfProperty(*it)) >= 0) {
            const QString field = itemProperties[propertyIndex].field;

            int fieldIndex = arguments->fieldNames.indexOf(field);

            if (fieldIndex >= 0) {
                Q_ASSERT( 0 );
                arguments->aliasColumns.append(fieldIndex + 2);
                aliasNames.append(*it);
                aliasAttributes.append(itemProperties[propertyIndex].attributes);
                aliasTypes.append(itemProperties[propertyIndex].type);
            } else {
                arguments->fieldNames.append(field);
                valueNames.append(*it);
                valueAttributes.append(itemProperties[propertyIndex].attributes);
                valueTypes.append(itemProperties[propertyIndex].type);
            }
        }
    }

    for (QStringList::const_iterator it = propertyNames.begin(); it != propertyNames.end(); ++it) {
        if (valueNames.contains(*it) || aliasNames.contains(*it) || compositeNames.contains(*it))
            continue;

        int propertyIndex;
        if ((propertyIndex = compositeProperties.indexOfProperty(*it)) >= 0) {
            const QGalleryItemPropertyList &dependencies
                    = compositeProperties[propertyIndex].dependencies;

            QVector<int> columns;
            for (int i = 0; i < dependencies.count; ++i) {
                const QString field = dependencies[i].field;

                int fieldIndex = arguments->fieldNames.indexOf(field);

                if (fieldIndex >= 0) {
                    columns.append(fieldIndex + 2);
                } else {
                    columns.append(arguments->fieldNames.count() + 2);

                    arguments->fieldNames.append(field);
                    extendedValueTypes.append(dependencies[i].type);
                }
            }

            QGalleryProperty::Attributes attributes = QGalleryProperty::CanRead;
            if (compositeProperties[propertyIndex].writeFilterCondition)
                attributes |= QGalleryProperty::CanFilter;

            compositeNames.append(*it);
            compositeAttributes.append(attributes);
            compositeTypes.append(compositeProperties[propertyIndex].type);
            arguments->compositeColumns.append(
                    compositeProperties[propertyIndex].createColumn(columns));
        }
    }

    bool descending = false;

    for (QStringList::const_iterator it = sortPropertyNames.constBegin();
            it != sortPropertyNames.constEnd();
            ++it) {
        int sortFlags = QGalleryTrackerSortCriteria::Ascending;

        QString propertyName = *it;

        if (propertyName.startsWith(QLatin1Char('-'))) {
            propertyName = propertyName.mid(1);
            sortFlags = QGalleryTrackerSortCriteria::Descending;

            if (arguments->sortCriteria.isEmpty())
                descending = true;

            sortFlags |= descending
                    ? QGalleryTrackerSortCriteria::Sorted
                    : QGalleryTrackerSortCriteria::ReverseSorted;
        } else {
            if (propertyName.startsWith(QLatin1Char('+')))
                propertyName = propertyName.mid(1);

            if (arguments->sortCriteria.isEmpty())
                descending = false;

            sortFlags |= descending
                    ? QGalleryTrackerSortCriteria::ReverseSorted
                    : QGalleryTrackerSortCriteria::Sorted;
        }

        const int propertyIndex = itemProperties.indexOfProperty(propertyName);

        if (propertyIndex >= 0
                && itemProperties[propertyIndex].attributes & QGalleryProperty::CanSort) {
            const QString field = itemProperties[propertyIndex].field;

            int fieldIndex = arguments->fieldNames.indexOf(field);

            if (fieldIndex < 0) {
                fieldIndex = arguments->fieldNames.count();
                arguments->fieldNames.append(field);
                extendedValueTypes.append(itemProperties[propertyIndex].type);
            }

            sortFieldNames.append(field);
            arguments->sortCriteria.append(QGalleryTrackerSortCriteria(fieldIndex + 2, sortFlags));
        }
    }

    Q_UNUSED( sortByService );
    arguments->service = qt_galleryItemTypeList[m_itemIndex].service;
    arguments->updateMask = qt_galleryItemTypeList[m_itemIndex].updateMask;
    arguments->identityWidth = 1;
    arguments->valueOffset = 3;  // urn + nie:url + rdf:type
    arguments->tableWidth =  arguments->valueOffset + arguments->fieldNames.count();
    arguments->compositeOffset = arguments->valueOffset + valueNames.count();
    arguments->queryInterface = dbus->metaDataInterface();
    arguments->queryMethod = QLatin1String("SparqlQuery");
    arguments->queryArguments = QVariantList()
            << QLatin1String("SELECT DISTINCT ?x nie:url(?x) rdf:type(?x) ")
            + qt_writePropertyFunctions(arguments->fieldNames, QLatin1String("x"))
            + QLatin1String(" WHERE {")
            + qt_galleryItemTypeList[m_itemIndex].typeFragment
            + query
            + QLatin1String("}")
            + qt_writeSorting(sortFieldNames, arguments->sortCriteria);

    arguments->idColumn.reset(new QGalleryTrackerServicePrefixColumn);
    arguments->urlColumn.reset(new QGalleryTrackerFileUrlColumn(QGALLERYTRACKERFILEURLCOLUMN_DEFAULT_COL));
    arguments->typeColumn.reset(new QGalleryTrackerServiceTypeColumn);
    arguments->valueColumns = qt_createValueColumns(valueTypes + extendedValueTypes);
    arguments->propertyNames = valueNames + compositeNames + aliasNames;
    arguments->propertyAttributes = valueAttributes + compositeAttributes + aliasAttributes;
    arguments->propertyTypes = valueTypes + compositeTypes + aliasTypes;

    for (int i = 0; i < arguments->propertyAttributes.count(); ++i) {
        if (arguments->propertyAttributes.at(i) & IsResource)
            arguments->resourceKeys.append(i + arguments->valueOffset );
        arguments->propertyAttributes[i] &= PropertyMask;
    }
}

void QGalleryTrackerSchema::populateAggregateArguments(
        QGalleryTrackerResultSetArguments *arguments,
        QGalleryDBusInterfaceFactory *dbus,
        const QString &query,
        const QStringList &propertyNames,
        const QStringList &sortPropertyNames) const
{
    QStringList identityFields;
    QStringList aggregateFields;
    QStringList aggregates;
    QStringList identityNames;
    QStringList aggregateNames;
    QStringList aliasNames;
    QStringList compositeNames;
    QVector<QGalleryProperty::Attributes> identityAttributes;
    QVector<QGalleryProperty::Attributes> aggregateAttributes;
    QVector<QGalleryProperty::Attributes> aliasAttributes;
    QVector<QGalleryProperty::Attributes> compositeAttributes;
    QVector<QVariant::Type> identityTypes;
    QVector<QVariant::Type> aggregateTypes;
    QVector<QVariant::Type> aliasTypes;
    QVector<QVariant::Type> compositeTypes;
    QVector<int> identityColumns;

    const QGalleryAggregateType &type = qt_galleryAggregateTypeList[m_aggregateIndex];
    const QGalleryItemPropertyList &properties = type.properties;
    const QGalleryAggregatePropertyList &aggregateProperties = type.aggregateProperties;
    const QGalleryCompositePropertyList &compositeProperties = type.compositeProperties;

    for (int i = 0; i < type.identity.count; ++i)
        identityNames.append(type.identity[i].name);

    qSort(identityNames.begin(), identityNames.end(), QGalleryReorderLessThan(sortPropertyNames));

    for (int i = 0; i < type.identity.count; ++i)
        identityColumns.append(identityNames.indexOf(type.identity[i].name));

    for (QStringList::const_iterator it = identityNames.constBegin();
            it != identityNames.constEnd();
            ++it) {
        int propertyIndex = properties.indexOfProperty(*it);
        Q_ASSERT(propertyIndex >= 0);

        const QString field = properties[propertyIndex].field;
        Q_ASSERT(identityFields.indexOf(field) == -1);

        identityFields.append(field);
        identityAttributes.append(properties[propertyIndex].attributes & PropertyMask);
        identityTypes.append(properties[propertyIndex].type);
    }

    for (QStringList::const_iterator it = propertyNames.constBegin();
            it != propertyNames.constEnd();
            ++it) {
        if (identityNames.contains(*it)
                || aggregateNames.contains(*it)
                || aliasNames.contains(*it)) {
            continue;
        }

        int propertyIndex = properties.indexOfProperty(*it);

        if (propertyIndex >= 0) {
            const QString field = properties[propertyIndex].field;

            int fieldIndex = identityFields.indexOf(field);
            Q_ASSERT(fieldIndex != -1);

            arguments->aliasColumns.append(fieldIndex);
            aliasNames.append(*it);
            aliasAttributes.append(properties[propertyIndex].attributes & PropertyMask);
            aliasTypes.append(properties[propertyIndex].type);
        } else if ((propertyIndex = aggregateProperties.indexOfProperty(*it)) >= 0) {
            aggregateNames.append(*it);
            aggregateFields.append(aggregateProperties[propertyIndex].field);
            aggregates.append(aggregateProperties[propertyIndex].aggregate);
            aggregateAttributes.append(QGalleryProperty::CanRead);
            aggregateTypes.append(aggregateProperties[propertyIndex].type);
        } else if ((propertyIndex = compositeProperties.indexOfProperty(*it)) >= 0) {
            const QGalleryItemPropertyList &dependencies
                    = compositeProperties[propertyIndex].dependencies;

            QVector<int> columns;
            for (int i = 0; i < dependencies.count; ++i) {
                const QString field = dependencies[i].field;

                int fieldIndex = identityFields.indexOf(field);

                Q_ASSERT(fieldIndex >= 0);

                columns.append(fieldIndex);
            }

            QGalleryProperty::Attributes attributes = QGalleryProperty::CanRead;
            if (compositeProperties[propertyIndex].writeFilterCondition)
                attributes |= QGalleryProperty::CanFilter;

            compositeNames.append(*it);
            compositeAttributes.append(attributes);
            compositeTypes.append(compositeProperties[propertyIndex].type);
            arguments->compositeColumns.append(
                    compositeProperties[propertyIndex].createColumn(columns));
        }
    }

    bool descending = false;

    for (QStringList::const_iterator it = sortPropertyNames.begin();
            it != sortPropertyNames.end();
            ++it) {
        int sortFlags = QGalleryTrackerSortCriteria::Ascending;

        QString propertyName = *it;

        if (propertyName.startsWith(QLatin1Char('-'))) {
            propertyName = propertyName.mid(1);
            sortFlags = QGalleryTrackerSortCriteria::Descending;

            if (arguments->sortCriteria.isEmpty())
                descending = true;

            sortFlags |= descending
                    ? QGalleryTrackerSortCriteria::Sorted
                    : QGalleryTrackerSortCriteria::ReverseSorted;
        } else {
            if (propertyName.startsWith(QLatin1Char('+')))
                propertyName = propertyName.mid(1);

            if (arguments->sortCriteria.isEmpty())
                descending = false;

            sortFlags |= descending
                    ? QGalleryTrackerSortCriteria::ReverseSorted
                    : QGalleryTrackerSortCriteria::Sorted;
        }

        const int propertyIndex = properties.indexOfProperty(propertyName);
        if (propertyIndex >= 0) {
            const QString field = properties[propertyIndex].field;

            const int fieldIndex = identityFields.indexOf(field);
            Q_ASSERT(fieldIndex != -1);

            arguments->sortCriteria.append(QGalleryTrackerSortCriteria(fieldIndex, sortFlags));
        }
    }

    Q_UNUSED( query );
    arguments->service = type.service;
    arguments->updateMask = qt_galleryAggregateTypeList[m_aggregateIndex].updateMask;
    arguments->identityWidth = identityColumns.count();
    arguments->valueOffset = 0;
    arguments->tableWidth = identityColumns.count() + aggregates.count();
    arguments->compositeOffset = arguments->tableWidth;
    arguments->queryInterface = dbus->metaDataInterface();
    arguments->queryMethod = QLatin1String("SparqlQuery");
    arguments->queryArguments = QVariantList()
            << QLatin1String("SELECT DISTINCT ")
            + qt_writePropertyFunctions(identityFields, QLatin1String("x"))
            + QLatin1String(" WHERE{ ?x rdf:type ") + type.service + QLatin1String(" ")
            + query
            + QLatin1String("}");

    if (type.identity.count == 1)
        arguments->idColumn.reset(new QGalleryTrackerPrefixColumn(0, type.prefix));
    else
        arguments->idColumn.reset(new QGalleryTrackerCompositeIdColumn(identityColumns, type.prefix));
    arguments->urlColumn.reset(new QGalleryTrackerStaticColumn(QVariant()));
    arguments->typeColumn.reset(new QGalleryTrackerStaticColumn(type.itemType));
    arguments->valueColumns = qt_createValueColumns(identityTypes + aggregateTypes);
    arguments->propertyNames = identityNames + aggregateNames + compositeNames + aliasNames;
    arguments->propertyAttributes
            = identityAttributes
            + aggregateAttributes
            + compositeAttributes
            + aliasAttributes;
    arguments->propertyTypes = identityTypes + aggregateTypes + compositeTypes + aliasTypes;
}

QString QGalleryTrackerSchema::serviceForType( const QString &galleryType )
{
    QGalleryTypeList<QGalleryItemType> typeList(qt_galleryItemTypeList);
    int index = typeList.indexOfType(galleryType);
    if (index != -1)
        return typeList[index].service;

    QGalleryTypeList<QGalleryAggregateType> aggregateTypeList(qt_galleryAggregateTypeList);
    index = aggregateTypeList.indexOfType(galleryType);
    if (index != -1)
        return aggregateTypeList[index].service;

    qWarning() << galleryType << " does not exists";
    return "";
}

QTM_END_NAMESPACE