/***************************************************************************
 *   logic.cpp                                                             *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#include "logic.h"
#include <QtGui/QTextBrowser>
#include <QtCore/QDate>
#include <QtGui/QListWidgetItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QApplication>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtCore/QStringList>
#include <QtGui/QPrintDialog>
#include <QtGui/QDialog>
#include <QtCore/QFile>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtDebug>
#include "config.h"
#include "core.h"

Logic::Logic(QWidget *parent) : QObject(parent) {
	this->parent = parent;
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
	chapterLayout = new QMap<QString, QString>(core->chapterLayout(zoneId));
	articleId = "";
	chapterId = "radioc0";
	editMode = false;
	zoneId = 1;
	taxoSpecies = new QList<QTreeWidgetItem*>();
	zoneMapping = new QMap<QAction*, int>();
	currentCathegory =  "";
	stack = parent->findChild<QStackedWidget*>("stackedWidget");
}

Logic::~Logic() {
	delete core;
	delete config;
	qDeleteAll(*latAlphas);
	qDeleteAll(*rusAlphas);
	delete latAlphas;
	delete rusAlphas;
	delete chapterMap;
	delete chapterLayout;
	delete taxoSpecies;
	delete zoneMapping;
}

void Logic::initIndex() {
	QListWidget *indexList = parent->findChild<QListWidget*>("indexList");
	QLabel *indexLabel = parent->findChild<QLabel*>("indexLabel");
	indexList->clear();
	indexLabel->setText(config->value("Labels", "indexLabel").toString());
	QList<QString> params = config->parameters("Index");
	for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
		QString section = config->value("Index", *param).toString();
		QListWidgetItem *item = new QListWidgetItem(section);
		item->setData(Qt::UserRole, *param);
		indexList->addItem(item);
	}
}

void Logic::viewDocument(QListWidgetItem *item) {
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

	QTextBrowser *docViewer = parent->findChild<QTextBrowser*>("docViewer");
	QLabel *docTitle = parent->findChild<QLabel*>("docTitle");
	QComboBox *chapterCombo = parent->findChild<QComboBox*>("chapterCombo");
	QListWidget *indexList = parent->findChild<QListWidget*>("indexList");
	QLabel *indexLabel = parent->findChild<QLabel*>("indexLabel");
	
	QAction *editAction = parent->findChild<QAction*>("editAction");
	QAction *saveAction = parent->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent->findChild<QAction*>("cancelAction");
	QAction *specMenu = parent->findChild<QMenu*>("specMenu")->menuAction();
	QAction *fontMenu = parent->findChild<QMenu*>("fontMenu")->menuAction();
	
	if (indexMode == "lit") {
		if (text == config->value("Labels", "back").toString()) {
			indexMode = "";
			initIndex();
			return;
		}
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
		stack->setCurrentIndex(1);
		indexMode = "";
		return;
	}
	if (indexMode == "app") {
		if (text == config->value("Labels", "back").toString()) {
			indexMode = "";
			initIndex();
			return;
		}
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
		stack->setCurrentIndex(1);
		indexMode = "";
		return;
	}

	/* Main screen */
	if (id == "p5") {
		currentDir = "";
		parent->findChild<QRadioButton*>(chapterId)->setChecked(true);
		viewRusAlpha();
		editAction->setVisible(false);
		saveAction->setVisible(false);
		cancelAction->setVisible(false);
		specMenu->setVisible(false);
		fontMenu->setVisible(false);
		QListWidget *alphaList = parent->findChild<QListWidget*>("alphaList");
		QTreeWidget *taxoTree = parent->findChild<QTreeWidget*>("taxoTree");
		taxoTree->clearSelection();
		alphaList->clearSelection();
		stack->setCurrentIndex(2);
		//taxoTree->setGeometry(alphaList->geometry());
	}
	/* Appendix */
	else if (id == "p7") {
		if (item != 0) {
			indexList->clear();
			indexLabel->setText(config->value("Index", "p7").toString());
			QList<QString> params = config->parameters("Appendix");
			indexList->addItem(config->value("Labels", "back").toString());
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
			indexMode = "";
			stack->setCurrentIndex(1);
		}
	}
	/* Literature for animals */
	else if (id == "p6") {
		if (item != 0) {
			indexList->clear();
			indexLabel->setText(config->value("Index", "p6").toString());
			QList<QString> params = config->parameters("Chapters");
			indexList->addItem(config->value("Labels", "back").toString());
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
			indexMode = "";
			stack->setCurrentIndex(1);
		}
	}
	else {
		currentDir = "";
		QFile file(qApp->applicationDirPath() + "/doc/" + id + ".html");
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		docViewer->setHtml(QString::fromUtf8(file.readAll()));
		file.close();
		// Here and further: setSource() appears to be broken in Windows
		//docViewer->setSource(qApp->applicationDirPath() + "/doc/" + id + ".html");
		docTitle->setText(text);
		editAction->setVisible(false);
		saveAction->setVisible(false);
		cancelAction->setVisible(false);
		specMenu->setVisible(false);
		fontMenu->setVisible(true);
		chapterCombo->hide();
		stack->setCurrentIndex(1);
	}
}

