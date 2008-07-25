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
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QApplication>
#include <QtGui/QRadioButton>
#include <QtCore/QStringList>
#include <QtCore/QFile>
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
	articleId = "a0";
	editMode = false;
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
	QListWidget *indexList = findChild<QListWidget*>("indexList");
	QLabel *indexLabel = findChild<QLabel*>("indexLabel");
	
	QAction *editAction = parent()->parent()->findChild<QAction*>("editAction");
	QAction *saveAction = parent()->parent()->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent()->parent()->findChild<QAction*>("cancelAction");
	QAction *specMenu = parent()->parent()->findChild<QMenu*>("specMenu")->menuAction();
	QAction *fontMenu = parent()->parent()->findChild<QMenu*>("fontMenu")->menuAction();
	
	if (indexMode == "lit") {
		currentDir = "p6";
		docTitle->setText(config->value("Index", "p6").toString());
		chapterCombo->show();
		chapterCombo->clear();
		QList<QString> params = config->parameters("Chapters");
		for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
			QString chapter = config->value("Chapters", *param).toString();
			chapterCombo->addItem(chapter, *param);
		}
		chapterCombo->setCurrentIndex(chapterCombo->findText(text));
		editAction->setVisible(false);
		saveAction->setVisible(false);
		cancelAction->setVisible(false);
		specMenu->setVisible(false);
		fontMenu->setVisible(true);
		this->setCurrentIndex(1);
		indexMode = "";
		return;
	}
	if (indexMode == "app") {
		currentDir = "p7";
		docTitle->setText(config->value("Index", "p7").toString());
		chapterCombo->show();
		chapterCombo->clear();
		QList<QString> params = config->parameters("Appendix");
		for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
			QString chapter = config->value("Appendix", *param).toString();
			chapterCombo->addItem(chapter, *param);
		}
		chapterCombo->setCurrentIndex(chapterCombo->findText(text));
		editAction->setVisible(false);
		saveAction->setVisible(false);
		cancelAction->setVisible(false);
		specMenu->setVisible(false);
		fontMenu->setVisible(true);
		this->setCurrentIndex(1);
		indexMode = "";
		return;
	}

	/* Main screen */
	if (id == "p5") {
		currentDir = "";
		radioc0->setChecked(true);
		viewRusAlpha();
		editAction->setVisible(false);
		saveAction->setVisible(false);
		cancelAction->setVisible(false);
		specMenu->setVisible(false);
		fontMenu->setVisible(false);
		this->setCurrentIndex(2);
	}
	/* Appendix */
	else if (id == "p7") {
		if (item != 0) {
			indexList->clear();
			indexLabel->setText(config->value("Index", "p7").toString());
			QList<QString> params = config->parameters("Appendix");
			for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
				QString section = config->value("Appendix", *param).toString();
				QListWidgetItem *item = new QListWidgetItem(section);
				item->setData(Qt::UserRole, *param);
				indexList->addItem(item);
			}
			indexMode = "app";
		}
		else {
			currentDir = "p7";
			docTitle->setText(config->value("Index", "p7").toString());
			chapterCombo->show();
			chapterCombo->clear();
			QList<QString> params = config->parameters("Appendix");
			for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
				QString chapter = config->value("Appendix", *param).toString();
				chapterCombo->addItem(chapter, *param);
			}
			chapterCombo->setCurrentIndex(0);
			editAction->setVisible(false);
			saveAction->setVisible(false);
			cancelAction->setVisible(false);
			specMenu->setVisible(false);
			fontMenu->setVisible(true);
			this->setCurrentIndex(1);
		}
	}
	/* Literature for animals */
	else if (id == "p6") {
		if (item != 0) {
			indexList->clear();
			indexLabel->setText(config->value("Index", "p6").toString());
			QList<QString> params = config->parameters("Chapters");
			for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
				QString section = config->value("Chapters", *param).toString();
				QListWidgetItem *item = new QListWidgetItem(section);
				item->setData(Qt::UserRole, *param);
				indexList->addItem(item);
			}
			indexMode = "lit";
		}
		else {
			currentDir = "p6";
			docTitle->setText(config->value("Index", "p6").toString());
			chapterCombo->show();
			chapterCombo->clear();
			QList<QString> params = config->parameters("Chapters");
			for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
				QString chapter = config->value("Chapters", *param).toString();
				chapterCombo->addItem(chapter, *param);
			}
			chapterCombo->setCurrentIndex(0);
			editAction->setVisible(false);
			saveAction->setVisible(false);
			cancelAction->setVisible(false);
			specMenu->setVisible(false);
			fontMenu->setVisible(true);
			this->setCurrentIndex(1);
		}
	}
	else {
		currentDir = "";
		docViewer->setSource(qApp->applicationDirPath() + "/doc/" + id + ".html");
		docTitle->setText(text);
		editAction->setVisible(false);
		saveAction->setVisible(false);
		cancelAction->setVisible(false);
		specMenu->setVisible(false);
		fontMenu->setVisible(true);
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
		QString oldid = element.attribute("oldid");
		if (oldid != QString())
			oldid += ". ";
		QString appendum = "";
		if (element.tagName() == "species")
			appendum = " (" + element.attribute("author") + ", " + element.attribute("year") + ") (" + element.attribute("status") + ")";
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(oldid + element.attribute("rus") + comment + " - " + element.attribute("lat") + appendum), 0);
		insertTaxoPart(item, element);
		item->setToolTip(0, element.attribute("rus") + comment + " - " + element.attribute("lat") + appendum);
		item->setData(0, Qt::UserRole, element.attribute("id"));
		parent->addChild(item);
	}
}

