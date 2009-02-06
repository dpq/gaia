/***************************************************************************
 *   uiloader.h                                                            *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#include <QtUiTools/QUiLoader>

class UiLoader : public QUiLoader {
public:
	UiLoader(QObject *parent = 0);
	virtual QWidget *createWidget(const QString &className, QWidget *parent = 0, const QString &name = QString());
};
