/***************************************************************************
 *   config.cpp                                                            *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Copyright 2007-2008 David Parunakian                                  *
 ***************************************************************************/
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QSet>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QPoint>
#include "config.h"

QrbConfig::QrbConfig(const QString &path) {
	c_values = new QHash<QPoint, QVariant>();
	c_sections = new QList<QString>();
	c_parameters = new QList<QString>();
	if (path != "")
		setFile(path);
}

QrbConfig::~QrbConfig() {
	delete c_values;
	delete c_sections;
	delete c_parameters;
}

QList<QString> QrbConfig::sections() const {
	return QList<QString>(*c_sections);
}

QList<QString> QrbConfig::parameters(const QString &section) const {
	if (! c_sections->contains(section)) {
		error_code = NoSuchSection;
		error_line = -1;
		return QList<QString>();
	}
	int sectionIndex = c_sections->indexOf(section);
	QList<int> indices;
	foreach (QPoint p, c_values->keys()) {
		if (p.x() == sectionIndex)
			indices.append(p.y());
	}
	qSort(indices);
	QList<QString> res;
	foreach (int i, indices) {
		res.append(c_parameters->at(i));
	}
	return res;
}


QVariant QrbConfig::value(const QString &section, const QString &parameter) const {
	int sectionIndex = c_sections->indexOf(section), parameterIndex = c_parameters->indexOf(parameter);

	if (sectionIndex == -1) {
		error_code = NoSuchSection;
		error_line = -1;
		return QVariant();
	}

	if (parameterIndex == -1 || !c_values->keys().contains(QPoint(sectionIndex, parameterIndex))) {
		error_code = NoSuchParameter;
		error_line = -1;
		return QVariant();
	}

	return c_values->value(QPoint(sectionIndex, parameterIndex));
}

void QrbConfig::setValue(const QString &section, const QString &parameter, const QString &value) {
	if (c_values->contains(QPoint(c_sections->indexOf(section), c_parameters->indexOf(parameter)))) {
		(*c_values)[QPoint(c_sections->indexOf(section), c_parameters->indexOf(parameter))] = value;
		return;
	}

	if (!c_sections->contains(section))
		c_sections->append(section);
	if (!c_parameters->contains(parameter))
		c_parameters->append(parameter);

	c_values->insert(QPoint(c_sections->indexOf(section), c_parameters->indexOf(parameter)), value);
}

void QrbConfig::error(QrbConfig::Error *code, int *line) {
	*code = error_code;
	*line = error_line;
}

void QrbConfig::setFile(const QString &path) {
	QFile file(path);
	QString c_section ="", c_param="", c_value="";
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		if (file.error() & (QFile::ReadError | QFile::FatalError | QFile::TimeOutError | QFile::OpenError | QFile::UnspecifiedError)) {
			error_code = NotFound;
			error_line = -1;
			return;
		}
		else if (file.error() & QFile::PermissionsError) {
			error_code = Forbidden;
			error_line = -1;
			return;
		}
	}
	
	error_code = Ok;
	error_line = -1;
	for (QString line = file.readLine(); ; line = QString::fromUtf8(file.readLine())) {
		error_line++;
		line = line.simplified();
	
		if (line.contains(";"))
			line = line.section(';', 0, 0).trimmed();

		if (line.isEmpty())
			continue;

		if (line.startsWith("[") && line.endsWith("]")) {
			c_section = line.mid(1, line.length() - 2).trimmed();
			if (c_section.contains(QRegExp("\\W")) || c_section == "") {
				error_code = BadSyntax;
				return;
			}
			else
				continue;
		}

		if (!line.contains('=')) {
			error_code = BadSyntax;
			return;
		}

		c_param = line.section('=', 0, 0).trimmed();
		c_value = line.section('=', 1, 1).trimmed();

		if (c_param.contains(QRegExp("\\W"))) {
			error_code = BadSyntax;
			return;
		}

		setValue(c_section, c_param, c_value);

		if (file.atEnd())
			break;
	}
	error_code = Ok;
	file.close();
	return;
}

inline uint qHash(const QPoint &key) {
	return qHash(QString(key.x()) + ";" + QString(key.y()));
}
