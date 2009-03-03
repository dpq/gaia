/***************************************************************************
 *   logic.cpp                                                             *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008-2009 David Parunakian                                  *
 ***************************************************************************/
#include <QtCore/QFile>
#include <QtCore/QDate>
#include <QtGui/QPainter>
#include <QtGui/QApplication>

#include <QtGui/QMessageBox>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrinter>

#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QTreeWidget>
#include <QtGui/QListWidget>

#include <QtGui/QMenu>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>

#include <QtDebug>

#include "config.h"
#include "core.h"
#include "logic.h"

Logic::Logic(QrbConfig *config, QWidget *parent) : QObject(parent) {
	core = new GaiaCore();
	this->config = config;
	core->openTaxonomyFile(":/species.xml");
	core->openZoneFile(":/zones.xml");
	
	initChapterRoots();
	chapterArticles = new QMap<QString, QString>(core->chapterLayout(zoneId));
	
	pageColor = new QMap<int, QString>();
	labelColor = new QMap<int, QString>();
	commentColor = new QMap<int, QString>();

	zoneMapping = new QMap<QAction*, int>();
	articleId = "";
	chapterId = "radioc0";
	lang = "rus";
	editMode = false;
	zoneId = 1;
	taxoSpecies = new QList<QTreeWidgetItem*>();
	currentCathegory =  "";

	this->parent = parent;
	stack = parent->findChild<QStackedWidget*>("stackedWidget");
	indexList = parent->findChild<QListWidget*>("indexList");
	indexLabel = parent->findChild<QLabel*>("indexLabel");
	docViewer = parent->findChild<QTextBrowser*>("docViewer");
	articleBrowser = parent->findChild<QTextBrowser*>("articleBrowser");
	docTitleLabel = parent->findChild<QLabel*>("docTitleLabel");
	chapterCombo = parent->findChild<QComboBox*>("chapterCombo");
	editAction = parent->findChild<QAction*>("editAction");
	saveAction = parent->findChild<QAction*>("saveAction");
	cancelAction = parent->findChild<QAction*>("cancelAction");
	specMenu = parent->findChild<QMenu*>("specMenu")->menuAction();
	fontMenu = parent->findChild<QMenu*>("fontMenu")->menuAction();
	alphaList = parent->findChild<QListWidget*>("alphaList");
	taxoTree = parent->findChild<QTreeWidget*>("taxoTree");
	photoLabel = parent->findChild<QLabel*>("photoLabel");
	arealLabel = parent->findChild<QLabel*>("arealLabel");
	logoLabel = parent->findChild<QLabel*>("logoLabel");
	speciesLabel = parent->findChild<QLabel*>("speciesLabel");
	commentLabel = parent->findChild<QLabel*>("commentLabel");
	sectionList = parent->findChild<QListWidget*>("sectionList");
	colorPage = parent->findChild<QWidget*>("colorPage");
	printButton = parent->findChild<QPushButton*>("printButton");
	backButton = parent->findChild<QPushButton*>("backToListsButton");
	
	indexSections[""] = "Index";
	indexSections["p6"] = "Chapters";
	indexSections["p7"] = "Appendix";

	oppositeLang["rus"] = "lat";
	oppositeLang["lat"] = "rus";

	labelColor->insert(0, "#ffffff");
	labelColor->insert(1, "#ffff00");
	labelColor->insert(2, "#008000");
	labelColor->insert(3, "#008000");
	labelColor->insert(4, "#008000");
	labelColor->insert(5, "#00ff00");

	commentColor->insert(0, "#ffffff");
	commentColor->insert(1, "#ffffff");
	commentColor->insert(2, "#000000");
	commentColor->insert(3, "#000000");
	commentColor->insert(4, "#000000");
	commentColor->insert(5, "#ffffff");
	
	pageColor->insert(0, "#000000");
	pageColor->insert(1, "#ff0000");
	pageColor->insert(2, "#ffffa0");
	pageColor->insert(3, "#ffffff");
	pageColor->insert(4, "#cccccc");
	pageColor->insert(5, "#008000");

	initIndex();
}

Logic::~Logic() {
	delete core;
	delete chapterRoots;
	delete chapterArticles;
	delete pageColor;
	delete labelColor;
	delete commentColor;

	delete taxoSpecies;
	delete zoneMapping;
}

