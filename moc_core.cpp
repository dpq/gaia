/****************************************************************************
** Meta object code from reading C++ file 'core.h'
**
** Created: Sun Jun 29 15:21:19 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "core.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'core.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GaiaCore[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   10,    9,    9, 0x0a,
      51,   46,   34,    9, 0x0a,
      85,   71,    9,    9, 0x0a,
     119,  112,    9,    9, 0x0a,
     147,  135,    9,    9, 0x0a,
     186,  172,    9,    9, 0x0a,
     235,  230,    9,    9, 0x0a,
     261,   10,    9,    9, 0x0a,
     311,  285,   34,    9, 0x0a,
     379,  365,   34,    9, 0x2a,
     440,  425,    9,    9, 0x0a,
     484,  476,    9,    9, 0x0a,
     522,  509,    9,    9, 0x0a,
     551,  509,    9,    9, 0x0a,
     600,  580,    9,    9, 0x0a,
     656,  629,    9,    9, 0x0a,
     710,  688,    9,    9, 0x0a,
     767,  743,    9,    9, 0x0a,
     845,  804,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GaiaCore[] = {
    "GaiaCore\0\0url\0setZoneUrl(QString)\0"
    "QDomElement\0name\0createZone(QString)\0"
    "zone,parentId\0mountZone(QDomElement,int)\0"
    "zoneId\0deleteZone(int)\0zoneId,name\0"
    "setZoneName(int,QString)\0zoneId,layout\0"
    "setChapterLayout(int,QMap<QString,QString>)\0"
    "path\0saveTaxonomyFile(QString)\0"
    "setTaxonomyUrl(QString)\0"
    "level,rus,lat,picturePath\0"
    "createTaxonomyEntry(Taxonomy,QString,QString,QString)\0"
    "level,rus,lat\0"
    "createTaxonomyEntry(Taxonomy,QString,QString)\0"
    "entry,parentId\0mountTaxonomyEntry(QDomElement,int)\0"
    "entryId\0deleteTaxonomyEntry(int)\0"
    "entryId,name\0setEntryLatName(int,QString)\0"
    "setEntryRusName(int,QString)\0"
    "entryId,picturePath\0setEntryPicture(int,QString)\0"
    "speciesId,zoneId,isEnabled\0"
    "setSpeciesEnabled(int,int,bool)\0"
    "speciesId,zoneId,path\0"
    "setSpeciesAreal(int,int,QString)\0"
    "speciesId,zoneId,status\0"
    "setSpeciesStatus(int,int,QList<int>)\0"
    "speciesId,zoneId,chapterName,chapterHtml\0"
    "setSpeciesChapter(int,int,QString,QString)\0"
};

const QMetaObject GaiaCore::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GaiaCore,
      qt_meta_data_GaiaCore, 0 }
};

const QMetaObject *GaiaCore::metaObject() const
{
    return &staticMetaObject;
}

void *GaiaCore::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GaiaCore))
	return static_cast<void*>(const_cast< GaiaCore*>(this));
    return QObject::qt_metacast(_clname);
}

int GaiaCore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setZoneUrl((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: { QDomElement _r = createZone((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDomElement*>(_a[0]) = _r; }  break;
        case 2: mountZone((*reinterpret_cast< const QDomElement(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: deleteZone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setZoneName((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: setChapterLayout((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QMap<QString,QString>(*)>(_a[2]))); break;
        case 6: saveTaxonomyFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: setTaxonomyUrl((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: { QDomElement _r = createTaxonomyEntry((*reinterpret_cast< Taxonomy(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDomElement*>(_a[0]) = _r; }  break;
        case 9: { QDomElement _r = createTaxonomyEntry((*reinterpret_cast< Taxonomy(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDomElement*>(_a[0]) = _r; }  break;
        case 10: mountTaxonomyEntry((*reinterpret_cast< const QDomElement(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: deleteTaxonomyEntry((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: setEntryLatName((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 13: setEntryRusName((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 14: setEntryPicture((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 15: setSpeciesEnabled((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 16: setSpeciesAreal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 17: setSpeciesStatus((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QList<int>(*)>(_a[3]))); break;
        case 18: setSpeciesChapter((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        }
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