void Logic::showIndex() {
	QComboBox *chapterCombo = parent->findChild<QComboBox*>("chapterCombo");
	QAction *editAction = parent->findChild<QAction*>("editAction");
	QAction *saveAction = parent->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent->findChild<QAction*>("cancelAction");
	QAction *specMenu = parent->findChild<QMenu*>("specMenu")->menuAction();
	QAction *fontMenu = parent->findChild<QMenu*>("fontMenu")->menuAction();
	currentDir = "";
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

void Logic::viewChapter(const QString &chapter) {
	if (chapter == "")
		return;
	QTextBrowser *docViewer = parent->findChild<QTextBrowser*>("docViewer");
	QComboBox *chapterCombo = qobject_cast<QComboBox*>(sender());
	QFile file(qApp->applicationDirPath() + "/doc/" + currentDir + "/" + chapterCombo->itemData(chapterCombo->currentIndex()).toString() + ".html");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	docViewer->setHtml(QString::fromUtf8(file.readAll()));
	file.close();
}

void Logic::viewLatAlpha() {
	QListWidget *alphaList = parent->findChild<QListWidget*>("alphaList");
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
			latItem->setToolTip("<font color=\"red\"><pre>" + (*i).attribute("rus") + comment + "</pre></font>");
			latItem->setData(Qt::UserRole, (*i).attribute("id"));
			alphaList->addItem(latItem);
		}
	}
	alphaMode = "lat";
	alphaList->sortItems();
}

void Logic::viewRusAlpha() {
	QListWidget *alphaList = parent->findChild<QListWidget*>("alphaList");
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
			rusItem->setToolTip("<font color=\"red\"><pre>" + (*i).attribute("lat") + "</pre></font>");
			rusItem->setData(Qt::UserRole, (*i).attribute("id"));
			alphaList->addItem(rusItem);
		}
	}	
	alphaMode = "rus";
	alphaList->sortItems();
}

void Logic::setTaxoChapter(bool isChecked) {
	if (!isChecked)
		return;
	chapterId = qobject_cast<QRadioButton*>(sender())->objectName();
	if (alphaMode == "rus")
		viewRusAlpha();
	else
		viewLatAlpha();
	updateTaxoTree();
}

