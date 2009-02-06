/***************************************************************************
 *   uiloader.cpp                                                          *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2008 David Parunakian                                       *
 ***************************************************************************/
#include "uiloader.h"
#include "stack.h"

UiLoader::UiLoader(QObject *parent) : QUiLoader(parent) {}

QWidget *UiLoader::createWidget(const QString &className, QWidget *parent, const QString &name) {
	if (className == QLatin1String("Stack")) {
		QWidget *stack = new Stack(parent);
		stack->setObjectName(name);
		return stack;
	}
	return QUiLoader::createWidget(className, parent, name);
}
