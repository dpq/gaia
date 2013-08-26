/***************************************************************************
 *   core.h                                                                *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QHash>
#include <QtXml/QDomDocument>
class QDomElement;
class QString;
class QPixmap;

class GaiaCore : public QObject {
Q_OBJECT

public:
    void readZoneFile(const QString &path);
    void readTaxonomyFile(const QString &path);

    QDomNodeList zoneElements(const QString &tagName, const QDomElement &parent = QDomElement());
    QList<int> taxonomyElements(const QString &tagName, const QDomElement &parent = QDomElement());
    QDomElement zoneEntry(int zoneId) const;
    QDomElement taxonomyEntry(int entryId) const;

	enum Taxonomy { Species, Genus, Family, Order, Class, Phylum, Kingdom, Domain };
    static const QString taxonomyLevels[];

    QString zoneName(int zoneId) const;
    QString entryLatName(int entryId) const;
    QString entryRusName(int entryId) const;
    QString speciesAuthor(int entryId) const;
    QString speciesYear(int entryId) const;
    QString speciesComment(int entryId) const;
    QPixmap speciesAreal(int entryId, int zoneId) const;
    QPixmap speciesPicture(int entryId, int zoneId) const;
    QList<int> speciesStatus(int entryId, int zoneId) const;
    QList<int> speciesZones(int entryId);
    QMap<QString, QString> chapterLayout(int zoneId, bool listedOnly = false) const;
    QString speciesChapter(int entryId, int zoneId, const QString &chapterName) const;
	~GaiaCore();

private:
    void readXML(const QString &path, const QString &rootName, const QString &tagName, QDomDocument **xmlDoc, QHash<int, QDomElement> **xmlHash);
    QList<QDomElement> allDescendants(const QDomElement &element, const QString &tagName = "");
    QString entryAttribute(int entryId, const QString &attribute) const;
    QPixmap speciesPixmap(int entryId, int zoneId, const QString &filename) const;
    QDomDocument *zoneDoc, *taxonomyDoc;
	QHash<int, QDomElement> *zoneHash, *taxonomyHash;
    QHash<int, QHash<int, QString> *> *statusHash;
};

/**
Zones

<?xml version="1.0" encoding="utf8"?>
<geo version="1.0" path="PATH">
<zone id="1" name="Russian Federation">
	<zone id="2" name="Kamchatkan region">
		<chapter file="002.html" name="Overview" />
	</zone>
	<zone id="3" name="Tatarstan republic" />
</zone>
</geo>

The root element GEO specifies the version of the file format and the path to the directory where zone-specific directories are located. Each zone has an ID to manage mounting and a name [multiple languages are not supported at this moment]. Although names of two zones with a common parent should generally be unique, name and ID have been separated to simplify zone renaming. Since as a rule zones are compiled one at a time and all species in a zone have similar chapter count and naming [see below], this information is also given in the zone description.
*/

/**
Taxonomy

<?xml version="1.0" encoding="utf8"?>
<bio version="1.0" path="PATH">
<family id="5" rus="Semeystvo #6" lat="Family #6">
	<genus id="4" rus="Rod #15" lat="Genus #15">
		<species id="168" rus="Shmel #1" lat="Bombus paradoxus" />
		<species id="169" rus="Shmel #2" lat="Bombus armenicus" />
	</genus>
</family>
</bio>

The XML structure repeats Linnean taxonometry (Species, Genus, Family, Order, Class, Phylum, Kingdom, Domain), but may start at any level, rather than always start at Domain. URL points to a directory where image files for entries are located. The basename of the picture file (without the suffix) must be equal to the ID of the entry it corresponds to.

species/
5.png 168.jpg 169.jpg
*/


/**
Species

Each zone has a number of species entries associated with it. Every species has a directory under this zone's directory, which contains its areal in this zone and  description chapters. A chapter file is essentially a html fragment. Since various zones can have various types of chapters and, which is more important, the data can be compiled independently.

115/
areal.png 001.txt 002.txt 003.txt
*/
#endif
