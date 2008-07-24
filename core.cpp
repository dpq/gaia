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
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>

GaiaCore::GaiaCore() : QObject() {
	maxZoneId = -1;
	maxTaxonomyId = -1;
	zoneHash = new QHash<int, QDomElement>();
	taxonomyHash = new QHash<int, QDomElement>();
}

GaiaCore::~GaiaCore() {
	delete zoneHash;
	delete taxonomyHash;
}

QDomElement GaiaCore::taxonomyDocumentElement() {
	return taxonomyDoc.documentElement();
}

QList<QDomElement> GaiaCore::taxonomyElementsByTagName(const QString &tagName, const QDomElement &parent) {
	return allElements((parent == QDomElement() ? taxonomyDoc.documentElement() : parent), tagName);
}

QDomElement GaiaCore::taxonomyElementById(const QString &id) {
	return elementById(taxonomyDoc.documentElement(), id);
}

QDomElement GaiaCore::zoneDocumentElement() {
	return zoneDoc.documentElement();
}

QList<QDomElement> GaiaCore::zoneElementsByTagName(const QString &tagName, const QDomElement &parent) {
	return allElements((parent == QDomElement() ? zoneDoc.documentElement() : parent), tagName);
}

QDomElement GaiaCore::zoneElementById(const QString &id) {
	return elementById(zoneDoc.documentElement(), id);
}

void GaiaCore::deleteDirectory(const QString &path) {
	QDir dir(path);
	QList<QString> entries = dir.entryList(QDir::Files);
	for (QList<QString>::iterator i = entries.begin(); i != entries.end(); i++) {
		QString currentPath = QString("%1/%2").arg(path).arg(*i);
		QFileInfo info(currentPath);
		if (info.isDir())
			deleteDirectory(path + "/" + *i);
		else
			QFile::remove(path + "/" + *i);
	}
	dir.rmdir(path);
}

void GaiaCore::openZoneFile(const QString &path) {
	zoneDoc = QDomDocument("zoneDocument");
	zoneHash->clear();
	maxZoneId = -1;
	QFile file(path);
	QString xml = "";
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
        for (QString line = file.readLine(); !file.atEnd(); line = QString::fromUtf8(file.readLine()))
                xml.append(line);
	file.close();
	zoneDoc.setContent(xml);
	QDomNodeList zones = zoneDoc.elementsByTagName("zone");
	for (int i = 0; i< zones.size(); i++) {
		int zoneId = zones.at(i).toElement().attribute("id").toInt();
		maxZoneId = zoneId > maxZoneId ? zoneId : maxZoneId;
		zoneHash->insert(zoneId, zones.at(i).toElement());
	}
}

#ifdef OPERATOR
void GaiaCore::saveZoneFile(const QString &path) {
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly))
		return;
	file.write(zoneDoc.toString().toUtf8());
	file.close();
}
#endif

QString GaiaCore::zoneUrl() const {
	return zoneDoc.documentElement().attribute("url");
}

#ifdef OPERATOR
void GaiaCore::setZoneUrl(const QString &url) {
	zoneDoc.documentElement().setAttribute("url", url);
}
#endif

QDomElement GaiaCore::zone(int zoneId) const {
	return zoneHash->value(zoneId);
}

#ifdef OPERATOR
QDomElement GaiaCore::createZone(const QString &name) {
	QDomElement zone = zoneDoc.createElement("zone");
	zone.setAttribute("id", QString::number(++maxZoneId));
	zone.setAttribute("name", name);
	QDir dir(zoneUrl());
	dir.mkdir(zone.attribute("id"));
	if (dir.exists(zone.attribute("id")))
		zoneHash->insert(zone.attribute("id").toInt(), zone);
		return zone;
	maxZoneId--;
	return QDomElement();
}

void GaiaCore::mountZone(const QDomElement &zone, int parentId) {
	if (this->zone(parentId) != QDomElement())
		this->zone(parentId).appendChild(zone);
}

