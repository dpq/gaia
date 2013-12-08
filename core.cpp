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
#include <QtCore/QByteArray>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtDebug>

GaiaCore::GaiaCore() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString("%1/entries.sqlite").arg(qApp->applicationDirPath()));
    db.open();
}

GaiaCore::~GaiaCore() {
	delete zoneHash;
	delete taxonomyHash;
    delete zoneDoc;
    delete taxonomyDoc;
    qDeleteAll(statusHash->values());
    delete statusHash;
    db.close();
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
    QSqlQuery query;
    QString media = QString(filename);
    media.replace(".png","");
    query.prepare("SELECT " + media +" FROM media WHERE zoneId=? and speciesId=?");
    query.addBindValue(zoneId);
    query.addBindValue(entryId);
    query.exec();
    query.next();
    QByteArray ba = query.value(0).toByteArray();
    QPixmap res = QPixmap();
    res.loadFromData(ba);
    return res;
}

QList<int> GaiaCore::speciesZones(int entryId) {
    QList<int> res;
    QDomNodeList zoneList = zoneElements("zone", QDomElement());
    QSqlQuery query;
    query.prepare("SELECT DISTINCT zoneId FROM chapter WHERE speciesId=?");
    query.addBindValue(entryId);
    query.exec();
    while (query.next()) {
        res.append(query.value(0).toInt());
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

    QSqlQuery query;
    query.prepare("SELECT chapterText FROM chapter WHERE zoneId=? AND speciesId=? AND chapterName=?");
    query.addBindValue(zoneId);
    query.addBindValue(speciesId);
    query.addBindValue(chapterName);
    query.exec();
    if (query.next()) {
        return query.value(0).toString();
    }
    else {
        return QString("");
    }
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
