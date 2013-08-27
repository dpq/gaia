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
    core->readTaxonomyFile(":/species.xml");
    core->readZoneFile(":/zones.xml");
    this->config = config;
	
    initChapterRoots();
    chapterArticles = new QMap<QString, QString>(core->chapterLayout(zoneId));
	
    pageColor = new QMap<int, QString>();
    labelColor = new QMap<int, QString>();
    commentColor = new QMap<int, QString>();

    zoneMapping = new QMap<QAction*, int>();
    chapterId = "radioc0";
    lang = "rus";
    oppositeLang["lat"] = "rus";
    oppositeLang["rus"] = "lat";
    zoneId = 1;
    speciesId = -1;
    taxoSpecies = new QList<QTreeWidgetItem*>();
    currentCategory =  "";

    this->parent = parent;
    stack = parent->findChild<QStackedWidget*>("stackedWidget");

    zoneMenu = parent->findChild<QMenu*>("zoneMenu")->menuAction();
    fontMenu = parent->findChild<QMenu*>("fontMenu")->menuAction();

    docViewer = parent->findChild<QTextBrowser*>("docViewer");
    docTitleLabel = parent->findChild<QLabel*>("docTitleLabel");
    chapterCombo = parent->findChild<QComboBox*>("chapterCombo");
    docPrintButton = parent->findChild<QPushButton*>("docPrintButton");

    articleBrowser = parent->findChild<QTextBrowser*>("articleBrowser");
    alphaList = parent->findChild<QListWidget*>("alphaList");
    taxoTree = parent->findChild<QTreeWidget*>("taxoTree");
    photoLabel = parent->findChild<QLabel*>("photoLabel");
    arealLabel = parent->findChild<QLabel*>("arealLabel");
    logoLabel = parent->findChild<QLabel*>("logoLabel");
    speciesLabel = parent->findChild<QLabel*>("speciesLabel");
    commentLabel = parent->findChild<QLabel*>("commentLabel");
    colorPage = parent->findChild<QWidget*>("colorPage");
    articlePrintButton = parent->findChild<QPushButton*>("articlePrintButton");
	
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

void Logic::chapterSelected(bool isChecked) {
    if (!isChecked) {
        return;
    }
    chapterId = qobject_cast<QRadioButton*>(sender())->objectName();
    populateAlphaList();
    populateSystematics();
}

void Logic::initChapterRoots() {
	chapterRoots = new QMap<QString, QList<int> >();
	foreach (const QString &chapter, config->parameters("ChapterRoots")) {
        QList<int> idList;
        foreach (const QString &id, config->value("ChapterRoots", chapter).toString().split(" ")) {
			idList.append(id.toInt());
		}
		chapterRoots->insert(chapter, idList);
	}
}

void Logic::viewSingleDoc(const QString &docId, const QString &docName) {
	QFile file(qApp->applicationDirPath() + "/doc/" + docId + ".html");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	docViewer->setHtml(QString::fromUtf8(file.readAll()));
	file.close();
	docTitleLabel->setText(docName);
    zoneMenu->setVisible(false);
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
    zoneMenu->setVisible(false);
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

void Logic::indexMenuClicked() {
	QString text = qobject_cast<QAction*>(sender())->text();
	QString id = qobject_cast<QAction*>(sender())->data().toString();
	if (id == "p6" || id == "p7") {
        //initIndex(id);
		viewMultiDoc(id);
	}
    else if (id == "p5") {
        //initIndex();
        //viewSpeciesLists();
    }
    else {
        //initIndex();
		viewSingleDoc(id, text);
	}
}

//void Logic::viewSpeciesLists() {
//	parent->findChild<QRadioButton*>(chapterId)->setChecked(true);
//	populateAlphaList();
//    zoneMenu->setVisible(false);
//	fontMenu->setVisible(false);
//	taxoTree->clearSelection();
//	alphaList->clearSelection();
//	stack->setCurrentIndex(2);
//}

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
    if (speciesId != -1) {
        foreach(QTreeWidgetItem *treeItem, *taxoSpecies) {
            if (treeItem->data(0, Qt::UserRole).toInt() == speciesId) {
                taxoTree->setCurrentItem(treeItem);
                break;
            }
        }
    }
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
        QList<int> speciesList = core->taxonomyElements("species", core->taxonomyEntry(taxonomyId));
        for (int i = 0; i < speciesList.size(); i++) {
			QListWidgetItem *item = new QListWidgetItem();
            text["lat"] = core->entryLatName(speciesList.at(i));
            text["rus"] = core->entryRusName(speciesList.at(i));
            QString comment = core->speciesComment(speciesList.at(i));
            if (comment.length() > 0) {
                text["rus"] = QString("%1 [%2]").arg(text["rus"]).arg(comment);
            }
            item->setText(text[lang]);
            item->setToolTip("<font color=\"red\"><pre>" + text[oppositeLang[lang]] + "</pre></font>");
            item->setData(Qt::UserRole, speciesList.at(i));
			alphaList->addItem(item);
		}
	}
    alphaList->sortItems();
    if (speciesId == -1) {
        alphaList->setCurrentItem(alphaList->itemAt(0, 0));
    }
    else {
        foreach (QListWidgetItem *listItem, alphaList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard)) {
            if (listItem->data(Qt::UserRole).toInt() == speciesId) {
                alphaList->setCurrentItem(listItem);
                break;
            }
        }
    }
}

