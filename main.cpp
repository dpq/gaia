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
	QMenu *indexMenu = redBook->findChild<QMenu*>("indexMenu");

	QListWidget *indexList = redBook->findChild<QListWidget*>("indexList");
	indexList->setCursor(Qt::PointingHandCursor);
	indexList->setSelectionMode(QAbstractItemView::NoSelection);

	QStackedWidget *stack = redBook->findChild<QStackedWidget*>("stackedWidget");
	QLabel *docTitle = redBook->findChild<QLabel*>("docTitle");

	if (indexMenu == 0 || indexList == 0)
		exit(1);
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
	QObject::connect(chapterCombo, SIGNAL(currentIndexChanged(const QString &)), stack, SLOT(viewChapter(const QString &)));

	QPushButton *latButton = redBook->findChild<QPushButton*>("latButton");
	QPushButton *rusButton = redBook->findChild<QPushButton*>("rusButton");
	QObject::connect(latButton, SIGNAL(clicked()), stack, SLOT(viewLatAlpha()));
	QObject::connect(rusButton, SIGNAL(clicked()), stack, SLOT(viewRusAlpha()));
	
	QRadioButton *radioc0 = redBook->findChild<QRadioButton*>("radioc0");
	QRadioButton *radioc1 = redBook->findChild<QRadioButton*>("radioc1");
	QRadioButton *radioc2 = redBook->findChild<QRadioButton*>("radioc2");
	QRadioButton *radioc3 = redBook->findChild<QRadioButton*>("radioc3");
	QRadioButton *radioc4 = redBook->findChild<QRadioButton*>("radioc4");
	QRadioButton *radioc5 = redBook->findChild<QRadioButton*>("radioc5");
	QRadioButton *radioc6 = redBook->findChild<QRadioButton*>("radioc6");
	QRadioButton *radioc7 = redBook->findChild<QRadioButton*>("radioc7");
	QObject::connect(radioc0, SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(radioc1, SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(radioc2, SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(radioc3, SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(radioc4, SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(radioc5, SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(radioc6, SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));
	QObject::connect(radioc7, SIGNAL(toggled(bool)), stack, SLOT(setTaxoChapter(bool)));

	
	stack->setCurrentIndex(0);
	redBook->show();
	
	sleep(2);
	splash.finish(redBook);
	
	return app.exec();
}