void Logic::initChapterRoots() {
	chapterRoots = new QMap<QString, QList<int> >();
	QList<int> idList;
	foreach (const QString &chapter, config->parameters("ChapterRoots")) {
		idList.clear();
		foreach (const QString &id, config->value("ChapterRoots", chapter).toString().split(" ")) {
			idList.append(id.toInt());
		}
		chapterRoots->insert(chapter, idList);
	}
}

void Logic::initIndex(const QString &section) {
	indexList->clear();
	if (section == "") {
		indexLabel->setText(config->value("Labels", "index").toString());
	}
	else if (section == "p6" || section == "p7") {
		indexLabel->setText(config->value("Index", section).toString());
		indexList->addItem(config->value("Labels", "back").toString());
	}
	foreach (const QString &param, config->parameters(indexSections[section])) {
		QListWidgetItem *item = new QListWidgetItem(config->value(indexSections[section], param).toString());
		item->setData(Qt::UserRole, param);
		indexList->addItem(item);
	}
}

void Logic::viewSingleDoc(const QString &docId, const QString &docName) {
	QFile file(qApp->applicationDirPath() + "/doc/" + docId + ".html");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	docViewer->setHtml(QString::fromUtf8(file.readAll()));
	file.close();
	docTitleLabel->setText(docName);
	editAction->setVisible(false);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	specMenu->setVisible(false);
	fontMenu->setVisible(true);
	chapterCombo->clear();
	chapterCombo->hide();
	stack->setCurrentIndex(1);
}

/* When invoked from the index widget, ID is of the paragraph to load. When invoked from the index menu, ID is of the general section */
void Logic::viewMultiDoc(const QString &id, const QString &item) {
	if (item == "") {
		multiDocDir = id;
	}
	docTitleLabel->setText(config->value("Index", id).toString());
	chapterCombo->clear();
	chapterCombo->show();
	foreach (const QString &param, config->parameters(indexSections[id])) {
		chapterCombo->addItem(config->value(indexSections[id], param).toString(), param);
	}
	if (item == "") {
		chapterCombo->setCurrentIndex(0);
	}
	else {
		chapterCombo->setCurrentIndex(chapterCombo->findText(item));
	}
	editAction->setVisible(false);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	specMenu->setVisible(false);
	fontMenu->setVisible(true);
	stack->setCurrentIndex(1);
}

void Logic::viewMultiDocChapter(const QString &chapter) {
	if (chapter == "")
		return;
	QFile file(qApp->applicationDirPath() + "/doc/" + multiDocDir + "/" + chapterCombo->itemData(chapterCombo->currentIndex()).toString() + ".html");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	docViewer->setHtml(QString::fromUtf8(file.readAll()));
	file.close();
}

void Logic::indexWidgetClicked(QListWidgetItem *item) {
	QString text = item->text();
	QString id = item->data(Qt::UserRole).toString();
	if (id == "p6" || id == "p7") {
		multiDocDir = id;
		initIndex(id);
	}
	else if (id == "") {
		initIndex();
	}
	else if (id == "p5") {
		viewSpeciesLists();
	}
	else if (id[0] != QChar('p')) {
		viewMultiDoc(multiDocDir, text);
	}
	else {
		initIndex();
		viewSingleDoc(id, text);
	}
}

void Logic::indexMenuClicked() {
	QString text = qobject_cast<QAction*>(sender())->text();
	QString id = qobject_cast<QAction*>(sender())->data().toString();
	if (id == "p6" || id == "p7") {
		initIndex(id);
		viewMultiDoc(id);
	}
	else if (id == "p5") {
		initIndex();
		viewSpeciesLists();
	}
	else {
		initIndex();
		viewSingleDoc(id, text);
	}
}

void Logic::viewSpeciesLists() {
	parent->findChild<QRadioButton*>(chapterId)->setChecked(true);
	populateAlphaList();
	editAction->setVisible(false);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	specMenu->setVisible(false);
	fontMenu->setVisible(false);
	taxoTree->clearSelection();
	alphaList->clearSelection();
	stack->setCurrentIndex(2);
}

void Logic::showIndex() {
	initIndex();
	editAction->setVisible(false);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	specMenu->setVisible(false);
	fontMenu->setVisible(false);
	stack->setCurrentIndex(0);
}

void Logic::chapterSelected(bool isChecked) {
	if (!isChecked)
		return;
	chapterId = qobject_cast<QRadioButton*>(sender())->objectName();
	populateAlphaList();
	populateSystematics();
}

