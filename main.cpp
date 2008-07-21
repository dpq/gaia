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
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QCursor>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QTextBrowser>
#include <QtGui/QStackedWidget>

#include "uiloader.h"
#include "core.h"
#include "config.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	GaiaCore *core = new GaiaCore();
	
	UiLoader loader;
	QFile file(":/redbook.ui");
	file.open(QFile::ReadOnly);
	QWidget *redBook = loader.load(&file, 0);
	file.close();

	QrbConfig *config = new QrbConfig(":/redbook.conf");


	//QLabel *ministryLabel = redBook->findChild<QLabel*>("ministryLabel");
	//QLabel *indexLabel = redBook->findChild<QLabel*>("indexLabel");

/*	QList<QLabel*> *widgets = new QList<QLabel*>();
	widgets->append(redBook->findChild<QLabel*>("ministryLabel"));
	widgets->append(redBook->findChild<QLabel*>("indexLabel"));

	for (QList<QLabel*>::iterator i = widgets->begin(); i != widgets->end(); i++) {
		(*i)->setText(config->value("Labels", (*i)->objectName()).toString());
	}*/

	/*QLabel * = redBook->findChild<QLabel*>("");
	QLabel * = redBook->findChild<QLabel*>("");
	QLabel * = redBook->findChild<QLabel*>("");
	QLabel * = redBook->findChild<QLabel*>("");
	QLabel * = redBook->findChild<QLabel*>("");*/
	
	QList<QString> params = config->parameters("Index");
	QMenu *indexMenu = redBook->findChild<QMenu*>("indexMenu");

	QListWidget *indexList = redBook->findChild<QListWidget*>("indexList");
	indexList->setCursor(Qt::PointingHandCursor);
	indexList->setSelectionMode(QAbstractItemView::NoSelection);

	QStackedWidget *stack = redBook->findChild<QStackedWidget*>("stackedWidget");
	QLabel *docTitle = redBook->findChild<QLabel*>("docTitle");
	QComboBox *chapterCombo = redBook->findChild<QComboBox*>("chapterCombo");
	QTextBrowser *docViewer = redBook->findChild<QTextBrowser*>("docViewer");

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
	QObject::connect(chapterCombo, SIGNAL(currentIndexChanged(const QString &)), stack, SLOT(viewChapter(const QString &)));

	stack->setCurrentIndex(0);
	redBook->show();


	return app.exec();
}
