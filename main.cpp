/***************************************************************************
 *   main.cpp                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#include <QtGui/QApplication>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtGui/QSplashScreen>
#include <QtGui/QBitmap>
#include <QtGui/QMenu>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QCursor>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QTextBrowser>
#include <QtGui/QStackedWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QMenuBar>
#include "uiloader.h"
#include "config.h"
#define OPERATOR

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	QApplication::setWindowIcon(QIcon(":/icon.png"));

	UiLoader loader;
	QFile file(":/redbook.ui");
	file.open(QFile::ReadOnly);
	QWidget *redBook = loader.load(&file, 0);
	file.close();

	QrbConfig *config = new QrbConfig(":/redbook.conf");

	QPixmap pic(":/splash.png");
	QSplashScreen splash(pic);
	splash.show();
	app.processEvents();

	QMenuBar *menuBar = redBook->findChild<QMenuBar*>("menuBar");
	QStackedWidget *stack = redBook->findChild<QStackedWidget*>("stackedWidget");
	
#ifdef OPERATOR
	QAction *editAction = menuBar->addAction(config->value("Labels", "edit").toString());
	editAction->setObjectName("editAction");
	QAction *saveAction = menuBar->addAction(config->value("Labels", "save").toString());
	saveAction->setObjectName("saveAction");
	QAction *cancelAction = menuBar->addAction(config->value("Labels", "cancel").toString());
	cancelAction->setObjectName("cancelAction");
	QObject::connect(editAction, SIGNAL(activated()), stack, SLOT(edit()));
	QObject::connect(saveAction, SIGNAL(activated()), stack, SLOT(saveEdit()));
	QObject::connect(cancelAction, SIGNAL(activated()), stack, SLOT(cancelEdit()));

	menuBar->addSeparator();
#endif

	QMenu *indexMenu = new QMenu(config->value("Labels", "indexLabel").toString());
	indexMenu->setObjectName("indexMenu");
	menuBar->addMenu(indexMenu);

	/*QAction *upAction = menuBar->addAction(config->value("Labels", "lists").toString());
	upAction->setObjectName("upAction");
	*/
	QObject::connect(redBook->findChild<QPushButton*>("backToListsButton"), SIGNAL(clicked()), stack, SLOT(up()));

	QMenu *specMenu = menuBar->addMenu(config->value("Labels", "spec").toString());
	specMenu->setObjectName("specMenu");
	//QAction *rfAction = specMenu->addAction(config->value("Labels", "rf").toString());
	//QAction *subjAction = specMenu->addAction(config->value("Labels", "subj").toString());
	//QAction *arealAction = specMenu->addAction(config->value("Labels", "areal").toString());
	
	QMenu *fontMenu = menuBar->addMenu(config->value("Labels", "settings").toString());
	fontMenu->setObjectName("fontMenu");
	QAction *lgAction = fontMenu->addAction(config->value("Labels", "font1").toString());
	QAction *smAction = fontMenu->addAction(config->value("Labels", "font0").toString());
	QObject::connect(smAction, SIGNAL(activated()), stack, SLOT(smallerFont()));
	QObject::connect(lgAction, SIGNAL(activated()), stack, SLOT(largerFont()));

	QAction *helpAction = menuBar->addAction(config->value("Labels", "help").toString());
	helpAction->setObjectName("helpAction");
	QObject::connect(helpAction, SIGNAL(activated()), stack, SLOT(showHelp()));

	QList<QString> params = config->parameters("Index");

	QListWidget *indexList = redBook->findChild<QListWidget*>("indexList");
	indexList->setCursor(Qt::PointingHandCursor);
	indexList->setSelectionMode(QAbstractItemView::NoSelection);
	indexList->clear();
	for (QList<QString>::iterator param = params.begin(); param != params.end(); param++) {
		QString section = config->value("Index", *param).toString();
		QAction *action = indexMenu->addAction(section);
		action->setData(*param);
		QObject::connect(action, SIGNAL(triggered()), stack, SLOT(viewDocument()));
		QListWidgetItem *item = new QListWidgetItem(section);
		item->setData(Qt::UserRole, *param);
		indexList->addItem(item);
	}
	QObject::connect(indexList, SIGNAL(itemClicked(QListWidgetItem*)), stack, SLOT(viewDocument(QListWidgetItem*)));

	QComboBox *chapterCombo = redBook->findChild<QComboBox*>("chapterCombo");
	params = config->parameters("Appendix");

	QObject::connect(redBook->findChild<QComboBox*>("chapterCombo"), SIGNAL(currentIndexChanged(const QString &)), stack, SLOT(viewChapter(const QString &)));

	QObject::connect(redBook->findChild<QPushButton*>("latButton"), SIGNAL(clicked()), stack, SLOT(viewLatAlpha()));
	QObject::connect(redBook->findChild<QPushButton*>("rusButton"), SIGNAL(clicked()), stack, SLOT(viewRusAlpha()));
	
	QObject::connect(redBook->findChild<QRadioButton*>("radioc0"), SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(redBook->findChild<QRadioButton*>("radioc1"), SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(redBook->findChild<QRadioButton*>("radioc2"), SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(redBook->findChild<QRadioButton*>("radioc3"), SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(redBook->findChild<QRadioButton*>("radioc4"), SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(redBook->findChild<QRadioButton*>("radioc5"), SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(redBook->findChild<QRadioButton*>("radioc6"), SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(redBook->findChild<QRadioButton*>("radioc7"), SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));

	QObject::connect(redBook->findChild<QListWidget*>("sectionList"), SIGNAL(itemClicked(QListWidgetItem*)), stack, SLOT(setArticle(QListWidgetItem*)));

	QObject::connect(redBook->findChild<QTreeWidget*>("taxoTree"), SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), stack, SLOT(treeItemSelected(QTreeWidgetItem *)));
	QObject::connect(redBook->findChild<QListWidget*>("alphaList"), SIGNAL(itemDoubleClicked(QListWidgetItem*)), stack, SLOT(listItemSelected(QListWidgetItem *)));

	QObject::connect(redBook->findChild<QPushButton*>("prevButton"), SIGNAL(clicked()), stack, SLOT(prevSpecies()));
	QObject::connect(redBook->findChild<QPushButton*>("nextButton"), SIGNAL(clicked()), stack, SLOT(nextSpecies()));
	QObject::connect(redBook->findChild<QPushButton*>("printButton"), SIGNAL(clicked()), stack, SLOT(printSpecies()));
	QObject::connect(redBook->findChild<QPushButton*>("docPrintButton"), SIGNAL(clicked()), stack, SLOT(printDocument()));

	QObject::connect(redBook->findChild<QPushButton*>("docBackButton"), SIGNAL(clicked()), stack, SLOT(showIndex()));
	QObject::connect(redBook->findChild<QPushButton*>("speciesBackButton"), SIGNAL(clicked()), stack, SLOT(showIndex()));
	QObject::connect(&app, SIGNAL(focusChanged(QWidget*, QWidget*)), stack, SLOT(changeFocus(QWidget*, QWidget*)));

	editAction->setVisible(false);
	saveAction->setVisible(false);
	cancelAction->setVisible(false);
	specMenu->menuAction()->setVisible(false);
	fontMenu->menuAction()->setVisible(false);
	QString advStyle = "";
	advStyle += "QRadioButton::indicator::checked { image: url(\":/radio.png\") }";
	advStyle += "QListWidget#indexList::item::hover { background: qlineargradient(spread:pad, x1:0, y1:1,  x2:0, y2:0, stop:0 rgba(255, 255, 255, 255), stop:0.0157895 rgba(255, 197, 166, 255), stop:0.647368 rgba(255, 255, 255, 255), stop:0.757895 rgba(255, 255, 255, 255), stop:1 rgba(255, 220, 197, 255)); }";
	app.setStyleSheet(advStyle);
	stack->setCurrentIndex(0);
	sleep(2);
	redBook->show();
	splash.finish(redBook);
	//QTimer::singleShot(3000, redBook, SLOT(showMaximized()));
	//redBook->showMaximized();
	//Windows doesn't have a sleep(3) call
	return app.exec();
}