void Logic::populateSystematics() {
	taxoTree->clear();
	taxoSpecies->clear();
	foreach (int id, chapterRoots->value(chapterId)) {
		QDomElement root = core->taxonomyEntry(id);
		QString taxoLevel = config->value("Taxo", root.tagName()).toString();
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(taxoLevel + " " + root.attribute("rus").toUpper() + " - " + root.attribute("lat")), 0);
		populateSystematicsBranch(item, root);
		item->setToolTip(0, "<font color=\"red\"><pre>" +  root.attribute("rus") + " - " + root.attribute("lat") + "</pre></font>");
		item->setData(0, Qt::UserRole, root.attribute("id"));
		taxoTree->addTopLevelItem(item);
	}
	taxoTree->expandAll();
}

void Logic::populateSystematicsBranch(QTreeWidgetItem *parent, const QDomElement &root) {
	for (int i = 0; i < root.childNodes().size(); i++) {
		QDomElement element = root.childNodes().at(i).toElement();
		QString comment = (element.attribute("comment") == "" ? "" : " [" + element.attribute("comment") + "]");
		QString oldid = (element.attribute("oldid") == "" ? "" : element.attribute("oldid") + ". ");
		QTreeWidgetItem *item;
		if (element.tagName() == "species") {
			QString reference = " (" + element.attribute("author") + ", " + element.attribute("year") + ") (" + element.attribute("status") + ")";
			item = new QTreeWidgetItem(QStringList(oldid + element.attribute("rus") + comment + " - " + element.attribute("lat") + reference), 0);
			item->setToolTip(0, "<font color=\"red\"><pre>" + element.attribute("rus") + comment + " - " + element.attribute("lat") + reference + "</pre></font>");
			taxoSpecies->append(item);
		}
		else {
			QString taxonomyLevel = config->value("Taxo", element.tagName()).toString() + " ";
			item = new QTreeWidgetItem(QStringList(taxonomyLevel + element.attribute("rus").toUpper() + " - " + element.attribute("lat")), 0);
		}
		populateSystematicsBranch(item, element);
		item->setData(0, Qt::UserRole, element.attribute("id"));
		parent->addChild(item);
	}
}

void Logic::populateAlphaList() {
	alphaList->clear();
	QMap<QString, QString> text;
	foreach (int taxonomyId, chapterRoots->value(chapterId)) {
		QList<QDomElement> speciesList = core->taxonomyElementsByTagName("species", core->taxonomyEntry(taxonomyId));
		for (QList<QDomElement>::iterator i = speciesList.begin(); i != speciesList.end(); i++) {
			QListWidgetItem *item = new QListWidgetItem();
			text["lat"] = (*i).attribute("lat");
			text["rus"] = (*i).attribute("rus") + ((*i).attribute("comment") == QString() ? "" : " [" + (*i).attribute("comment") + "]");
			item->setText(text[lang]);
			item->setToolTip("<font color=\"red\"><pre>" + text[oppositeLang[lang]] + "</pre></font>");
			item->setData(Qt::UserRole, (*i).attribute("id"));
			alphaList->addItem(item);
		}
	}
	this->lang = lang;
	alphaList->sortItems();
}

void Logic::latAlpha() {
	lang = "lat";
	populateAlphaList();
}

void Logic::rusAlpha() {
	lang = "rus";
	populateAlphaList();
}

void Logic::treeItemHighlighted(QTreeWidgetItem *item) {
	if (taxoSpecies->indexOf(item) == -1)
		return;
	speciesId = item->data(0, Qt::UserRole).toInt();
	foreach (QListWidgetItem *listItem, alphaList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard)) {
		if (listItem->data(Qt::UserRole).toInt() == speciesId) {
			alphaList->setCurrentItem(listItem);
			break;
		}
	}
}

void Logic::listItemHighlighted(QListWidgetItem *item) {
	speciesId = item->data(Qt::UserRole).toInt();
	foreach(QTreeWidgetItem *treeItem, *taxoSpecies) {
		if (treeItem->data(0, Qt::UserRole).toInt() == speciesId) {
			taxoTree->setCurrentItem(treeItem);
			break;
		}
	}
}

