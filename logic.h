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
	Logic(QrbConfig *config, QWidget *parent = 0);
	~Logic();

public slots:
	void indexMenuClicked();

	void viewMultiDocChapter(const QString &chapter);
    void printDocument();

	void largerFont();
	void smallerFont();

	void chapterSelected(bool isChecked);

    void populateSystematics();
    void populateAlphaList();
	void latAlpha();
	void rusAlpha();
    void sysList();

	void treeItemHighlighted(QTreeWidgetItem *item);
    void listItemHighlighted(QListWidgetItem *item, QListWidgetItem *prevItem);

	void setArticle(QListWidgetItem *item);
	void printSpecies();

	void changeFocus(QWidget *old, QWidget *now);
    //void setZone(QAction *action);

private:
    void initChapterRoots();
    void viewSingleDoc(const QString &docId, const QString &docName);
	void viewMultiDoc(const QString &id, const QString &item = "");
	void viewSpeciesLists();
	void viewSpeciesArticle();
    //void initIndex(const QString &section = "");
	void populateSystematicsBranch(QTreeWidgetItem *parent, const QDomElement &root);
    //bool checkModification();

	int firstItemId, lastItemId;
	QString currentDir, alphaMode, chapterId, articleId, indexMode;
	void refreshArticle();
	void printAux(QPainter &painter, QPrinter &printer);
	int speciesId, zoneId;
	QList<QTreeWidgetItem*> *taxoSpecies;
	GaiaCore *core;
	QrbConfig *config;
	QString currentCathegory;
	QMap<QAction*, int> *zoneMapping;
	void refreshSectionList();
	QListWidgetItem *overviewItem;
	QString original;
	int prevArtRow;

	QWidget *parent, *colorPage;
	QStackedWidget *stack;
    QListWidget *alphaList, *indexList;
	QLabel *indexLabel, *docTitleLabel, *photoLabel, *arealLabel, *speciesLabel, *commentLabel, *logoLabel;
	QTextBrowser *docViewer, *articleBrowser;
	QComboBox *chapterCombo;
    QAction *zoneMenu, *fontMenu;
	QTreeWidget *taxoTree;
    QPushButton *articlePrintButton, *docPrintButton;
	
	QMap<QString, QString> indexSections, oppositeLang;
	QString multiDocDir, lang;

	QMap<QString, QList<int> > *chapterRoots;
	QMap<QString, QString> *chapterArticles;

	QMap<int, QString> *pageColor, *labelColor, *commentColor;

};

#endif