void GaiaCore::deleteZone(int zoneId) {
	if (this->zone(zoneId) == QDomElement())
		return;
	QDomElement zone = this->zone(zoneId);
	QDomElement parent = zone.parentNode().toElement();
	parent.removeChild(zone);
	deleteDirectory(QString("%1/%2").arg(zoneUrl()).arg(zoneId));
}
#endif

QString GaiaCore::zoneName(int zoneId) const {
	if (zone(zoneId) == QDomElement())
		return QString();
	return zone(zoneId).attribute("name");
}

QMap<QString, QString> GaiaCore::chapterLayout(int zoneId) const {
	if (this->zone(zoneId) == QDomElement())
		return QMap<QString, QString>();
	QDomElement zone = this->zone(zoneId);
	QMap<QString, QString> res;
	res.clear();
	QDomNodeList chapters = zone.childNodes();
	for (int i = 0; i< chapters.size(); i++) {
		QDomElement chapter = chapters.at(i).toElement();
		if (chapter.tagName() == "chapter")
			res.insert(chapter.attribute("name"), chapter.attribute("file"));
	}
	return res;
}

#ifdef OPERATOR
void GaiaCore::setZoneName(int zoneId, const QString &name) {
	if (zone(zoneId) == QDomElement())
		return;
	zone(zoneId).setAttribute("name", name);
}

void GaiaCore::setChapterLayout(int zoneId, const QMap<QString, QString> &layout) {
	if (this->zone(zoneId) == QDomElement())
		return;
	QDomElement zone = this->zone(zoneId);
	int size = zone.childNodes().size();
	for (int i = 0; i < size; i++) {
		if (zone.childNodes().at(i).toElement().tagName() == "chapter") {
			zone.removeChild(zone.childNodes().at(i));
			size--;
		}
	}
	for (QMap<QString, QString>::const_iterator i = layout.begin(); i != layout.end(); i++) {
		QDomElement chapter = zoneDoc.createElement("chapter");
		chapter.setAttribute("name", i.key());
		chapter.setAttribute("file", i.value());
		zone.appendChild(chapter);
	}
}
#endif

const QString GaiaCore::taxonomyLevels[] = { "species", "genus", "family", "order", "class", "phylum", "kingdom", "domain" };

void GaiaCore::openTaxonomyFile(const QString &path) {
	taxonomyDoc = QDomDocument("taxonomyDocument");
	taxonomyHash->clear();
	maxTaxonomyId = -1;
	QFile file(path);
	QString taxonomy = "";
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
        for (QString line = file.readLine(); !file.atEnd(); line = QString::fromUtf8(file.readLine()))
                taxonomy.append(line);
	file.close();
	taxonomyDoc.setContent(taxonomy);
	QList<QDomElement> entries = allElements(taxonomyDoc.documentElement());
	for (QList<QDomElement>::iterator i = entries.begin(); i != entries.end(); i++) {
		int entryId = (*i).toElement().attribute("id").toInt();
		maxTaxonomyId = entryId > maxTaxonomyId ? entryId : maxTaxonomyId;
		taxonomyHash->insert(entryId, (*i).toElement());
	}
}

#ifdef OPERATOR
void GaiaCore::saveTaxonomyFile(const QString &path) {
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly))
		return;
	file.write(taxonomyDoc.toString().toUtf8());
	file.close();
}
#endif

QString GaiaCore::taxonomyUrl() const {
	return taxonomyDoc.documentElement().attribute("url");
}

#ifdef OPERATOR
void GaiaCore::setTaxonomyUrl(const QString &url) {
	taxonomyDoc.documentElement().setAttribute("url", url);
}
#endif

QDomElement GaiaCore::taxonomyEntry(int entryId) const {
	return taxonomyHash->value(entryId);
}

#ifdef OPERATOR
QDomElement GaiaCore::createTaxonomyEntry(Taxonomy level, const QString &rus, const QString &lat, const QString &picturePath) {
	QDomElement entry = taxonomyDoc.createElement(taxonomyLevels[level]);
	entry.setAttribute("id", QString::number(++maxTaxonomyId));
	entry.setAttribute("rus", rus);
	entry.setAttribute("lat", lat);
	if (QFile::copy(picturePath, QString("%1/%2.png").arg(taxonomyUrl()).arg(entry.attribute("id"))) || QFile::exists(QString("%1/%2.png").arg(taxonomyUrl()).arg(entry.attribute("id"))))
		return entry;
	maxTaxonomyId--;
	return QDomElement();
}