void Logic::treeItemSelected(QTreeWidgetItem *item) {
	if (taxoSpecies->indexOf(item) == -1)
		return;
	speciesId = item->data(0, Qt::UserRole).toInt();
	foreach (QListWidgetItem *listItem, alphaList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard)) {
		if (listItem->data(Qt::UserRole).toInt() == speciesId) {
			alphaList->setCurrentItem(listItem);
			break;
		}
	}
	viewSpeciesArticle();
}

void Logic::listItemSelected(QListWidgetItem *item) {
	speciesId = item->data(Qt::UserRole).toInt();
	foreach(QTreeWidgetItem *treeItem, *taxoSpecies) {
		if (treeItem->data(0, Qt::UserRole).toInt() == speciesId) {
			taxoTree->setCurrentItem(treeItem);
			break;
		}
	}
	viewSpeciesArticle();
}

void Logic::viewSpeciesArticle() {
	photoLabel->setPixmap(core->entryPicture(speciesId));
	arealLabel->setPixmap(core->speciesAreal(speciesId, zoneId));
	
	QString speciesText = core->speciesChapter(speciesId, zoneId, config->value("Labels", "name").toString());
	QString line1 = speciesText.split("\n")[0].toUpper();
	QString line2 = speciesText.split("\n")[1];
	speciesText = speciesText.split("\n")[2] + "\n" + speciesText.split("\n")[3] + "\n";
	
	QStringList litText = core->speciesChapter(speciesId, zoneId, config->value("Labels", "lit").toString()).split("\n", QString::SkipEmptyParts);
	
	QString compilers = "";
	for (int i = litText.size() - 1; i >= 0; i--) {
		if (litText[i].trimmed().length() > 0) {
			compilers = litText[i].trimmed();
			break;
		}
	}
	
	QList<int> cat = core->speciesStatus(speciesId, zoneId);
	QString cathegory = "";
	for (QList<int>::const_iterator i = cat.begin(); i != cat.end(); i++) {
		cathegory += QString::number(*i) + ", ";
	}
	
	cathegory = cathegory.left(cathegory.length() - 2);
	cathegory += " " + config->value("Labels", "cathegory").toString().toLower();
	speciesLabel->setWordWrap(true);
	speciesLabel->setText("<div style=\"whitespace:pre-wrap\">" + line1 + "<br />" + line2 + "</div>");
	commentLabel->setText(speciesText.trimmed() + "\n" + cathegory + "\n" + compilers);
	bool selectionFound = false;
	sectionList->clear();
	sectionList->addItem(config->value("Labels", "full").toString());
	overviewItem = sectionList->item(0);
	if (editMode) {
		overviewItem->setHidden(true);
	}
	QMap<QString, QString> parameters = core->chapterLayout(zoneId, true);
	QList<QString> keys = parameters.values();
	qSort(keys);
		
	for (QList<QString>::iterator i = keys.begin(); i != keys.end(); i++) {
		if (!QFile().exists(core->zoneUrl() + "/" + QString::number(zoneId)  + "/" + QString::number(speciesId)  + "/" + *i))
			continue;
		QListWidgetItem *item = new QListWidgetItem(parameters.key(*i));
		item->setData(Qt::UserRole, *i);
		sectionList->addItem(item);
		if (parameters.key(*i) == articleId) {
			sectionList->setCurrentItem(item);
			selectionFound = true;
		}
	}
	if (! selectionFound) {
		sectionList->setCurrentRow(0);
		articleId = "";
	}
	
	refreshArticle();
	
	/* Colorizing */
	if (currentCathegory != cathegory) {
		if (cat.size() > 1) {
			QString stylesheet = "#colorPage { background: qconicalgradient(cx:0, cy:0,";
			QString stops = "";
			float x = 1.0;
			for (QList<int>::const_iterator i = cat.begin(); i != cat.end(); i++) {
				stops += " stop: " + QString::number(x, 'f', 3) + " " + pageColor->value(*i) + ",";
				x -= (0.25/(cat.size()));
				if (i + 1 != cat.end())
					stops += " stop: " + QString::number(x + 0.005, 'f', 3) + " " + pageColor->value(*i) + ",";
			stops = stops.left(stops.length() - 1);
			colorPage->setStyleSheet(stylesheet + stops + ") }");
			}
		}
		else {
			colorPage->setStyleSheet("#colorPage { background-color:" + pageColor->value(cat[0]) + "}");
		}
		QString specialBackground = "", lc = labelColor->value(cat[0]), cc = commentColor->value(cat[0]), bc = pageColor->value(cat[0]);
		if (cat.size() > 1) {
			specialBackground = "background-color: #ffffff; border: 1px solid black;";
			cc = "#000000";
			lc = "#000000";
		}
		speciesLabel->setStyleSheet("#speciesLabel {" + specialBackground + "font: 75 16pt \"Sans Serif\"; color: " + lc + "}");
		commentLabel->setStyleSheet("#commentLabel {" + specialBackground + "font: 10pt \"Sans Serif\"; color: " + cc + "}");
		if (specialBackground != "") {
			sectionList->setStyleSheet("#sectionList { background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); selection-background-color:#ffffff; selection-color:" + lc + "} #sectionList::item::selected { border: 1px solid black }");
		}
		else {
			sectionList->setStyleSheet("#sectionList { background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); selection-background-color:" + bc + "; selection-color:" + lc + "}");
		}
		currentCathegory = cathegory;
	}
	
	if (editMode) {
		editAction->setVisible(false);
		saveAction->setVisible(true);
		cancelAction->setVisible(true);
	}
	else {
		editAction->setVisible(true);
		saveAction->setVisible(false);
		cancelAction->setVisible(false);
	}
	/* Check the Core to see if this species exists in other zones as well */
	QList<int> speciesZones = core->speciesZones(speciesId);
	specMenu->menu()->clear();
	zoneMapping->clear();
	if (speciesZones.size() > 1) {
		foreach (int zoneId, speciesZones) {
			QAction *zoneAction = specMenu->menu()->addAction(core->zoneName(zoneId));
			zoneMapping->insert(zoneAction, zoneId);
			QObject::connect(specMenu->menu(), SIGNAL(triggered(QAction*)), this, SLOT(setZone(QAction*)));
		}
		specMenu->setVisible(true);
	}
	else {
		specMenu->setVisible(false);
	}
	fontMenu->setVisible(true);
	
	stack->setCurrentIndex(3);
}

