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

// TODO Rus - Lat modified lists

Logic::Logic(QrbConfig *config, QWidget *parent) : QObject(parent) {
	core = new GaiaCore();
	this->config = config;
	core->openTaxonomyFile(":/species.xml");
	core->openZoneFile(":/zones.xml");
	
	initChapterMap();

	zoneMapping = new QMap<QAction*, int>();
	chapterLayout = new QMap<QString, QString>(core->chapterLayout(zoneId));

	articleId = "";
	chapterId = "radioc0";
	editMode = false;
	zoneId = 1;
	taxoSpecies = new QList<QTreeWidgetItem*>();
	currentCathegory =  "";

	this->parent = parent;
	stack = parent->findChild<QStackedWidget*>("stackedWidget");
	indexLabel = parent->findChild<QLabel*>("indexLabel");
	docViewer = parent->findChild<QTextBrowser*>("docViewer");
	articleBrowser = parent->findChild<QTextBrowser*>("articleBrowser");
	docTitle = parent->findChild<QLabel*>("docTitle");
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
	
	sectionMapping[""] = "Index";
	sectionMapping["p6"] = "Chapters";
	sectionMapping["p7"] = "Appendix";

	oppositeLang["rus"] = "lat";
	oppositeLang["lat"] = "rus";
}

Logic::~Logic() {
	delete core;
	delete config;
	delete chapterMap;
	delete chapterLayout;
	delete taxoSpecies;
	delete zoneMapping;
}

void Logic::initChapterMap() {
	chapterMap = new QMap<QString, QList<int> >();
	QList<int> idList;
	foreach (const QString &chapter, config->parameters("ChapterMapping")) {
		idList.clear();
		foreach (const QString &id, config->value("ChapterMapping", chapter).toString().split(" ")) {
			idList.append(id.toInt());
		}
		chapterMap->insert(chapter, idList);
	}
}

void Logic::initIndex(const QString &mode) {
	QListWidget *indexList = parent->findChild<QListWidget*>("indexList");
	indexList->clear();
	if (mode == "") {
		indexLabel->setText(config->value("Labels", "index").toString());
	}
	else if (mode == "p6") {
		indexLabel->setText(config->value("Index", "p6").toString());
		indexList->addItem(config->value("Labels", "back").toString());
	}
	else if (mode == "p7") {
		indexLabel->setText(config->value("Index", "p7").toString());
		indexList->addItem(config->value("Labels", "back").toString());
	}
	foreach (const QString &param, config->parameters(sectionMapping[mode])) {
		QListWidgetItem *item = new QListWidgetItem(config->value(sectionMapping[mode], param).toString());
		item->setData(Qt::UserRole, param);
		indexList->addItem(item);
	}
}

void Logic::viewSingleDoc(const QString &docId, const QString &docName) {
	QFile file(qApp->applicationDirPath() + "/doc/" + docId + ".html");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	docViewer->setHtml(QString::fromUtf8(file.readAll()));
	file.close();
	docTitle->setText(docName);
	editAction->setVisible(false);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	specMenu->setVisible(false);
	fontMenu->setVisible(true);
	chapterCombo->clear();
	chapterCombo->hide();
	stack->setCurrentIndex(1);
}