void Stack::treeItemSelected(QTreeWidgetItem *item) {
	QListWidget *alphaList = findChild<QListWidget*>("alphaList");
	if (!item->text(0).contains(" - ") || !item->text(0).contains(". "))
		return;
	QList<QListWidgetItem*> matches = alphaList->findItems(item->text(0).split(" - ")[0].split(". ")[1], Qt::MatchContains);
	if (matches.size() == 0)
		return;
	alphaList->setCurrentItem(matches[0]);
	listItemSelected(matches[0]);
}

void Stack::listItemSelected(QListWidgetItem *item) {
	speciesId = item->data(Qt::UserRole).toInt();
	findChild<QLabel*>("photoLabel")->setPixmap(core->entryPicture(speciesId));
	findChild<QLabel*>("arealLabel")->setPixmap(core->speciesAreal(speciesId, DEFAULT_ZONE));
	QString speciesText = core->speciesChapter(speciesId, DEFAULT_ZONE, "ax");
	QString line1 = speciesText.split("\n")[0].toUpper();
	QString line2 = speciesText.split("\n")[1];
	speciesText = speciesText.split("\n")[2] + "\n" + speciesText.split("\n")[3] + "\n";

	QStringList litText = core->speciesChapter(speciesId, DEFAULT_ZONE, "a7").split("\n", QString::SkipEmptyParts);
	QString compilers = "";
	for (int i = litText.size() - 1; i >= 0; i--) {
		if (litText[i].trimmed().length() > 0) {
			compilers = litText[i].trimmed();
			break;
		}
	}
	QList<int> cat = core->speciesStatus(speciesId, DEFAULT_ZONE);
	QString cathegory = "";
	for (QList<int>::const_iterator i = cat.begin(); i != cat.end(); i++) {
		cathegory += QString::number(*i) + ", ";
	}
	cathegory = cathegory.left(cathegory.length() - 2);
	cathegory += " " + config->value("Labels", "cathegory").toString().toLower();
	findChild<QLabel*>("speciesLabel")->setWordWrap(true);
	findChild<QLabel*>("speciesLabel")->setText("<div style=\"whitespace:pre-wrap\">" + line1 + "<br />" + line2 + "</div>");
	findChild<QLabel*>("commentLabel")->setText(speciesText.trimmed() + "\n" + cathegory + "\n" + compilers);
	refreshArticle();
	findChild<QListWidget*>("sectionList")->clear();
	QList<QString> parameters = config->parameters("ArticleType");
	for (QList<QString>::iterator i = parameters.begin(); i != parameters.end(); i++) {
		if (*i == "a2" && !QFile().exists(core->zoneUrl() + "/" + DEFAULT_ZONE + QString::number(speciesId) + "002.txt"))
			continue;
		QListWidgetItem *item = new QListWidgetItem(config->value("ArticleType", *i).toString());
		item->setData(Qt::UserRole, *i);
		findChild<QListWidget*>("sectionList")->addItem(item);
	}
	
	/* Colorizing */
	QWidget *colorPage = findChild<QWidget*>("colorPage");
	QString stylesheet = "background: qconicalgradient(cx:0, cy:0,";
	QString stops = "";
	float x = 1.0;
	for (QList<int>::const_iterator i = cat.begin(); i != cat.end(); i++) {
		stops += "stop: " + QString::number(x, 'f', 3) + " " + pageColor(*i) + ",";
		x -= (0.25/(cat.size()));
		if (i + 1 != cat.end())
			stops += "stop: " + QString::number(x + 0.005, 'f', 3) + " " + pageColor(*i) + ",";
	}
	stops = stops.left(stops.length() - 1);
	colorPage->setStyleSheet(stylesheet + stops + ")");
	QString specialBackground = "", lc = labelColor(cat[0]), cc = commentColor(cat[0]);
	if (cat.size() > 1) {
		specialBackground = "background-color: #ffffff; border: 1px solid black;";
		cc = "#000000";
		lc = "#000000";
	}

	findChild<QLabel*>("speciesLabel")->setStyleSheet(specialBackground + "font: 75 16pt \"Sans Serif\"; color: " + lc);
	findChild<QLabel*>("commentLabel")->setStyleSheet(specialBackground + "font: 10pt \"Sans Serif\"; color: " + cc);

	QAction *editAction = parent()->parent()->findChild<QAction*>("editAction");
	QAction *saveAction = parent()->parent()->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent()->parent()->findChild<QAction*>("cancelAction");
	QAction *specMenu = parent()->parent()->findChild<QMenu*>("specMenu")->menuAction();
	QAction *fontMenu = parent()->parent()->findChild<QMenu*>("fontMenu")->menuAction();
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
	specMenu->setVisible(true);
	fontMenu->setVisible(true);
	this->setCurrentIndex(3);
}