void Logic::setZone(QAction *action) {
	bool isHidden = overviewItem->isHidden();
	checkModification();
	zoneId = zoneMapping->value(action);
	delete chapterArticles;
	chapterArticles = new QMap<QString, QString>(core->chapterLayout(zoneId));
	refreshSectionList();
	overviewItem = sectionList->item(0);
	if (isHidden) {
		overviewItem->setHidden(true);
		sectionList->setCurrentItem(sectionList->item(1));
		this->setArticle(sectionList->currentItem());
	}
	refreshArticle();
}

void Logic::refreshSectionList() {
	bool selectionFound = false;
	sectionList->clear();
	sectionList->addItem(config->value("Labels", "full").toString());
	QMap<QString, QString> parameters = core->chapterLayout(zoneId, true);
	QList<QString> keys = parameters.values();
	qSort(keys);
	
	for (QList<QString>::iterator i = keys.begin(); i != keys.end(); i++) {
		if (!QFile().exists(core->zoneUrl() + "/" + QString::number(zoneId)  + "/" + QString::number(speciesId)  + "/" + *i))
			continue;
		QListWidgetItem *item = new QListWidgetItem(parameters.key(*i));
		item->setData(Qt::UserRole, *i);
		sectionList->addItem(item);
		if (parameters.key(*i) == articleId) {
			sectionList->setCurrentItem(item);
			selectionFound = true;
		}
	}

	if (! selectionFound) {
		sectionList->setCurrentRow(0);
		articleId = "";
	}
}

void Logic::refreshArticle() {
	QString all = "";
	if (articleId == "") {
		QMap<QString, QString> parameters = core->chapterLayout(zoneId, true);
		QList<QString> keys = parameters.values();
		qSort(keys);
		for (QList<QString>::iterator i = keys.begin(); i != keys.end(); i++) {
			if (!QFile().exists(core->zoneUrl() + "/" + QString::number(zoneId)  + "/" + QString::number(speciesId)  + "/" + *i))
				continue;
			all += "\t" + parameters.key(*i) + "\n";
			all += core->speciesChapter(speciesId, zoneId, parameters.key(*i)) + "\n\n";
		}
		articleBrowser->setText("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"ru\" lang=\"ru\"><head><title></title><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" /></head><body align=\"justify\" style=\"margin: 15px\"><div style=\"white-space: pre-wrap\">" + all + "</div></body><html>");
	}
	else
		articleBrowser->setText(core->speciesChapter(speciesId, zoneId, articleId));
	if (editMode) {
		original = articleBrowser->toPlainText();
		prevArtRow = sectionList->currentRow();
	}
}