void Logic::updateTaxoTree() {
	QTreeWidget *taxoTree = parent->findChild<QTreeWidget*>("taxoTree");
	taxoTree->clear();
	taxoSpecies->clear();
	for (int i = 0; i < chapterMap->value(chapterId).size(); i++) {
		QDomElement root = core->taxonomyEntry(chapterMap->value(chapterId)[i]);
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
	QListWidget *alphaList = parent->findChild<QListWidget*>("alphaList");
	if (!item->text(0).contains(" - ") || !item->text(0).contains(". "))
		return;
	QList<QListWidgetItem*> matches = alphaList->findItems(item->text(0).split(" - ")[0].split(". ")[1], Qt::MatchContains);
	if (matches.size() == 0)
		return;
	alphaList->setCurrentItem(matches[0]);
	listItemSelected(matches[0]);
}

void Logic::listItemSelected(QListWidgetItem *item) {
	//QTime benchmark;
	//benchmark.start();
	speciesId = item->data(Qt::UserRole).toInt();
	QTreeWidget *taxoTree = parent->findChild<QTreeWidget*>("taxoTree");
	foreach(QTreeWidgetItem *treeItem, *taxoSpecies) {
		if (treeItem->data(0, Qt::UserRole).toInt() == speciesId) {
			taxoTree->setCurrentItem(treeItem);
			break;
		}
	}
	//qDebug() << "Finding the current speciesId and setting taxoTree: " << benchmark.restart();

	QLabel *photoLabel = parent->findChild<QLabel*>("photoLabel");
	QLabel *arealLabel = parent->findChild<QLabel*>("arealLabel");
	QLabel *speciesLabel = parent->findChild<QLabel*>("speciesLabel");
	QLabel *commentLabel = parent->findChild<QLabel*>("commentLabel");

	// qDebug() << "Creating pointers to widgets :" << benchmark.restart();

	photoLabel->setPixmap(core->entryPicture(speciesId));
	arealLabel->setPixmap(core->speciesAreal(speciesId, zoneId));
	
	QString speciesText = core->speciesChapter(speciesId, zoneId, config->value("Labels", "Name").toString());
	QString line1 = speciesText.split("\n")[0].toUpper();
	QString line2 = speciesText.split("\n")[1];
	speciesText = speciesText.split("\n")[2] + "\n" + speciesText.split("\n")[3] + "\n";
	
	QStringList litText = core->speciesChapter(speciesId, zoneId, config->value("Labels", "Lit").toString()).split("\n", QString::SkipEmptyParts);
	
	// qDebug() << "Loading species, literature text and setting pixmaps provided by the Core: " << benchmark.restart();

	QString compilers = "";
	for (int i = litText.size() - 1; i >= 0; i--) {
		if (litText[i].trimmed().length() > 0) {
			compilers = litText[i].trimmed();
			break;
		}
	}
	// qDebug() << "Extracting compiler names: " << benchmark.restart();
	
	QList<int> cat = core->speciesStatus(speciesId, zoneId);
	QString cathegory = "";
	for (QList<int>::const_iterator i = cat.begin(); i != cat.end(); i++) {
		cathegory += QString::number(*i) + ", ";
	}
	
	// qDebug() << "Getting cathegory: " << benchmark.restart();
		
	cathegory = cathegory.left(cathegory.length() - 2);
	cathegory += " " + config->value("Labels", "cathegory").toString().toLower();
	speciesLabel->setWordWrap(true);
	speciesLabel->setText("<div style=\"whitespace:pre-wrap\">" + line1 + "<br />" + line2 + "</div>");
	commentLabel->setText(speciesText.trimmed() + "\n" + cathegory + "\n" + compilers);
	bool selectionFound = false;
	QListWidget *sectionList = parent->findChild<QListWidget*>("sectionList");
	sectionList->clear();
	sectionList->addItem(config->value("Labels", "Full").toString());
	overviewItem = sectionList->item(0);
	if (editMode) {
		overviewItem->setHidden(true);
	}
	QMap<QString, QString> parameters = core->chapterLayout(zoneId, true);
	QList<QString> keys = parameters.values();
	qSort(keys);
		
	// qDebug() << "Setting label values and retrieving section list:" << benchmark.restart();

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
	// qDebug() << "More work on sectionList: " << benchmark.restart();
	
	refreshArticle();
	// qDebug() << "Article refreshed: " << benchmark.restart();
	
	/* Colorizing */
	if (currentCathegory != cathegory) {
		QWidget *colorPage = parent->findChild<QWidget*>("colorPage");
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
			//qApp->setStyleSheet(qApp->styleSheet() + "QListWidget#sectionList::item::selected { border: 1px solid black }");
		}
		else {
			sectionList->setStyleSheet("#sectionList { background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); selection-background-color:" + bc + "; selection-color:" + lc + "}");
			//qApp->setStyleSheet(qApp->styleSheet().replace("QListWidget#sectionList::item::selected { border: 1px solid black }", ""));
		}
		currentCathegory = cathegory;
	}
	// qDebug() << "Installed colors: " << benchmark.restart();
	
	QAction *editAction = parent->findChild<QAction*>("editAction");
	QAction *saveAction = parent->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent->findChild<QAction*>("cancelAction");
	QMenu *specMenu = parent->findChild<QMenu*>("specMenu");
	QAction *fontMenu = parent->findChild<QMenu*>("fontMenu")->menuAction();
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
	specMenu->clear();
	zoneMapping->clear();
	if (speciesZones.size() > 1) {
		foreach (int zoneId, speciesZones) {
			QAction *zoneAction = specMenu->addAction(core->zoneName(zoneId));
			zoneMapping->insert(zoneAction, zoneId);
			QObject::connect(specMenu, SIGNAL(triggered(QAction*)), this, SLOT(setZone(QAction*)));
		}
		specMenu->menuAction()->setVisible(true);
	}
	else {
		specMenu->menuAction()->setVisible(false);
	}
	fontMenu->setVisible(true);
	
	// qDebug() << "Initiated the menu: " << benchmark.restart();
	
	stack->setCurrentIndex(3);
}

