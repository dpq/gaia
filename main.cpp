/***************************************************************************
 *   main.cpp                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008-2009 David Parunakian                                  *
 ***************************************************************************/
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QtUiTools/QUiLoader>
#include <QtCore/QFile>
#include <QtCore/QTime>

#include <QtGui/QComboBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QListWidget>

#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>

#include <QtGui/QMenuBar>

#include "logic.h"
#include "config.h"

void delay(int secs) {
    QTime dieTime= QTime::currentTime().addSecs(secs);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

int main(int argc, char **argv) {
	/* Initiate the application */
	QApplication app(argc, argv);
	QApplication::setWindowIcon(QIcon(":/icon.png"));

	app.processEvents();

	/* Load the user interface file */
	QUiLoader loader;
	QFile file(":/redbook.ui");
	file.open(QFile::ReadOnly);
	QWidget *redBook = loader.load(&file, 0);
	file.close();

	/* Load the configuration file */
	QrbConfig *config = new QrbConfig(":/redbook.conf");

    /* Populate the menu bar */
	QMenuBar *menuBar = redBook->findChild<QMenuBar*>("menuBar");
	QMenu *indexMenu = menuBar->addMenu(config->value("Labels", "index").toString());
	indexMenu->setObjectName("indexMenu");
    QMenu *zoneMenu = menuBar->addMenu(config->value("Labels", "spec").toString());
    zoneMenu->setObjectName("zoneMenu");
	QMenu *fontMenu = menuBar->addMenu(config->value("Labels", "font").toString());
	fontMenu->setObjectName("fontMenu");
    QAction *lgAction = fontMenu->addAction(config->value("Labels", "font1").toString());
	QAction *smAction = fontMenu->addAction(config->value("Labels", "font0").toString());

    /* Initiate program logic */
    Logic *logic = new Logic(config, redBook);

    foreach (const QString &param, config->parameters("Index")) {
        QAction *action = indexMenu->addAction(config->value("Index", param).toString());
        action->setData(param);
        QObject::connect(action, SIGNAL(triggered()), logic, SLOT(indexMenuClicked()));
    }

    QObject::connect(smAction, SIGNAL(activated()), logic, SLOT(smallerFont()));
    QObject::connect(lgAction, SIGNAL(activated()), logic, SLOT(largerFont()));

    QObject::connect(redBook->findChild<QRadioButton*>("radioc0"), SIGNAL(toggled(bool)), logic, SLOT(chapterSelected(bool)));
    QObject::connect(redBook->findChild<QRadioButton*>("radioc1"), SIGNAL(toggled(bool)), logic, SLOT(chapterSelected(bool)));
    QObject::connect(redBook->findChild<QRadioButton*>("radioc2"), SIGNAL(toggled(bool)), logic, SLOT(chapterSelected(bool)));
    QObject::connect(redBook->findChild<QRadioButton*>("radioc3"), SIGNAL(toggled(bool)), logic, SLOT(chapterSelected(bool)));
    QObject::connect(redBook->findChild<QRadioButton*>("radioc4"), SIGNAL(toggled(bool)), logic, SLOT(chapterSelected(bool)));
    QObject::connect(redBook->findChild<QRadioButton*>("radioc5"), SIGNAL(toggled(bool)), logic, SLOT(chapterSelected(bool)));
    QObject::connect(redBook->findChild<QRadioButton*>("radioc6"), SIGNAL(toggled(bool)), logic, SLOT(chapterSelected(bool)));
    QObject::connect(redBook->findChild<QRadioButton*>("radioc7"), SIGNAL(toggled(bool)), logic, SLOT(chapterSelected(bool)));

    QObject::connect(redBook->findChild<QPushButton*>("latButton"), SIGNAL(clicked()), logic, SLOT(latAlpha()));
    QObject::connect(redBook->findChild<QPushButton*>("rusButton"), SIGNAL(clicked()), logic, SLOT(rusAlpha()));
    QObject::connect(redBook->findChild<QPushButton*>("sysButton"), SIGNAL(clicked()), logic, SLOT(sysList()));

    QObject::connect(redBook->findChild<QTreeWidget*>("taxoTree"), SIGNAL(itemClicked(QTreeWidgetItem*, int)), logic, SLOT(treeItemHighlighted(QTreeWidgetItem *)));
    QObject::connect(redBook->findChild<QListWidget*>("alphaList"), SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), logic, SLOT(listItemHighlighted(QListWidgetItem *, QListWidgetItem *)));

    QObject::connect(redBook->findChild<QPushButton*>("articlePrintButton"), SIGNAL(clicked()), logic, SLOT(printSpecies()));

    QObject::connect(redBook->findChild<QPushButton*>("docPrintButton"), SIGNAL(clicked()), logic, SLOT(printDocument()));
    QObject::connect(redBook->findChild<QComboBox*>("chapterCombo"), SIGNAL(currentIndexChanged(const QString &)), logic, SLOT(viewMultiDocChapter(const QString &)));

    QObject::connect(&app, SIGNAL(focusChanged(QWidget*, QWidget*)), logic, SLOT(changeFocus(QWidget*, QWidget*)));

	app.setStyleSheet("QRadioButton::indicator::checked { image: url(\":/radio.png\") } QListWidget#indexList::item::hover { background: qlineargradient(spread:pad, x1:0, y1:1,  x2:0, y2:0, stop:0 rgba(255, 255, 255, 255), stop:0.0157895 rgba(255, 197, 166, 255), stop:0.647368 rgba(255, 255, 255, 255), stop:0.757895 rgba(255, 255, 255, 255), stop:1 rgba(255, 220, 197, 255)); }");

    redBook->show();
    redBook->findChild<QRadioButton*>("radioc0")->toggle();
    logic->rusAlpha();
    //redBook->findChild<QListWidget*>("alphaList")->setCurrentItem(redBook->findChild<QListWidget*>("alphaList")->itemAt(0, 0));
    return app.exec();
}