void Logic::viewMultiDoc(const QString &id, const QString &item) {
	//QString section;
	if (item == "") {
		multiDocDir = id;
	}
	//section = id;
	docTitle->setText(config->value("Index", id).toString());
	//}
	//else {
	//	section = type;
	//	multiDocDir = type;
	//	docTitle->setText(config->value("Index", type).toString());
	//}
	chapterCombo->clear();
	chapterCombo->show();
	foreach (const QString &param, config->parameters(sectionMapping[id])) {
		chapterCombo->addItem(config->value(sectionMapping[id], param).toString(), param);
	}
	if (item != "") {
		chapterCombo->setCurrentIndex(chapterCombo->findText(item));
	}
	else {
		chapterCombo->setCurrentIndex(0);
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

void Logic::viewSpeciesLists() {
	parent->findChild<QRadioButton*>(chapterId)->setChecked(true);
	rusAlpha();
	editAction->setVisible(false);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	specMenu->setVisible(false);
	fontMenu->setVisible(false);
	taxoTree->clearSelection();
	alphaList->clearSelection();
	stack->setCurrentIndex(2);
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
		return;
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

void Logic::showIndex() {
	indexMode = "";
	initIndex();
	editAction->setVisible(false);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	specMenu->setVisible(false);
	fontMenu->setVisible(false);
	chapterCombo->hide();
	stack->setCurrentIndex(0);
}

void Logic::latAlpha() {
	setAlphaListLang("lat");
}

void Logic::rusAlpha() {
	setAlphaListLang("rus");
}

void Logic::setAlphaListLang(const QString &lang) {
	alphaList->clear();
	QMap<QString, QString> text;
	/* Load into the list all species in the taxonomy entities corresponding to the selected chapter */
	foreach (int taxonomyId, chapterMap->value(chapterId)) {
		QDomElement root = core->taxonomyEntry(taxonomyId);
		QList<QDomElement> speciesList = core->taxonomyElementsByTagName("species", root);
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
	alphaMode = lang;
	alphaList->sortItems();
}

void Logic::setTaxoChapter(bool isChecked) {
	if (!isChecked)
		return;
	chapterId = qobject_cast<QRadioButton*>(sender())->objectName();
	if (alphaMode == "rus")
		rusAlpha();
	else
		latAlpha();
	updateTaxonomy();
}

void Logic::updateTaxonomy() {
	taxoTree->clear();
	taxoSpecies->clear();
	foreach (int id, chapterMap->value(chapterId)) {
		QDomElement root = core->taxonomyEntry(id);
		QString taxoLevel = config->value("Taxo", root.tagName()).toString();
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(taxoLevel + " " + root.attribute("rus").toUpper() + " - " + root.attribute("lat")), 0);
		insertTaxoPart(item, root);
		item->setToolTip(0, "<font color=\"red\"><pre>" +  root.attribute("rus") + " - " + root.attribute("lat") + "</pre></font>");
		item->setData(0, Qt::UserRole, root.attribute("id"));
		taxoTree->addTopLevelItem(item);
	}
	taxoTree->expandAll();
}

void Logic::insertTaxoPart(QTreeWidgetItem *parent, const QDomElement &root) {
	for (int i = 0; i < root.childNodes().size(); i++) {
		QDomElement element = root.childNodes().at(i).toElement();
		QString comment = element.attribute("comment");
		if (comment != QString())
			comment = " [" + comment + "]";
		QString oldid = element.attribute("oldid");
		if (oldid != QString())
			oldid += ". ";
		QTreeWidgetItem *item;
		QString appendum = "";
		if (element.tagName() == "species") {
			appendum = " (" + element.attribute("author") + ", " + element.attribute("year") + ") (" + element.attribute("status") + ")";
			item = new QTreeWidgetItem(QStringList(oldid + element.attribute("rus") + comment + " - " + element.attribute("lat") + appendum), 0);
		}
		else {
			QString taxoLevel = config->value("Taxo", element.tagName()).toString() + " ";
			item = new QTreeWidgetItem(QStringList(taxoLevel + element.attribute("rus").toUpper() + comment + " - " + element.attribute("lat")), 0);
		}	
		insertTaxoPart(item, element);
		item->setToolTip(0, "<font color=\"red\"><pre>" + element.attribute("rus") + comment + " - " + element.attribute("lat") + appendum + "</pre></font>");
		item->setData(0, Qt::UserRole, element.attribute("id"));
		if (element.tagName() == "species")
				taxoSpecies->append(item);
		parent->addChild(item);
	}
}

void Logic::treeItemSelected(QTreeWidgetItem *item) {
	if (!item->text(0).contains(" - ") || !item->text(0).contains(". "))
		return;
	QList<QListWidgetItem*> matches = alphaList->findItems(item->text(0).split(" - ")[0].split(". ")[1], Qt::MatchContains);
	if (matches.size() == 0)
		return;
	alphaList->setCurrentItem(matches[0]);
	listItemSelected(matches[0]);
}

void Logic::listItemSelected(QListWidgetItem *item) {
	speciesId = item->data(Qt::UserRole).toInt();
	foreach(QTreeWidgetItem *treeItem, *taxoSpecies) {
		if (treeItem->data(0, Qt::UserRole).toInt() == speciesId) {
			taxoTree->setCurrentItem(treeItem);
			break;
		}
	}
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
				stops += " stop: " + QString::number(x, 'f', 3) + " " + pageColor(*i) + ",";
				x -= (0.25/(cat.size()));
				if (i + 1 != cat.end())
					stops += " stop: " + QString::number(x + 0.005, 'f', 3) + " " + pageColor(*i) + ",";
			stops = stops.left(stops.length() - 1);
			colorPage->setStyleSheet(stylesheet + stops + ") }");
			}
		}
		else {
			colorPage->setStyleSheet("#colorPage { background-color:" + pageColor(cat[0]) + "}");
		}
		QString specialBackground = "", lc = labelColor(cat[0]), cc = commentColor(cat[0]), bc = pageColor(cat[0]);
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
	zoneId = zoneMapping->value(action);
	delete chapterLayout;
	chapterLayout = new QMap<QString, QString>(core->chapterLayout(zoneId));
	refreshSectionList();
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

QString Logic::labelColor(int cat) {
	switch (cat) {
	case 0: return "#ffffff";
	case 1: return "#ffff00";
	case 2: return "#008000";
	case 3: return "#008000";
	case 4: return "#008000";
	case 5: return "#00ff00";
	}
}

QString Logic::commentColor(int cat) {
	switch (cat) {
	case 0: return "#ffffff";
	case 1: return "#ffffff";
	case 2: return "#000000";
	case 3: return "#000000";
	case 4: return "#000000";
	case 5: return "#ffffff";
	}
}

QString Logic::pageColor(int cat) {
	switch (cat) {
	case 0: return "#000000";
	case 1: return "#ff0000";
	case 2: return "#ffffa0";
	case 3: return "#ffffff";
	case 4: return "#cccccc";
	case 5: return "#008000";
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
	if (!checkModification())
		return;
	articleId = (item->data(Qt::UserRole).toString() == "" ? "" : item->text());
	refreshArticle();
}

void Logic::nextSpecies() {
	if (!checkModification())
		return;
	if (zoneId != 1) {
		zoneId = 1;
		delete chapterLayout;
		chapterLayout = new QMap<QString, QString>(core->chapterLayout(zoneId));
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
		delete chapterLayout;
		chapterLayout = new QMap<QString, QString>(core->chapterLayout(zoneId));
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
	editMode = false;
	editAction->setVisible(true);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	articleBrowser->setReadOnly(true);
	printButton->setEnabled(true);
	backButton->setEnabled(true);
	overviewItem->setHidden(false);
}

void Logic::cancelEdit() {
	editMode = false;
	editAction->setVisible(true);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	articleBrowser->setText(original);
	articleBrowser->setReadOnly(true);
	printButton->setEnabled(true);
	backButton->setEnabled(true);
	overviewItem->setHidden(false);
}

void Logic::edit() {
	editAction->setVisible(false);
	saveAction->setVisible(true);
	cancelAction->setVisible(true);
	articleBrowser->setReadOnly(false);
	if (articleId == "") {
		overviewItem = sectionList->currentItem();
		sectionList->setCurrentItem(sectionList->item(1));
		this->setArticle(sectionList->currentItem());
	}
	else {
		overviewItem = sectionList->item(0);
	}
	prevArtRow = sectionList->currentRow();
	overviewItem->setHidden(true);
	printButton->setEnabled(false);
	backButton->setEnabled(false);
	articleBrowser->setFocus();
	editMode = true;
	original = articleBrowser->toPlainText();
}

void Logic::showHelp() {
	QDialog *helpDialog = new QDialog(qApp->activeWindow());
	QVBoxLayout *helpLayout = new QVBoxLayout(helpDialog);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	QPushButton *closeButton = new QPushButton(config->value("Labels", "close").toString());
	buttonLayout->addStretch();
	buttonLayout->addWidget(closeButton);
	QTextBrowser *helpBrowser = new QTextBrowser();
	helpLayout->addWidget(helpBrowser);
	helpLayout->addLayout(buttonLayout);
	connect(closeButton, SIGNAL(clicked()), helpDialog, SLOT(accept()));
	helpDialog->setStyleSheet("QDialog { background-color:#ffffff; color: #000000 }");
	QFile file(qApp->applicationDirPath() + "/doc/help/h" + QString::number(stack->currentIndex()) + ".html");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	helpBrowser->setHtml(QString::fromUtf8(file.readAll()));
	file.close();
	helpDialog->resize(400, 300);
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

	/** Заголовок: русское и латинское название; статус; составители  */
	QFont f = painter.font();
	QFont f2 = f;
	f2.setBold(true);
	f2.setPointSize(14);
	painter.setFont(f2);
	painter.drawText(QRect(0, 80, printer.pageRect().width(), 50), Qt::TextWordWrap, speciesLabel->text().replace("<br />","\n").remove("<div style=\"whitespace:pre-wrap\">").remove("</div>"));

	painter.drawText(0, 145, core->entryLatName(speciesId));
	painter.setFont(f);
	painter.drawText(QRect(0, 155, printer.pageRect().width(), 75), Qt::TextWordWrap, commentLabel->text());

	/** Портрет */
	painter.drawPixmap(printer.pageRect().width() - 265, 225, 225, 150, *(arealLabel->pixmap()));

	/** Карта ареала */
	painter.drawPixmap(40, 225, 225, 150, *(photoLabel->pixmap()));

	textOffset = 400;

	/** Текст статьи или выбранных рубрик */
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
	/** Логотип */
	painter.drawPixmap(0, 20, 30, 30, *(logoLabel->pixmap()));

	/** Верхний колонтитул */
	painter.drawText(QRect(40, 20, printer.pageRect().width() - 70, 30), Qt::TextWordWrap, config->value("Labels", "TopBanner").toString());

	/** Нижний колонтитул */
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