void Logic::latAlpha() {
	lang = "lat";
    populateAlphaList();
    alphaList->show();
    taxoTree->hide();
}

void Logic::rusAlpha() {
	lang = "rus";
    populateAlphaList();
    alphaList->show();
    taxoTree->hide();
}

void Logic::sysList() {
    populateSystematics();
    alphaList->hide();
    taxoTree->show();
}

void Logic::treeItemHighlighted(QTreeWidgetItem *item) {
    if (taxoSpecies->indexOf(item) == -1) {
        return;
    }
    speciesId = item->data(0, Qt::UserRole).toInt();
    foreach (QListWidgetItem *listItem, alphaList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard)) {
        if (listItem->data(Qt::UserRole).toInt() == speciesId) {
            alphaList->setCurrentItem(listItem);
            break;
        }
    }
    viewSpeciesArticle();
}

void Logic::listItemHighlighted(QListWidgetItem *item, QListWidgetItem *prevItem) {
    if (item) {
        speciesId = item->data(Qt::UserRole).toInt();
        foreach(QTreeWidgetItem *treeItem, *taxoSpecies) {
            if (treeItem->data(0, Qt::UserRole).toInt() == speciesId) {
                taxoTree->setCurrentItem(treeItem);
                break;
            }
        }
        viewSpeciesArticle();
    }
}

void Logic::viewSpeciesArticle() {
    photoLabel->setPixmap(core->speciesPicture(speciesId, zoneId));
	arealLabel->setPixmap(core->speciesAreal(speciesId, zoneId));
	
    QStringList lines = core->speciesChapter(speciesId, zoneId, config->value("Labels", "name").toString()).split("\n");
    QString line1 = lines[0].toUpper();
    QString line2 = lines[1];
    QString speciesText = lines[2] + "\n" + lines[3] + "\n";
	QStringList litText = core->speciesChapter(speciesId, zoneId, config->value("Labels", "lit").toString()).split("\n", QString::SkipEmptyParts);

	QString compilers = "";
	for (int i = litText.size() - 1; i >= 0; i--) {
		if (litText[i].trimmed().length() > 0) {
			compilers = litText[i].trimmed();
			break;
		}
	}
	
    QList<int> cat = core->speciesStatus(speciesId, zoneId);
    QString category = "";
	for (QList<int>::const_iterator i = cat.begin(); i != cat.end(); i++) {
        category += QString::number(*i) + ", ";
	}
    category = category.left(category.length() - 2);
    category += " " + config->value("Labels", "category").toString().toLower();
	speciesLabel->setWordWrap(true);
	speciesLabel->setText("<div style=\"whitespace:pre-wrap\">" + line1 + "<br />" + line2 + "</div>");
    commentLabel->setText(speciesText.trimmed() + "\n" + category + "\n" + compilers);
	QMap<QString, QString> parameters = core->chapterLayout(zoneId, true);
	QList<QString> keys = parameters.values();
	qSort(keys);

    refreshArticle();

    /* Fill the background etc depending on the status of the species population(s) */
    if (currentCategory != category) {
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
        currentCategory = category;
	}
	
	/* Check the Core to see if this species exists in other zones as well */
	QList<int> speciesZones = core->speciesZones(speciesId);
    zoneMenu->menu()->clear();
	zoneMapping->clear();
	if (speciesZones.size() > 1) {
		foreach (int zoneId, speciesZones) {
            QAction *zoneAction = zoneMenu->menu()->addAction(core->zoneName(zoneId));
			zoneMapping->insert(zoneAction, zoneId);
            QObject::connect(zoneMenu->menu(), SIGNAL(triggered(QAction*)), this, SLOT(setZone(QAction*)));
		}
        zoneMenu->setVisible(true);
	}
	else {
        zoneMenu->setVisible(false);
	}
}

/*void Logic::setZone(QAction *action) {
	bool isHidden = overviewItem->isHidden();
    checkModification();
	zoneId = zoneMapping->value(action);
	delete chapterArticles;
	chapterArticles = new QMap<QString, QString>(core->chapterLayout(zoneId));
	overviewItem = sectionList->item(0);
	if (isHidden) {
		overviewItem->setHidden(true);
		sectionList->setCurrentItem(sectionList->item(1));
		this->setArticle(sectionList->currentItem());
	}
	refreshArticle();
}*/

void Logic::refreshArticle() {
	QString all = "";
    QMap<QString, QString> parameters = core->chapterLayout(zoneId, true);
    QList<QString> keys = parameters.values();
    qSort(keys);
    for (QList<QString>::iterator i = keys.begin(); i != keys.end(); i++) {
        QString chapter = core->speciesChapter(speciesId, zoneId, parameters.key(*i));
        if (chapter.length() > 0) {
            all += "&nbsp;&nbsp;&nbsp;<b>" + parameters.key(*i) + "</b>\n";
            all += chapter + "\n\n";
        }
    }
    articleBrowser->setText("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"ru\" lang=\"ru\"><head><title></title><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" /></head><body align=\"justify\" style=\"margin: 15px\"><div style=\"white-space: pre-wrap\">" + all + "</div></body><html>");
}

void Logic::changeFocus(QWidget *old, QWidget *now) {
	if (old == taxoTree && now == alphaList) {
		taxoTree->clearSelection();
	}
	else if(old == alphaList && now == taxoTree) {
		alphaList->clearSelection();
	}
}
/*

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
}*/

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
            str = doc[i] + ". ";
        }
        else {
            str += doc[i] + ". ";
        }
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
