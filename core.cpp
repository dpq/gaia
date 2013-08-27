/***************************************************************************
 *   core.cpp                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#include "core.h"
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QtGui/QApplication>
#include <QtGui/QPixmap>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtDebug>

GaiaCore::~GaiaCore() {
	delete zoneHash;
	delete taxonomyHash;
    delete zoneDoc;
    delete taxonomyDoc;
    qDeleteAll(statusHash->values());
    delete statusHash;
}

QList<int> GaiaCore::taxonomyElements(const QString &tagName, const QDomElement &parent) {
    QDomNodeList res = (parent == QDomElement() ? taxonomyDoc->documentElement() : parent).elementsByTagName(tagName);
    QList<int> ids;
    for (int i=0; i < res.size(); i++) {
        ids.append(res.at(i).toElement().attribute("id").toInt());
    }
    return ids;
}

QDomNodeList GaiaCore::zoneElements(const QString &tagName, const QDomElement &parent) {
    return (parent == QDomElement() ? zoneDoc->documentElement() : parent).elementsByTagName(tagName);
}

void GaiaCore::readZoneFile(const QString &path) {
    readXML(path, "zoneDocument", "zone", &zoneDoc, &zoneHash);
    statusHash = new QHash<int, QHash<int, QString> *>();
    foreach (int zoneId, zoneHash->keys()) {
        QHash<int, QString> *h = new QHash<int, QString>();
        statusHash->insert(zoneId, h);
        QDomNodeList species = zoneElements("species", zoneEntry(zoneId));
        for (int i=0; i < species.size(); i++) {
            QDomElement element = species.at(i).toElement();
            h->insert(element.attribute("id").toInt(), element.attribute("status"));
        }
    }
}

void GaiaCore::readTaxonomyFile(const QString &path) {
    readXML(path, "taxonomyDocument", "", &taxonomyDoc, &taxonomyHash);
}

void GaiaCore::readXML(const QString &path, const QString &rootName, const QString &tagName, QDomDocument **xmlDoc, QHash<int, QDomElement> **xmlHash) {
    *xmlDoc = new QDomDocument(rootName);
    *xmlHash = new QHash<int, QDomElement>();
    QFile file(path);
    QString xml = "";
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    for (QString line = file.readLine(); !file.atEnd(); line = QString::fromUtf8(file.readLine())) {
        xml.append(line);
    }
    file.close();
    (*xmlDoc)->setContent(xml);
    QList<QDomElement> entries = allDescendants((*xmlDoc)->documentElement(), tagName);
    for (int i = 0; i < entries.size(); i++) {
        int entryId = entries.at(i).attribute("id").toInt();
        (*xmlHash)->insert(entryId, entries.at(i));
    }
}

QDomElement GaiaCore::zoneEntry(int zoneId) const {
	return zoneHash->value(zoneId);
}

QDomElement GaiaCore::taxonomyEntry(int entryId) const {
    return taxonomyHash->value(entryId);
}

const QString GaiaCore::taxonomyLevels[] = { "species", "genus", "family", "order", "class", "phylum", "kingdom", "domain" };

QString GaiaCore::entryLatName(int entryId) const {
    return this->entryAttribute(entryId, QString("lat"));
}

QString GaiaCore::entryRusName(int entryId) const {
    return this->entryAttribute(entryId, QString("rus"));
}

QString GaiaCore::speciesAuthor(int entryId) const {
    return this->entryAttribute(entryId, QString("author"));
}

QString GaiaCore::speciesComment(int entryId) const {
    return this->entryAttribute(entryId, QString("comment"));
}

QString GaiaCore::speciesYear(int entryId) const {
    return this->entryAttribute(entryId, QString("year"));
}

QString GaiaCore::entryAttribute(int entryId, const QString &attribute) const {
    if (taxonomyEntry(entryId) == QDomElement()) {
        return QString();
    }
    return taxonomyEntry(entryId).attribute(attribute);
}

QString GaiaCore::zoneName(int zoneId) const {
    if (zoneEntry(zoneId) == QDomElement()) {
        return QString();
    }
    return zoneEntry(zoneId).attribute("name");
}

QPixmap GaiaCore::speciesPicture(int entryId, int zoneId) const {
    return this->speciesPixmap(entryId, zoneId, "picture");
}

QPixmap GaiaCore::speciesAreal(int entryId, int zoneId) const {
    return this->speciesPixmap(entryId, zoneId, "areal");
}

QPixmap GaiaCore::speciesPixmap(int entryId, int zoneId, const QString &filename) const {
    if (taxonomyEntry(entryId) == QDomElement()) {
        return QPixmap();
    }
    return QPixmap(QString("%1/species/%2/%3/%4.png").arg(qApp->applicationDirPath()).arg(zoneId).arg(entryId).arg(filename));
}

QList<int> GaiaCore::speciesZones(int entryId) {
    QList<int> res;
    QDomNodeList zoneList = zoneElements("zone", QDomElement());
    for (int i = 0; i < zoneList.size(); i++) {
        QString zoneId = zoneList.at(i).toElement().attribute("id");
        if (QFile::exists(QString("%1/species/%2/%3").arg(qApp->applicationDirPath()).arg(zoneId).arg(entryId))) {
                    res.append(zoneId.toInt());
                }
    }
	return res;
}

QList<int> GaiaCore::speciesStatus(int speciesId, int zoneId) const {
	QList<int> res;
    if (taxonomyEntry(speciesId) == QDomElement() || zoneEntry(zoneId) == QDomElement()) {
		return QList<int>();
    }
    QString str = statusHash->value(zoneId)->value(speciesId);
	QList<QString> splitted = str.split(",");
	foreach(QString status, splitted) {
		res.append(status.toInt());
	}
	return res;
}

QString GaiaCore::speciesChapter(int speciesId, int zoneId, const QString &chapterName) const {
    if (taxonomyEntry(speciesId) == QDomElement() || zoneEntry(zoneId) == QDomElement()) {
		return QString();
    }
    QFile file(QString("%1/species/%2/%3/%4").arg(qApp->applicationDirPath())\
               .arg(zoneId).arg(speciesId).arg(chapterLayout(zoneId).value(chapterName)));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
	QString res = QString::fromUtf8(file.readAll());
	file.close();
	return res;
}

QMap<QString, QString> GaiaCore::chapterLayout(int zoneId, bool listedOnly) const {
    if (this->zoneEntry(zoneId) == QDomElement()) {
        return QMap<QString, QString>();
    }
    QDomElement zone = this->zoneEntry(zoneId);
    QMap<QString, QString> res;
    QDomNodeList chapters = zone.childNodes();
    for (int i = 0; i< chapters.size(); i++) {
        QDomElement chapter = chapters.at(i).toElement();
        if (chapter.tagName() == "chapter") {
            if (listedOnly == false || chapter.attribute("listed") != "false") {
                res.insert(chapter.attribute("name"), chapter.attribute("file"));
            }
        }
    }
    return res;
}

QList<QDomElement> GaiaCore::allDescendants(const QDomElement &element, const QString &tagName) {
    QList<QDomElement> res = QList<QDomElement>();
    if (tagName == "" || element.tagName() == tagName)
        res.append(element);
    for (int i = 0; i < element.childNodes().size(); i++) {
        res << allDescendants(element.childNodes().at(i).toElement(), tagName);
    }
    return res;
}
