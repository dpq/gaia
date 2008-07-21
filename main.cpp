/***************************************************************************
 *   main.cpp                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#include <QtGui/QApplication>
#include "core.h"

#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QFrame>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTextBrowser>

int main(int argc, char **argv) {
	QApplication app(argc, argv);

	GaiaCore *core = new GaiaCore();
	core->openZoneFile("zone.xml");
	core->openTaxonomyFile("taxonomy.xml");
	
	/* Menu: index, exit */
	QLabel *logoLabel, *ministryLabel;

	QFrame *redFrame;
	QListWidget *indexView;

	QFrame *blueFrame;
	QListWidget *chapterList;
	QListWidget *indexView2;
	QTextBrowser *docView;

	/* Menu: index, settings [resolutions], help, search [disabled], exit */

	QLabel *headerLabel;
	QPushButton *rusButton, *latButton, *helpButton;
	QLabel *sysLabel, *alphaLabel;
	QListWidget *sysList, *alphaList;
	QFrame *radioFrame;
	QRadioButton *c0, *c1, *c2, *c3, *c4, *c5, *c6, *c7;

	/* File, spisok vidov, index, spec.info [disable, russia-region-areal-literature], settings [resolutions + font size], help, print, exit */
	QWidget *articlePage;
	QTextBrowser *articleBrowser;
	QListWidget *articleTypeList;
	QLabel *animalPic, *arealPic;
	QLabel *animalName;
	QLabel *animalDetails;

	return app.exec();
}
