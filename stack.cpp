/***************************************************************************
 *   stack.cpp                                                             *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#include "stack.h"
#include <QtGui/QTextBrowser>
#include <QtDebug>
#include <QtGui/QListWidgetItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QApplication>
#include <QtGui/QRadioButton>
#include <QtCore/QStringList>
#include "config.h"
#include "core.h"
#define DEFAULT_ZONE 1

Stack::Stack(QWidget *parent) : QStackedWidget(parent) {
	currentDir = "";
	core = new GaiaCore();
	config = new QrbConfig(":/redbook.conf");
	latAlphas = new QList<QListWidgetItem*>();
	rusAlphas = new QList<QListWidgetItem*>();
	core->openTaxonomyFile(":/species.xml");
	chapterMap = new QMap<QString, QList<int> >();
	QList<int> indices;
	indices.clear();
	indices.append(1);
	chapterMap->insert("radioc0", QList<int>(indices));
	indices.clear();
	indices.append(84);
	chapterMap->insert("radioc1", QList<int>(indices));
	indices.clear();
	indices.append(225);
	indices.append(236);
	chapterMap->insert("radioc2", QList<int>(indices));
	indices.clear();
	indices.append(261);
	indices.append(266);
	chapterMap->insert("radioc3", QList<int>(indices));
	indices.clear();
	indices.append(322);
	chapterMap->insert("radioc4", QList<int>(indices));
	indices.clear();
	indices.append(430);
	chapterMap->insert("radioc5", QList<int>(indices));
	indices.clear();
	indices.append(502);
	indices.append(506);
	chapterMap->insert("radioc6", QList<int>(indices));
	indices.clear();
	indices.append(482);
	chapterMap->insert("radioc7", QList<int>(indices));
	core->openZoneFile(":/zones.xml");
	chapterLayout = new QMap<QString, QString>(core->chapterLayout(DEFAULT_ZONE));
}

Stack::~Stack() {
	delete core;
	delete config;
	qDeleteAll(*latAlphas);
	qDeleteAll(*rusAlphas);
	delete latAlphas;
	delete rusAlphas;
	delete chapterMap;
	delete chapterLayout;
}

void Stack::viewDocument(QListWidgetItem *item) {
	QString id = "", text = "";
	/* Index menu */
	if (item == 0) {
		text = qobject_cast<QAction*>(sender())->text();
		id = qobject_cast<QAction*>(sender())->data().toString();
	}
	/* Index list */
	else {
		text = item->text();
		id = item->data(Qt::UserRole).toString();
	}

	QTextBrowser *docViewer = findChild<QTextBrowser*>("docViewer");
	QLabel *docTitle = findChild<QLabel*>("docTitle");
	QComboBox *chapterCombo = findChild<QComboBox*>("chapterCombo");
	QRadioButton *radioc0 = findChild<QRadioButton*>("radioc0");

	/* Main screen */
	if (id == "p5") {
		currentDir = "";
		radioc0->setChecked(true);
		viewRusAlpha();
		this->setCurrentIndex(2);
	}
	/* Edit screen */
	else if (id == "p9") {
		currentDir = "";
		/* TODO */
	}
	/* Appendix */
	else if (id == "p7") {
		currentDir = "p7";
		docTitle->setText(text);
		chapterCombo->show();
		chapterCombo->clear();
		QList<QString> params = config->parameters("Appendix");
		for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
			QString chapter = config->value("Appendix", *param).toString();
			chapterCombo->addItem(chapter, *param);
		}
		this->setCurrentIndex(1);
	}
	/* Literature for animals */
	else if (id == "p6") {
		currentDir = "p6";
		docTitle->setText(text);
		chapterCombo->show();
		chapterCombo->clear();
		QList<QString> params = config->parameters("Chapters");
		for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
			QString chapter = config->value("Chapters", *param).toString();
			chapterCombo->addItem(chapter, *param);
		}
		this->setCurrentIndex(1);
	}
	else {
		currentDir = "";
		docViewer->setSource(qApp->applicationDirPath() + "/doc/" + id + ".html");
		docTitle->setText(text);
		chapterCombo->hide();
		this->setCurrentIndex(1);
	}
}


void Stack::viewChapter(const QString &chapter) {
	if (chapter == "")
		return;
	QTextBrowser *docViewer = findChild<QTextBrowser*>("docViewer");
	QComboBox *chapterCombo = qobject_cast<QComboBox*>(sender());
	QString file = chapterCombo->itemData(chapterCombo->currentIndex()).toString();
	docViewer->setSource(qApp->applicationDirPath() + "/doc/" + currentDir + "/" + file + ".html");
}

void Stack::viewLatAlpha() {
	QListWidget *alphaList = findChild<QListWidget*>("alphaList");
	alphaList->clear();

	for (int i = 0; i < chapterMap->value(chapterId).size(); i++) {
		QDomElement root = core->taxonomyEntry(chapterMap->value(chapterId)[i]);
		QList<QDomElement> speciesList = core->taxonomyElementsByTagName("species", root);
		for (QList<QDomElement>::iterator i = speciesList.begin(); i != speciesList.end(); i++) {
			QListWidgetItem *latItem = new QListWidgetItem();
			latItem->setText((*i).attribute("lat"));
			QString comment = (*i).attribute("comment");
			if (comment != QString())
				comment = " [" + comment + "]";
			latItem->setToolTip((*i).attribute("rus") + comment);
			latItem->setData(Qt::UserRole, (*i).attribute("id"));
			alphaList->addItem(latItem);
		}
	}

	alphaMode = "lat";
	alphaList->sortItems();
}