void Logic::setZone(QAction *action) {
	zoneId = zoneMapping->value(action);
	delete chapterLayout;
	chapterLayout = new QMap<QString, QString>(core->chapterLayout(zoneId));
	refreshSectionList();
	refreshArticle();
	//qDebug() << findChild<QWidget*>("colorPage")->styleSheet();
}

void Logic::refreshSectionList() {
	bool selectionFound = false;
	QListWidget *sectionList = parent->findChild<QListWidget*>("sectionList");
	sectionList->clear();
	sectionList->addItem(config->value("Labels", "Full").toString());
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
		parent->findChild<QTextBrowser*>("articleBrowser")->setText("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"ru\" lang=\"ru\"><head><title></title><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" /></head><body align=\"justify\" style=\"margin: 15px\"><div style=\"white-space: pre-wrap\">" + all + "</div></body><html>");
	}
	else
		parent->findChild<QTextBrowser*>("articleBrowser")->setText(core->speciesChapter(speciesId, zoneId, articleId));
	if (editMode) {
		original = parent->findChild<QTextBrowser*>("articleBrowser")->toPlainText();
		prevArtRow = parent->findChild<QListWidget*>("sectionList")->currentRow();
	}
}

#include <QtGui/QMessageBox>

bool Logic::checkModification() {
	if (editMode && original != parent->findChild<QTextBrowser*>("articleBrowser")->toPlainText()) {
		QMessageBox msgBox;
		msgBox.setText(config->value("Labels", "DocMod").toString());
		msgBox.setInformativeText(config->value("Labels", "DocSave").toString());
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();
		if (ret == QMessageBox::Cancel) {
			parent->findChild<QListWidget*>("sectionList")->setCurrentRow(prevArtRow);
			return false;
		}
		if (ret == QMessageBox::Save) {
			core->setSpeciesChapter(speciesId, zoneId, articleId, parent->findChild<QTextBrowser*>("articleBrowser")->toPlainText());
		}
		else if (ret == QMessageBox::Discard) {
			parent->findChild<QTextBrowser*>("articleBrowser")->setText(original);
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
	// TODO If we're currently editing, save the current section via the Core
	if (zoneId != 1) {
		zoneId = 1;
		delete chapterLayout;
		chapterLayout = new QMap<QString, QString>(core->chapterLayout(zoneId));
		refreshSectionList();
	}
	QTreeWidget *taxoTree = parent->findChild<QTreeWidget*>("taxoTree");
	if (taxoSpecies->indexOf(taxoTree->currentItem()) < taxoSpecies->count() - 1) {
		taxoTree->setCurrentItem(taxoSpecies->at(taxoSpecies->indexOf(taxoTree->currentItem()) + 1));
	}
	else {
		// TODO check for zero length array
		taxoTree->setCurrentItem(taxoSpecies->first());
	}
	treeItemSelected(taxoTree->currentItem());
	// TODO save a rollback version of the current section
	if (editMode)
		original = parent->findChild<QTextBrowser*>("articleBrowser")->toPlainText();
}

void Logic::prevSpecies() {
	if (!checkModification())
		return;
	// TODO If we're currently editing, save the current section via the Core
	if (zoneId != 1) {
		zoneId = 1;
		delete chapterLayout;
		chapterLayout = new QMap<QString, QString>(core->chapterLayout(zoneId));
		refreshSectionList();
	}
	QTreeWidget *taxoTree = parent->findChild<QTreeWidget*>("taxoTree");
	if (taxoSpecies->indexOf(taxoTree->currentItem()) > 0) {
		taxoTree->setCurrentItem(taxoSpecies->at(taxoSpecies->indexOf(taxoTree->currentItem()) - 1));
	}
	else {
		// TODO check for zero length array
		taxoTree->setCurrentItem(taxoSpecies->last());
	}
	treeItemSelected(taxoTree->currentItem());
	if (editMode)
		original = parent->findChild<QTextBrowser*>("articleBrowser")->toPlainText();
}


void Logic::changeFocus(QWidget *old, QWidget *now) {
	QTreeWidget *taxoTree = parent->findChild<QTreeWidget*>("taxoTree");
	QListWidget *alphaList = parent->findChild<QListWidget*>("alphaList");
	if (old == taxoTree && now == alphaList) {
		taxoTree->clearSelection();
	}
	else if(old == alphaList && now == taxoTree) {
		alphaList->clearSelection();
	}
}

void Logic::up() {
	parent->findChild<QTreeWidget*>("taxoTree")->clearSelection();
	parent->findChild<QListWidget*>("alphaList")->clearSelection();
	stack->setCurrentIndex(2);
}

void Logic::saveEdit() {
	core->setSpeciesChapter(speciesId, zoneId, articleId, parent->findChild<QTextBrowser*>("articleBrowser")->toPlainText());
	QAction *editAction = parent->findChild<QAction*>("editAction");
	QAction *saveAction = parent->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent->findChild<QAction*>("cancelAction");
	QAction *specMenu = parent->findChild<QMenu*>("specMenu")->menuAction();
	QAction *fontMenu = parent->findChild<QMenu*>("fontMenu")->menuAction();
	editMode = false;
	editAction->setVisible(true);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	parent->findChild<QTextBrowser*>("articleBrowser")->setReadOnly(true);
	QPushButton *printButton = parent->findChild<QPushButton*>("printButton");
	QPushButton *backButton = parent->findChild<QPushButton*>("backToListsButton");
	printButton->setEnabled(true);
	backButton->setEnabled(true);
	overviewItem->setHidden(false);
}

void Logic::cancelEdit() {
	QAction *editAction = parent->findChild<QAction*>("editAction");
	QAction *saveAction = parent->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent->findChild<QAction*>("cancelAction");
	QAction *specMenu = parent->findChild<QMenu*>("specMenu")->menuAction();
	QAction *fontMenu = parent->findChild<QMenu*>("fontMenu")->menuAction();
	editMode = false;
	editAction->setVisible(true);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	parent->findChild<QTextBrowser*>("articleBrowser")->setText(original);
	parent->findChild<QTextBrowser*>("articleBrowser")->setReadOnly(true);
	QPushButton *printButton = parent->findChild<QPushButton*>("printButton");
	QPushButton *backButton = parent->findChild<QPushButton*>("backToListsButton");
	printButton->setEnabled(true);
	backButton->setEnabled(true);
	overviewItem->setHidden(false);
}

/**setSpeciesChapter(int speciesId, int zoneId, const QString &chapterName, const QString &chapterHtml)*/

void Logic::edit() {
	QAction *editAction = parent->findChild<QAction*>("editAction");
	QAction *saveAction = parent->findChild<QAction*>("saveAction");
	QAction *cancelAction = parent->findChild<QAction*>("cancelAction");
	QListWidget *sectionList = parent->findChild<QListWidget*>("sectionList");
	editAction->setVisible(false);
	saveAction->setVisible(true);
	cancelAction->setVisible(true);
	parent->findChild<QTextBrowser*>("articleBrowser")->setReadOnly(false);
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
	QPushButton *printButton = parent->findChild<QPushButton*>("printButton");
	QPushButton *backButton = parent->findChild<QPushButton*>("backToListsButton");
	printButton->setEnabled(false);
	backButton->setEnabled(false);
	parent->findChild<QTextBrowser*>("articleBrowser")->setFocus();
	editMode = true;
	original = parent->findChild<QTextBrowser*>("articleBrowser")->toPlainText();
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
	helpDialog->setStyleSheet("background-color:#ffffff; color: #000000");
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
	parent->findChild<QTextBrowser*>("docViewer")->print(&printer);
}

void Logic::printSpecies() {
	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, 0);
	dialog->setWindowTitle(tr("Print Document"));
	if (dialog->exec() != QDialog::Accepted)
		return;

	QLabel *photoLabel = parent->findChild<QLabel*>("photoLabel");
	QLabel *arealLabel = parent->findChild<QLabel*>("arealLabel");
	QLabel *speciesLabel = parent->findChild<QLabel*>("speciesLabel");
	QLabel *commentLabel = parent->findChild<QLabel*>("commentLabel");
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
	QTextBrowser *articleBrowser = parent->findChild<QTextBrowser*>("articleBrowser");
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
	painter.drawPixmap(0, 20, 30, 30, *(parent->findChild<QLabel*>("logoLabel")->pixmap()));

	/** Верхний колонтитул */
	painter.drawText(QRect(40, 20, printer.pageRect().width() - 70, 30), Qt::TextWordWrap, config->value("Labels", "TopBanner").toString());

	/** Нижний колонтитул */
	painter.drawText(QRect(40, printer.pageRect().height() - 40, printer.pageRect().width() - 70, 30), Qt::TextWordWrap, config->value("Labels", "BottomBanner").toString());
	painter.drawText(QRect(printer.pageRect().width() - 70, printer.pageRect().height() - 40, printer.pageRect().width(), 30), Qt::TextWordWrap, QDate::currentDate().toString("yyyy-MM-dd"));
}

void Logic::largerFont() {
	parent->findChild<QTextBrowser*>("docViewer")->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); font: 16pt \"Sans Serif\"; padding: 15px");
	parent->findChild<QTextBrowser*>("articleBrowser")->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); font: 16pt \"Sans Serif\"; padding: 15px");
}

void Logic::smallerFont() {
	parent->findChild<QTextBrowser*>("docViewer")->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); font: 10pt \"Sans Serif\"; padding: 15px");
	parent->findChild<QTextBrowser*>("articleBrowser")->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0); font: 10pt \"Sans Serif\"; padding: 15px");
}