void GaiaCore::mountTaxonomyEntry(const QDomElement &entry, int parentId) {
	if (taxonomyEntry(parentId) == QDomElement())
		return;
	taxonomyEntry(parentId).appendChild(entry);
}

void GaiaCore::deleteTaxonomyEntry(int entryId) {
	if (taxonomyEntry(entryId) == QDomElement())
		return;
	QDomElement entry = taxonomyEntry(entryId);
	QDomElement parent = entry.parentNode().toElement();
	parent.removeChild(entry);
	QFile::remove(QString("%1/%2.png").arg(taxonomyUrl()).arg(entryId));
	QDir dir(zoneUrl());
	QList<QString> zoneDirs = dir.entryList(QDir::Dirs);
	for (QList<QString>::iterator i = zoneDirs.begin(); i != zoneDirs.end(); i++) {
		deleteDirectory(QString("%1/%2/%3").arg(zoneUrl()).arg(*i).arg(entryId));
	}
}
#endif

QString GaiaCore::entryLatName(int entryId) const {
	if (taxonomyEntry(entryId) == QDomElement())
		return QString();
	return taxonomyEntry(entryId).attribute("lat");
}

QString GaiaCore::entryRusName(int entryId) const {
	if (taxonomyEntry(entryId) == QDomElement())
		return QString();
	return taxonomyEntry(entryId).attribute("rus");
}

QPixmap GaiaCore::entryPicture(int entryId) const {
	if (taxonomyEntry(entryId) == QDomElement())
		return QPixmap();
	return QPixmap(QString("%1/%2.png").arg(taxonomyUrl()).arg(entryId));
}

#ifdef OPERATOR
void GaiaCore::setEntryLatName(int entryId, const QString &name) {
	if (taxonomyEntry(entryId) == QDomElement())
		return;
	taxonomyEntry(entryId).setAttribute("lat", name);
}

void GaiaCore::setEntryRusName(int entryId, const QString &name) {
	if (taxonomyEntry(entryId) == QDomElement())
		return;
	taxonomyEntry(entryId).setAttribute("rus", name);
}

void GaiaCore::setEntryPicture(int entryId, const QString &picturePath) {
	if (taxonomyEntry(entryId) == QDomElement())
		return;
	QFile::copy(picturePath, QString("%1/%2.png").arg(taxonomyUrl()).arg(entryId));
}
#endif

/* QList<int> GaiaCore::speciesReferences(int speciesId, int zoneId) const {
	QList<int> res;
	if (taxonomyEntry(speciesId) == QDomElement() || zone(zoneId) == QDomElement())
		return QList<int>();
	QString referencesPath = QString("%1/%2/%3/lit.txt").arg(zoneUrl()).arg(zoneId).arg(speciesId);
	QFile file(referencesPath);
	if (!file.open(QIODevice::ReadOnly))
		return QList<int>();
	QString str = file.readAll();
	file.close();
	QList<QString> splitted = str.split(",");
	foreach(QString status, splitted) {
		res.append(status.toInt());
	}
	return res;
} */

bool GaiaCore::speciesEnabled(int speciesId, int zoneId) const {
	return !QFile::exists(QString("%1/%2/%3/disabled").arg(zoneUrl()).arg(zoneId).arg(speciesId));
}

QPixmap GaiaCore::speciesAreal(int speciesId, int zoneId) const {
	return QPixmap(zoneDoc.documentElement().attribute("url") + "/" + QString::number(zoneId) + "/" + QString::number(speciesId) + "/areal.png");
}