QString Stack::labelColor(int cat) {
	switch (cat) {
	case 0: return "#ffffff";
	case 1: return "#ffff00";
	case 2: return "#008000";
	case 3: return "#008000";
	case 4: return "#008000";
	case 5: return "#00ff00";
	}
}

QString Stack::commentColor(int cat) {
	switch (cat) {
	case 0: return "#ffffff";
	case 1: return "#ffffff";
	case 2: return "#000000";
	case 3: return "#000000";
	case 4: return "#000000";
	case 5: return "#ffffff";
	}
}

QString Stack::pageColor(int cat) {
	switch (cat) {
	case 0: return "#000000";
	case 1: return "#ff0000";
	case 2: return "#ffffa0";
	case 3: return "#ffffff";
	case 4: return "#cccccc";
	case 5: return "#008000";
	}
}

void Stack::refreshArticle() {
	QString all = "";
	if (articleId == "a0") {
		for (int i = 1; i < 8; i++) {
			if (i == 2 && !QFile().exists(core->zoneUrl() + "/" + DEFAULT_ZONE + QString::number(speciesId) + "002.txt"))
				continue;
			all += "     " + config->value("ArticleType", "a" + QString::number(i)).toString() + "\n";
			all += core->speciesChapter(speciesId, DEFAULT_ZONE, "a" + QString::number(i)) + "\n\n";
		}
		findChild<QTextBrowser*>("articleBrowser")->setText("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"ru\" lang=\"ru\"><head><title></title><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" /></head><body align=\"justify\"><div style=\"white-space: pre-wrap\">" + all + "</div></body><html>");
	}
	else
		findChild<QTextBrowser*>("articleBrowser")->setText(core->speciesChapter(speciesId, DEFAULT_ZONE, articleId));
}

