/*
 * Copyright (C) 2013 RCP100 Team (rcpteam@yahoo.com)
 *
 * This file is part of qtmib project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef OID_TRANSLATOR_H
#define OID_TRANSLATOR_H
#include <QString>
#include <QStandardItem>

class OidTranslator {
public:
	OidTranslator(QStandardItem *top);
	QString translate(QString input);

private:
	// element print function
	void print(QStandardItem *item);
	// element match
	bool match(QStandardItem *item);
	// tree traversal
	bool travers(QStandardItem *item);
private:
	QStandardItem *top_;
	QString oid_;
	bool last_;
	QString retval_;	
};
#endif