QList<int> GaiaCore::speciesStatus(int speciesId, int zoneId) const {
	QList<int> res;
	if (taxonomyEntry(speciesId) == QDomElement() || zone(zoneId) == QDomElement())
		return QList<int>();
	QString statusPath = QString("%1/%2/%3/status.txt").arg(zoneUrl()).arg(zoneId).arg(speciesId);
	if (!QFile::exists(statusPath))
		return QList<int>();
	QFile file(statusPath);
	if (!file.open(QIODevice::ReadOnly))
		return QList<int>();
	QString str = file.readAll();
	file.close();
	QList<QString> splitted = str.split(",");
	foreach(QString status, splitted) {
		res.append(status.toInt());
	}
	return res;
}

QString GaiaCore::speciesChapter(int speciesId, int zoneId, const QString &chapterName) const {
	if (taxonomyEntry(speciesId) == QDomElement() || zone(zoneId) == QDomElement())
		return QString();
	QFile file(QString("%1/%2/%3/%4.html").arg(zoneUrl()).arg(zoneId).arg(speciesId).arg(chapterLayout(zoneId).value(chapterName)));
	if (!file.open(QIODevice::ReadOnly))
		return QString();
	QString res = file.readAll();
	file.close();
	return res;
}

#ifdef OPERATOR
/* void GaiaCore::setSpeciesReferences(int speciesId, int zoneId, const QList<int> &references) {
	if (taxonomyEntry(speciesId) == QDomElement() || zone(zoneId) == QDomElement())
		return;
	QDir dir(QString("%1/%2").arg(zoneUrl()).arg(zoneId));
	if (!dir.exists(QString::number(speciesId))) {
		if (!dir.mkdir(QString::number(speciesId)))
			return;
	}
	QFile file(QString("%1/%2/%3/lit.txt").arg(zoneUrl()).arg(zoneId).arg(speciesId));
	if (!file.open(QIODevice::WriteOnly))
		return;
	for (int i = 0; i < references.size() - 1; i++)
		file.write(QString("%1,").arg(references.at(i)).toUtf8());
	file.write(QString::number(references.last()).toUtf8());
	file.close();
} */

void GaiaCore::setSpeciesEnabled(int speciesId, int zoneId, bool isEnabled) {
	if (taxonomyEntry(speciesId) == QDomElement() || zone(zoneId) == QDomElement())
		return;
	if (isEnabled) {
		QFile::remove(QString("%1/%2/%3/disabled").arg(zoneUrl()).arg(zoneId).arg(speciesId));
	}
	else {
		QFile file(QString("%1/%2/%3/disabled").arg(zoneUrl()).arg(zoneId).arg(speciesId));
		if (!file.open(QIODevice::WriteOnly))
			return;
		file.write("1");
		file.close();
	}
}

void GaiaCore::setSpeciesAreal(int speciesId, int zoneId, const QString &path) {
	if (taxonomyEntry(speciesId) == QDomElement() || zone(zoneId) == QDomElement())
		return;
	QDir dir(QString("%1/%2").arg(zoneUrl()).arg(zoneId));
	if (!dir.exists(QString::number(speciesId))) {
		if (!dir.mkdir(QString::number(speciesId)))
			return;
	}
	QString arealPath = QString("%1/%2/%3/areal.png").arg(zoneUrl()).arg(zoneId).arg(speciesId);
	if (QFile::exists(arealPath))
		QFile::remove(arealPath);
	QFile::copy(path, arealPath);
}

void GaiaCore::setSpeciesChapter(int speciesId, int zoneId, const QString &chapterName, const QString &chapterHtml) {
	QDir dir(QString("%1/%2").arg(zoneUrl()).arg(zoneId));
	if (!dir.exists(QString::number(speciesId)))
		if (!dir.mkdir(QString::number(speciesId)))
			return;
	QFile file(QString("%1/%2/%3/%4.html").arg(zoneUrl()).arg(zoneId).arg(speciesId).arg(chapterLayout(zoneId).value(chapterName)));
	if (!file.open(QIODevice::WriteOnly))
		return;
	file.write(chapterHtml.toUtf8());
	file.close();
}