bool Logic::checkModification() {
	if (editMode && original != articleBrowser->toPlainText()) {
		QMessageBox msgBox;
		msgBox.setText(config->value("Labels", "DocModified").toString());
		msgBox.setInformativeText(config->value("Labels", "DocSave").toString());
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Cancel) {
			sectionList->setCurrentRow(prevArtRow);
			return false;
		}
		if (ret == QMessageBox::Save) {
			core->setSpeciesChapter(speciesId, zoneId, articleId, articleBrowser->toPlainText());
		}
		else if (ret == QMessageBox::Discard) {
			articleBrowser->setText(original);
		}
	}
	return true;
}

void Logic::setArticle(QListWidgetItem *item) {
	articleId = (item->data(Qt::UserRole).toString() == "" ? "" : item->text());
	refreshArticle();
}

void Logic::nextSpecies() {
	if (!checkModification())
		return;
	if (zoneId != 1) {
		zoneId = 1;
		delete chapterArticles;
		chapterArticles = new QMap<QString, QString>(core->chapterLayout(zoneId));
		refreshSectionList();
	}
	if (taxoSpecies->indexOf(taxoTree->currentItem()) < taxoSpecies->count() - 1) {
		taxoTree->setCurrentItem(taxoSpecies->at(taxoSpecies->indexOf(taxoTree->currentItem()) + 1));
	}
	else {
		taxoTree->setCurrentItem(taxoSpecies->first());
	}
	treeItemSelected(taxoTree->currentItem());
	if (editMode)
		original = articleBrowser->toPlainText();
}

void Logic::prevSpecies() {
	if (!checkModification())
		return;
	if (zoneId != 1) {
		zoneId = 1;
		delete chapterArticles;
		chapterArticles = new QMap<QString, QString>(core->chapterLayout(zoneId));
		refreshSectionList();
	}
	if (taxoSpecies->indexOf(taxoTree->currentItem()) > 0) {
		taxoTree->setCurrentItem(taxoSpecies->at(taxoSpecies->indexOf(taxoTree->currentItem()) - 1));
	}
	else {
		taxoTree->setCurrentItem(taxoSpecies->last());
	}
	treeItemSelected(taxoTree->currentItem());
	if (editMode)
		original = articleBrowser->toPlainText();
}

void Logic::changeFocus(QWidget *old, QWidget *now) {
	if (old == taxoTree && now == alphaList) {
		taxoTree->clearSelection();
	}
	else if(old == alphaList && now == taxoTree) {
		alphaList->clearSelection();
	}
}

void Logic::up() {
	taxoTree->clearSelection();
	alphaList->clearSelection();
	stack->setCurrentIndex(2);
}

void Logic::saveEdit() {
	core->setSpeciesChapter(speciesId, zoneId, articleId, articleBrowser->toPlainText());
	articleBrowser->setReadOnly(true);
	editMode = false;
	editAction->setVisible(true);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	printButton->setEnabled(true);
	backButton->setEnabled(true);
	overviewItem->setHidden(false);
}

void Logic::cancelEdit() {
	articleBrowser->setText(original);
	articleBrowser->setReadOnly(true);
	editMode = false;
	editAction->setVisible(true);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	printButton->setEnabled(true);
	backButton->setEnabled(true);
	overviewItem->setHidden(false);
}

void Logic::edit() {
	original = articleBrowser->toPlainText();
	articleBrowser->setReadOnly(false);
	editMode = true;
	editAction->setVisible(false);
	saveAction->setVisible(true);
	cancelAction->setVisible(true);
	printButton->setEnabled(false);
	backButton->setEnabled(false);
	overviewItem->setHidden(true);
	if (articleId == "") {
		sectionList->setCurrentItem(sectionList->item(1));
		if (checkModification())
			this->setArticle(sectionList->currentItem());
	}
	prevArtRow = sectionList->currentRow();
	articleBrowser->setFocus();
}

