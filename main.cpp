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

#include "uiloader.h"
#include "config.h"

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
	
	QList<QString> params = config->parameters("Index");
	QStackedWidget *stack = redBook->findChild<QStackedWidget*>("stackedWidget");
	stack->setCurrentIndex(0);

	QListWidget *indexList = redBook->findChild<QListWidget*>("indexList");
	indexList->setCursor(Qt::PointingHandCursor);
	indexList->setSelectionMode(QAbstractItemView::NoSelection);
	QMenu *indexMenu = redBook->findChild<QMenu*>("indexMenu");
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

	QObject::connect(&app, SIGNAL(focusChanged(QWidget*, QWidget*)), stack, SLOT(changeFocus(QWidget*, QWidget*)));

	redBook->show();
	sleep(3);
	splash.finish(redBook);
	
	return app.exec();
}