void GaiaCore::setSpeciesStatus(int speciesId, int zoneId, const QList<int> &status) {
	QDir dir(QString("%1/%2").arg(zoneUrl()).arg(zoneId));
	if (!dir.exists(QString::number(speciesId)))
		if (!dir.mkdir(QString::number(speciesId)))
			return;
	QFile file(QString("%1/%2/%3/status.txt").arg(zoneUrl()).arg(zoneId).arg(speciesId));
	if (!file.open(QIODevice::WriteOnly))
		return;
	for (int i = 0; i < status.size() - 1; i++) {
		file.write(QString(QString::number(status.at(i)) + ",").toUtf8());
	}
	file.write(QString::number(status.last()).toUtf8());
	file.close();

}

#endif

QDomElement GaiaCore::elementById(const QDomElement &element, const QString &id) {
	QList<QDomElement> res = allElements(element);
	for (QList<QDomElement>::iterator i = res.begin(); i != res.end(); i++)
		if ((*i).attribute("id") == id)
			return *i;
	return QDomElement();
}

QList<QDomElement> GaiaCore::allElements(const QDomElement &element, const QString &tagName) {
	QList<QDomElement> res = QList<QDomElement>();
	if (tagName == "" || element.tagName() == tagName)
		res.append(element);
	for (int i = 0; i < element.childNodes().size(); i++) {
		res << allElements(element.childNodes().at(i).toElement(), tagName);
	}
	return res;
}

//void GaiaCore::printInfo(int speciesId, int zoneId) const {
	/*QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(tr("Print Document"));
	if (dialog->exec() != QDialog::Accepted)
		return;

	QPainter painter;
	int textOffset = 0;
	int bottomOffset = 60;

	painter.begin(&printer);
	printAux(painter, printer);*/

	/** Заголовок: русское и латинское название; статус; составители 
	QFont f = painter.font();
	QFont f2 = f;
	f2.setBold(true);
	f2.setPointSize(14);
	painter.setFont(f2);
	if (comment == "")
		painter.drawText(0, 80, rus);
	else
		painter.drawText(0, 100, QString("%1 (%2)").arg(rus).arg(comment));

	painter.drawText(0, 120, lat);
	painter.setFont(f);
	painter.drawText(0, 140, QString("%1 %2").arg(config->value(TEXT, "Cathegory").toString()).arg(status));
	painter.drawText(0, 155, compiler.trimmed());*/

	/** Портрет */
	//painter.drawPixmap(printer.pageRect().width() - 225, 80, 225, 150, *(detailPhoto->pixmap()));

	/** Федеральная карта */
	//painter.drawPixmap(0, 240, 225, 150, *(detailAreal->pixmap()));

	/** Карта региона */
	//painter.drawPixmap(printer.pageRect().width()/2, 225, 225, 150, *(localAreal->pixmap()));

	//textOffset = 400;

	/** Текст статьи или выбранных рубрик 
	QStringList doc = textView->toPlainText().split(". ");
	QString str = "";
	for (int i = 0; i < doc.size(); i++) {
		if (painter.boundingRect(0, 0, printer.pageRect().width(), printer.pageRect().height(), Qt::TextWordWrap, str + doc[i]).height() > printer.pageRect().height() - bottomOffset - textOffset) {
			painter.drawText(0, textOffset, printer.pageRect().width(), printer.pageRect().height() - bottomOffset, Qt::TextWordWrap, str + ". ");
			printer.newPage();
			printAux(painter, printer);
			textOffset = 80;
			str =  doc[i] + ". ";
		}
			str += doc[i] + ". ";
	}
	painter.drawText(0, textOffset, printer.pageRect().width(), printer.pageRect().height() - bottomOffset, Qt::TextWordWrap, str);
	painter.end();*/
//}

//void GaiaCore::printAux(QPainter &painter, QPrinter &printer) {
	/** Логотип */
	//painter.drawPixmap(0, 20, 30, 30, *(logo->pixmap()));
	/** Верхний колонтитул */
	//painter.drawText(QRect(40, 20, printer.pageRect().width() - 70, 30), Qt::TextWordWrap, config->value(TEXT, "TopBanner").toString());

	/** Нижний колонтитул */
	//painter.drawText(QRect(40, printer.pageRect().height() - 40, printer.pageRect().width() - 70, 30), Qt::TextWordWrap, config->value(TEXT, "BottomBanner").toString());
//}
