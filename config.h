/***************************************************************************
 *   config.h                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2007-2008 David Parunakian                                  *
 ***************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H
#include <QtCore/QList>
#include <QtCore/QHash>
class QString;
class QVariant;
class QPoint;
class QPainter;
class QPrinter;

class QrbConfig {

public:
	enum Error { Ok, NotFound, Forbidden, NoSuchSection, NoSuchParameter, BadSyntax };
    QrbConfig(const QString &path);
	~QrbConfig();

	QList<QString> sections() const;
	QList<QString> parameters(const QString &section) const;
	QVariant value(const QString &section, const QString &parameter) const;

    void error(Error *code, int *line);

private:
    void initValue(const QString &section, const QString &parameter, const QString &value);
	mutable Error error_code;
	mutable int error_line;
	QHash<QPoint, QVariant> *c_values;
	QList<QString> *c_sections;
	QList<QString> *c_parameters;
};

inline uint qHash(const QPoint &key);

#endif
