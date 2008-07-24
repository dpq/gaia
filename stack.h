/***************************************************************************
 *   stack.h                                                               *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#ifndef STACK_H
#define STACK_H
#include <QtGui/QStackedWidget>
class QListWidgetItem;
class QTreeWidgetItem;
class QDomElement;
class GaiaCore;
class QrbConfig;

class Stack : public QStackedWidget {
Q_OBJECT

public:
	Stack(QWidget *parent = 0);
	~Stack();

public slots:
	void viewDocument(QListWidgetItem *item = 0);
	void viewChapter(const QString &chapter);
	void viewLatAlpha();
	void viewRusAlpha();
	void updateTaxoTree();
	void setTaxoChapter(bool isChecked);
	void treeItemSelected(QTreeWidgetItem *item);
	void listItemSelected(QListWidgetItem *item);
	void setArticle(QListWidgetItem *item);

private:
	QString currentDir, alphaMode, chapterId, articleId;
	void refreshArticle();
	int speciesId;
	GaiaCore *core;
	QrbConfig *config;
	QList<QListWidgetItem*> *latAlphas;
	QList<QListWidgetItem*> *rusAlphas;
	QMap<QString, QList<int> > *chapterMap;
	QMap<QString, QString> *chapterLayout;
	void insertTaxoPart(QTreeWidgetItem *parent, const QDomElement &root);
};

#endif