void Logic::showHelp() {
	QDialog *helpDialog = new QDialog(qApp->activeWindow());

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	QPushButton *closeButton = new QPushButton(config->value("Labels", "close").toString());
	buttonLayout->addStretch();
	buttonLayout->addWidget(closeButton);

	QVBoxLayout *helpLayout = new QVBoxLayout(helpDialog);
	QTextBrowser *helpBrowser = new QTextBrowser();
	helpLayout->addWidget(helpBrowser);
	helpLayout->addLayout(buttonLayout);

	QFile file(qApp->applicationDirPath() + "/doc/help/h" + QString::number(stack->currentIndex()) + ".html");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	helpBrowser->setHtml(QString::fromUtf8(file.readAll()));
	file.close();
	
	helpDialog->resize(400, 300);
	connect(closeButton, SIGNAL(clicked()), helpDialog, SLOT(accept()));
	helpDialog->exec();
	delete helpDialog;
}

void Logic::printDocument() {
	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, 0);
	dialog->setWindowTitle(tr("Print Document"));
	if (dialog->exec() != QDialog::Accepted)
		return;
	docViewer->print(&printer);
}

void Logic::printSpecies() {
	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, 0);
	dialog->setWindowTitle(tr("Print Document"));
	if (dialog->exec() != QDialog::Accepted)
		return;
	QPainter painter;
	int textOffset = 0;
	int bottomOffset = 60;

	painter.begin(&printer);
	printAux(painter, printer);

	/* Russian and Latin names; status; compilers */
	QFont f = painter.font();
	QFont f2 = f;
	f2.setBold(true);
	f2.setPointSize(14);
	painter.setFont(f2);
	painter.drawText(QRect(0, 80, printer.pageRect().width(), 50), Qt::TextWordWrap, speciesLabel->text().replace("<br />","\n").remove("<div style=\"whitespace:pre-wrap\">").remove("</div>"));

	painter.drawText(0, 145, core->entryLatName(speciesId));
	painter.setFont(f);
	painter.drawText(QRect(0, 155, printer.pageRect().width(), 75), Qt::TextWordWrap, commentLabel->text());

	/* Species picture */
	painter.drawPixmap(printer.pageRect().width() - 265, 225, 225, 150, *(arealLabel->pixmap()));

	/* Species range */
	painter.drawPixmap(40, 225, 225, 150, *(photoLabel->pixmap()));

	textOffset = 400;

	/* Text of the selected articles */
	QStringList doc = articleBrowser->toPlainText().split(". ");
	QString str = "";
	for (int i = 0; i < doc.size(); i++) {
		if (painter.boundingRect(0, 0, printer.pageRect().width(), printer.pageRect().height(), Qt::TextWordWrap, str + doc[i]).height() > printer.pageRect().height() - bottomOffset - textOffset) {
			painter.drawText(0, textOffset, printer.pageRect().width(), printer.pageRect().height() - bottomOffset, Qt::TextWordWrap, str);
			printer.newPage();
			printAux(painter, printer);
			textOffset = 80;
			str =  doc[i] + ". ";
		}
			str += doc[i] + ". ";
	}
	painter.drawText(0, textOffset, printer.pageRect().width(), printer.pageRect().height() - bottomOffset, Qt::TextWordWrap, str);
	painter.end();
}

void Logic::printAux(QPainter &painter, QPrinter &printer) {
	/* Logo */
	painter.drawPixmap(0, 20, 30, 30, *(logoLabel->pixmap()));

	/* Top banner */
	painter.drawText(QRect(40, 20, printer.pageRect().width() - 70, 30), Qt::TextWordWrap, config->value("Labels", "TopBanner").toString());

	/* Bottom banner */
	painter.drawText(QRect(40, printer.pageRect().height() - 40, printer.pageRect().width() - 70, 30), Qt::TextWordWrap, config->value("Labels", "BottomBanner").toString());
	painter.drawText(QRect(printer.pageRect().width() - 70, printer.pageRect().height() - 40, printer.pageRect().width(), 30), Qt::TextWordWrap, QDate::currentDate().toString("yyyy-MM-dd"));
}

void Logic::largerFont() {
	docViewer->setStyleSheet("font: 16pt \"Sans Serif\"");
	articleBrowser->setStyleSheet("font: 16pt \"Sans Serif\"");
}

void Logic::smallerFont() {
	docViewer->setStyleSheet("font: 10pt \"Sans Serif\"");
	articleBrowser->setStyleSheet("font: 10pt \"Sans Serif\"");
}
