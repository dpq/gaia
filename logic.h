/***************************************************************************
 *   logic.h                                                               *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#ifndef LOGIC_H
#define LOGIC_H
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtGui/QStackedWidget>
class QListWidget;
class QListWidgetItem;
class QTreeWidget;
class QTreeWidgetItem;

class QDomElement;
class GaiaCore;
class QrbConfig;

class QAction;
class QPainter;
class QPrinter;

class QLabel;
class QTextBrowser;
class QComboBox;
class QPushButton;

class Logic : public QObject {
Q_OBJECT

public:
	Logic(QWidget *parent = 0);
	~Logic();

public slots:
	void viewDocument(QListWidgetItem *item = 0);
	void viewChapter(const QString &chapter);

	void setAlphaListLang(const QString &lang);
	void latAlpha();
	void rusAlpha();

	void updateTaxoTree();
	void setTaxoChapter(bool isChecked);
	void treeItemSelected(QTreeWidgetItem *item);
	void listItemSelected(QListWidgetItem *item);

	void setArticle(QListWidgetItem *item);

	void nextSpecies();
	void prevSpecies();

	void printSpecies();
	void printDocument();

	void changeFocus(QWidget *old, QWidget *now);

	void largerFont();
	void smallerFont();

	void up();
	void showHelp();
	void showIndex();
	void initChapterMap();
	void initIndex();

	void setZone(QAction *action);

	void edit();
	void saveEdit();
	void cancelEdit();

private:
	bool checkModification();
	int firstItemId, lastItemId;
	QString pageColor(int cat);
	QString commentColor(int cat);
	QString labelColor(int cat);
	QString currentDir, alphaMode, chapterId, articleId, indexMode;
	void refreshArticle();
	void printAux(QPainter &painter, QPrinter &printer);
	int speciesId, zoneId;
	QList<QTreeWidgetItem*> *taxoSpecies;
	bool editMode;
	GaiaCore *core;
	QrbConfig *config;
	QString currentCathegory;
	QList<QListWidgetItem*> *latAlphas;
	QList<QListWidgetItem*> *rusAlphas;
	QMap<QString, QList<int> > *chapterMap;
	QMap<QString, QString> *chapterLayout;
	QMap<QAction*, int> *zoneMapping;
	void insertTaxoPart(QTreeWidgetItem *parent, const QDomElement &root);
	void refreshSectionList();
	QListWidgetItem *overviewItem;
	QString original;
	int prevArtRow;

	QWidget *parent, *colorPage;
	QStackedWidget *stack;
	QListWidget *indexList, *alphaList, *sectionList;
	QLabel *indexLabel, *docTitle, *photoLabel, *arealLabel, *speciesLabel, *commentLabel;
	QTextBrowser *docViewer, *articleBrowser;
	QComboBox *chapterCombo;
	QAction *editAction, *saveAction, *cancelAction, *specMenu, *fontMenu;
	QTreeWidget *taxoTree;
	QPushButton *printButton, *backButton;
};

#endif