void Stack::setArticle(QListWidgetItem *item) {
	articleId = item->data(Qt::UserRole).toString();
	refreshArticle();
}

void Stack::nextSpecies() {
	QListWidget *alphaList = findChild<QListWidget*>("alphaList");
	if (alphaList->count() > alphaList->currentRow() + 1) {
		alphaList->setCurrentRow(alphaList->currentRow() + 1);
	}
	else {
		alphaList->setCurrentRow(0);
	}
	listItemSelected(alphaList->currentItem());
}

void Stack::prevSpecies() {
	QListWidget *alphaList = findChild<QListWidget*>("alphaList");
	if (alphaList->currentRow() > 0) {
		alphaList->setCurrentRow(alphaList->currentRow() - 1);
	}
	else {
		alphaList->setCurrentRow(alphaList->count() - 1);
	}
	listItemSelected(alphaList->currentItem());
}

void Stack::changeFocus(QWidget *old, QWidget *now) {
	QTreeWidget *taxoTree = findChild<QTreeWidget*>("taxoTree");
	QListWidget *alphaList = findChild<QListWidget*>("alphaList");
	if (old == taxoTree && now == alphaList) {
		taxoTree->clearSelection();
	}
	else if(old == alphaList && now == taxoTree) {
		alphaList->clearSelection();
	}
}

/*void Stack::up() {
	findChild<QTreeWidget*>("taxoTree")->clearSelection();
	findChild<QListWidget*>("alphaList")->clearSelection();
	this->setCurrentIndex(2);
	
}*/

void Stack::saveEdit() {
	QAction *editAction = parent()->parent()->findChild<QAction*>("editAction");
	QAction *saveAction = parent()->parent()->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent()->parent()->findChild<QAction*>("cancelAction");
	QAction *specMenu = parent()->parent()->findChild<QMenu*>("specMenu")->menuAction();
	QAction *fontMenu = parent()->parent()->findChild<QMenu*>("fontMenu")->menuAction();
	editMode = false;
	editAction->setVisible(true);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
}

void Stack::cancelEdit() {
	QAction *editAction = parent()->parent()->findChild<QAction*>("editAction");
	QAction *saveAction = parent()->parent()->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent()->parent()->findChild<QAction*>("cancelAction");
	QAction *specMenu = parent()->parent()->findChild<QMenu*>("specMenu")->menuAction();
	QAction *fontMenu = parent()->parent()->findChild<QMenu*>("fontMenu")->menuAction();
	editMode = false;
	editAction->setVisible(true);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
}

void Stack::edit() {
	QAction *editAction = parent()->parent()->findChild<QAction*>("editAction");
	QAction *saveAction = parent()->parent()->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent()->parent()->findChild<QAction*>("cancelAction");
	editMode = true;
	editAction->setVisible(false);
	saveAction->setVisible(true);
	cancelAction->setVisible(true);
}

void Stack::showHelp() {

}

void Stack::printSpecies() {

}

void Stack::largerFont() {
	findChild<QTextBrowser*>("docViewer")->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); font: 16pt \"Sans Serif\"; padding: 15px");
	findChild<QTextBrowser*>("articleBrowser")->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); font: 16pt \"Sans Serif\"; padding: 15px");
}

void Stack::smallerFont() {
	findChild<QTextBrowser*>("docViewer")->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); font: 10pt \"Sans Serif\"; padding: 15px");
	findChild<QTextBrowser*>("articleBrowser")->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); font: 10pt \"Sans Serif\"; padding: 15px");
}