void Stack::viewRusAlpha() {
	QListWidget *alphaList = findChild<QListWidget*>("alphaList");
	alphaList->clear();

	for (int i = 0; i < chapterMap->value(chapterId).size(); i++) {
		QDomElement root = core->taxonomyEntry(chapterMap->value(chapterId)[i]);
		QList<QDomElement> speciesList = core->taxonomyElementsByTagName("species", root);
		for (QList<QDomElement>::iterator i = speciesList.begin(); i != speciesList.end(); i++) {
			QListWidgetItem *rusItem = new QListWidgetItem();
			QString comment = (*i).attribute("comment");
			if (comment != QString())
				comment = " [" + comment + "]";
			rusItem->setText((*i).attribute("rus") + comment);
			rusItem->setToolTip((*i).attribute("lat"));
			rusItem->setData(Qt::UserRole, (*i).attribute("id"));
			alphaList->addItem(rusItem);
		}
	}

	alphaMode = "rus";
	alphaList->sortItems();
}

void Stack::setTaxoChapter(bool isChecked) {
	if (!isChecked)
		return;
	chapterId = qobject_cast<QRadioButton*>(sender())->objectName();
	if (alphaMode == "rus")
		viewRusAlpha();
	else
		viewLatAlpha();
	updateTaxoTree();
}

void Stack::updateTaxoTree() {
	QTreeWidget *taxoTree = findChild<QTreeWidget*>("taxoTree");
	taxoTree->clear();
	for (int i = 0; i < chapterMap->value(chapterId).size(); i++) {
		QDomElement root = core->taxonomyEntry(chapterMap->value(chapterId)[i]);
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(root.attribute("rus") + " - " + root.attribute("lat")), 0);
		insertTaxoPart(item, root);
		item->setToolTip(0, root.attribute("rus") + " - " + root.attribute("lat"));
		item->setData(0, Qt::UserRole, root.attribute("id"));
		taxoTree->addTopLevelItem(item);
	}
	taxoTree->expandAll();
}

void Stack::insertTaxoPart(QTreeWidgetItem *parent, const QDomElement &root) {
	for (int i = 0; i < root.childNodes().size(); i++) {
		QDomElement element = root.childNodes().at(i).toElement();
		QString comment = element.attribute("comment");
		if (comment != QString())
			comment = " [" + comment + "]";
		QString appendum = "";
		if (element.tagName() == "species")
			appendum = " (" + element.attribute("author") + ", " + element.attribute("year") + ") (" + element.attribute("status") + ")";
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(element.attribute("rus") + comment + " - " + element.attribute("lat") + appendum), 0);
		insertTaxoPart(item, element);
		item->setToolTip(0, element.attribute("rus") + comment + " - " + element.attribute("lat") + appendum);
		item->setData(0, Qt::UserRole, element.attribute("id"));
		parent->addChild(item);
	}
}

void Stack::treeItemSelected(QTreeWidgetItem *item) {
	speciesId = item->data(0, Qt::UserRole).toInt();
	articleId = "a7";
	findChild<QLabel*>("photoLabel")->setPixmap(core->entryPicture(speciesId));
	findChild<QLabel*>("arealLabel")->setPixmap(core->speciesAreal(speciesId, DEFAULT_ZONE));
	findChild<QLabel*>("speciesLabel")->setText(core->speciesChapter(speciesId, DEFAULT_ZONE, "ax"));
	findChild<QLabel*>("commentLabel")->setText(core->entryAuthor(speciesId) + ", " + core->entryYear(speciesId));
	refreshArticle();
	QList<QString> parameters = config->parameters("ArticleType");
	for (QList<QString>::iterator i = parameters.begin(); i != parameters.end(); i++) {
		QListWidgetItem *item = new QListWidgetItem(config->value("ArticleType", *i).toString());
		item->setData(Qt::UserRole, *i);
		findChild<QListWidget*>("sectionList")->addItem(item);
	}
	this->setCurrentIndex(3);
}

void Stack::listItemSelected(QListWidgetItem *item) {
	speciesId = item->data(Qt::UserRole).toInt();
	articleId = "a7";
	findChild<QLabel*>("photoLabel")->setPixmap(core->entryPicture(speciesId));
	findChild<QLabel*>("arealLabel")->setPixmap(core->speciesAreal(speciesId, DEFAULT_ZONE));
	findChild<QLabel*>("speciesLabel")->setText(core->speciesChapter(speciesId, DEFAULT_ZONE, "ax"));
	findChild<QLabel*>("commentLabel")->setText(core->entryAuthor(speciesId) + ", " + core->entryYear(speciesId));
	refreshArticle();
	QList<QString> parameters = config->parameters("ArticleType");
	for (QList<QString>::iterator i = parameters.begin(); i != parameters.end(); i++) {
		QListWidgetItem *item = new QListWidgetItem(config->value("ArticleType", *i).toString());
		item->setData(Qt::UserRole, *i);
		findChild<QListWidget*>("sectionList")->addItem(item);
	}
	this->setCurrentIndex(3);
}

void Stack::refreshArticle() {
	QString all = "";
	if (articleId == "a7") {
		for (int i = 0; i < 7; i++)
			all += core->speciesChapter(speciesId, DEFAULT_ZONE, "a" + QString::number(i)) + "\n\n";
		findChild<QTextBrowser*>("articleBrowser")->setText("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"ru\" lang=\"ru\"><head><title></title><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" /></head><body align=\"justify\"><div style=\"white-space: pre-wrap\">" + all + "</div></body><html>");
	}
	else
		findChild<QTextBrowser*>("articleBrowser")->setText(core->speciesChapter(speciesId, DEFAULT_ZONE, articleId));
}

void Stack::setArticle(QListWidgetItem *item) {
	articleId = item->data(Qt::UserRole).toString();
	refreshArticle();
}
