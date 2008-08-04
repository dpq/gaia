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
#define OPERATOR

#include <QtCore/QMap>
#include <QtCore/QHash>
#include <QtXml/QDomDocument>
class QDomElement;
class QString;
class QPixmap;

class GaiaCore : public QObject {
Q_OBJECT

/* Zones */
public:
	void openZoneFile(const QString &path);
#ifdef OPERATOR
	void saveZoneFile(const QString &path);
#endif

	QString zoneUrl() const;
#ifdef OPERATOR
public slots:
	void setZoneUrl(const QString &url);
#endif

public:
	QDomElement zone(int zoneId) const;
#ifdef OPERATOR
public slots:
	QDomElement createZone(const QString &name);
	void mountZone(const QDomElement &zone, int parentId);
	void deleteZone(int zoneId);
#endif

public:
	QString zoneName(int zoneId) const;
	QMap<QString, QString> chapterLayout(int zoneId, bool listedOnly = false) const;
#ifdef OPERATOR
public slots:
	void setZoneName(int zoneId, const QString &name);
	void setChapterLayout(int zoneId, const QMap<QString, QString> &layout);
#endif

/* Taxonomy */
public:
	enum Taxonomy { Species, Genus, Family, Order, Class, Phylum, Kingdom, Domain };

	void openTaxonomyFile(const QString &path);
#ifdef OPERATOR
public slots:
	void saveTaxonomyFile(const QString &path);
#endif

/*HACK*/

public:
	QDomElement taxonomyDocumentElement();
	QList<QDomElement> taxonomyElementsByTagName(const QString &tagName, const QDomElement &parent = QDomElement());
//	QDomElement taxonomyElementById(const QString &id);

	QDomElement zoneDocumentElement();
	QList<QDomElement> zoneElementsByTagName(const QString &tagName, const QDomElement &parent = QDomElement());
//	QDomElement zoneElementById(const QString &id);
/*============*/

public:
	QString taxonomyUrl() const;
#ifdef OPERATOR
public slots:
	void setTaxonomyUrl(const QString &url);
#endif

public:
	QDomElement taxonomyEntry(int entryId) const;
#ifdef OPERATOR
public slots:
	QDomElement createTaxonomyEntry(Taxonomy level, const QString &rus, const QString &lat, const QString &picturePath = "");
	void mountTaxonomyEntry(const QDomElement &entry, int parentId);
	void deleteTaxonomyEntry(int entryId);
#endif

public:
	QString entryLatName(int entryId) const;
	QString entryRusName(int entryId) const;
	QPixmap entryPicture(int entryId) const;
	QString entryAuthor(int entryId) const;
	QString entryYear(int entryId) const;


#ifdef OPERATOR
public slots:
	void setEntryLatName(int entryId, const QString &name);
	void setEntryRusName(int entryId, const QString &name);
	void setEntryPicture(int entryId, const QString &picturePath);
#endif

/* TODO Scientific and judicial papers
public:
	QString referenceAuthor(int referenceId);
	int referenceYear(int referenceId);
	QString referenceInfo(int referenceId);
	int referenceSection(int referenceId);

	QString lawTitle(int lawId);
	QString lawBody(int lawId);

#ifdef OPERATOR
public slots:
	int createReference(const QString &author, const QString &year, const QString &info);
	void deleteReference(int referenceId);

	int createLaw(const QString &title, const QString &body);
	void deleteLaw(int lawId);

	void setReferenceAuthor(int referenceId, const QString &author);
	void setReferenceYear(int referenceId, int year);
	void setReferenceInfo(int referenceId, const QString &info);
	void setReferenceSection(int referenceId, int sectionId);

	void setLawTitle(int lawId, const QString &title);
	void setLawBody(int lawId, const QString &body);
#endif
*/

/* Species */
public:
	//TODO QList<int> speciesReferences(int speciesId, int zoneId) const;
	//TODO QList<int> speciesLaws(int speciesId, int zoneId) const;
	bool speciesEnabled(int speciesId, int zoneId) const;
	QPixmap speciesAreal(int speciesId, int zoneId) const;
	QList<int> speciesStatus(int speciesId, int zoneId) const;
	QString speciesChapter(int speciesId, int zoneId, const QString &chapterName) const;

#ifdef OPERATOR
public slots:
	//TODO void setSpeciesReferences(int speciesId, int zoneId, const QList<int> &references);
	//TODO void setSpeciesLaws(int speciesId, int zoneId, const QList<int> &laws);
	void setSpeciesEnabled(int speciesId, int zoneId, bool isEnabled);
	void setSpeciesAreal(int speciesId, int zoneId, const QString &path);
	void setSpeciesStatus(int speciesId, int zoneId, const QList<int> &status);
	void setSpeciesChapter(int speciesId, int zoneId, const QString &chapterName, const QString &chapterHtml);
#endif

public:
	GaiaCore();
	~GaiaCore();

private:
	QDomDocument zoneDoc, taxonomyDoc;
	int maxZoneId, maxTaxonomyId;
	QHash<int, QDomElement> *zoneHash, *taxonomyHash;
	QDomElement elementById(const QDomElement &element, const QString &id);
	QList<QDomElement> allElements(const QDomElement &element, const QString &tagName = "");
	void deleteDirectory(const QString &path);
	static const QString taxonomyLevels[];
};

/**
Zones

<?xml version="1.0" encoding="utf8"?>
<geo version="1.0" url="URL">
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
<bio version="1.0" url="URL">
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
areal.png 001.html 002.html bomm.html lit.txt
*/

/**
References

In each species directory there can be a lit.txt file, which contains IDs of published works used, separated by commas. Full details on the works they reference are kept in the appropriate XML file.

lit.txt
1,4,87

<?xml version="1.0" encoding="utf8"?>
<lit version="1.0">
<section bio="3">
<reference id="1" author="Ivanov" year="1947" info="Moscow, FIZMATGIZ" />
<reference id="4" author="Petruccio et al" year="1920" info="Astrobiology review,vol.23.issue 12, pg.117-125" />
</section>
<section bio="5,3">
<reference id="87" author="Sidoradze, Kalaxadze" year="2002" info="Georgian biologist, 2002 Tbilisi" />
</section>
</lit>
*/
#endif
