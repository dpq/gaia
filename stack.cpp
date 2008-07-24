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
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QApplication>
#include <QtGui/QRadioButton>
#include "config.h"
#include "core.h"

Stack::Stack(QWidget *parent) : QStackedWidget(parent) {
	currentDir = "";
	core = new GaiaCore();
	latAlphas = new QList<QListWidgetItem*>();
	rusAlphas = new QList<QListWidgetItem*>();
	core->openTaxonomyFile(":/species.xml");
	core->openZoneFile(":/zones.xml");
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
}

Stack::~Stack() {
	delete core;
	qDeleteAll(*latAlphas);
	qDeleteAll(*rusAlphas);
	delete latAlphas;
	delete rusAlphas;
}

void Stack::viewDocument(QListWidgetItem *item) {
	QrbConfig *config = new QrbConfig(":/redbook.conf");
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

	QTextBrowser *docViewer = this->findChild<QTextBrowser*>("docViewer");
	QLabel *docTitle = this->findChild<QLabel*>("docTitle");
	QComboBox *chapterCombo = this->findChild<QComboBox*>("chapterCombo");
	QRadioButton *radioc0 = this->findChild<QRadioButton*>("radioc0");

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
	delete config;
}


void Stack::viewChapter(const QString &chapter) {
	if (chapter == "")
		return;
	QTextBrowser *docViewer = this->findChild<QTextBrowser*>("docViewer");
	QComboBox *chapterCombo = qobject_cast<QComboBox*>(sender());
	QString file = chapterCombo->itemData(chapterCombo->currentIndex()).toString();
	docViewer->setSource(qApp->applicationDirPath() + "/doc/" + currentDir + "/" + file + ".html");
}

void Stack::viewLatAlpha() {
	QListWidget *alphaList = this->findChild<QListWidget*>("alphaList");
	alphaList->clear();

	for (int i = 0; i < chapterMap->value(chapterId).size(); i++) {
		QDomElement root = core->taxonomyElementById(QString::number(chapterMap->value(chapterId)[i]));
		QList<QDomElement> speciesList = core->taxonomyElementsByTagName("species", root);
		for (QList<QDomElement>::iterator i = speciesList.begin(); i != speciesList.end(); i++) {
			QListWidgetItem *latItem = new QListWidgetItem();
			latItem->setText((*i).attribute("lat"));
			latItem->setToolTip((*i).attribute("rus"));
			alphaList->addItem(latItem);
		}
	}

	alphaMode = "lat";
	alphaList->sortItems();
}

void Stack::viewRusAlpha() {
	QListWidget *alphaList = this->findChild<QListWidget*>("alphaList");
	alphaList->clear();

	for (int i = 0; i < chapterMap->value(chapterId).size(); i++) {
		QDomElement root = core->taxonomyElementById(QString::number(chapterMap->value(chapterId)[i]));		
		QList<QDomElement> speciesList = core->taxonomyElementsByTagName("species", root);
		for (QList<QDomElement>::iterator i = speciesList.begin(); i != speciesList.end(); i++) {
			QListWidgetItem *rusItem = new QListWidgetItem();
			rusItem->setText((*i).attribute("rus"));
			rusItem->setToolTip((*i).attribute("lat"));
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
	//updateTaxoTree();
}
