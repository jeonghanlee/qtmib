/*
 * Copyright (C) 2013-2014 RCP100 Team (rcpteam@yahoo.com)
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

#include <stdio.h>
#include <stdlib.h>
#include "oid_translator.h"

void OidTranslator::print(QStandardItem *item) {
	QString tmp = item->text();
	if (tmp == "mib-2(1)") {
		retval_ = "mib-2";
		return;
	}
	else if (tmp == "enterprises(1)") {
		retval_ = "enterprises";
		return;
	}
	else if (tmp == "snmpV2(6)") {
		retval_ = "snmpV2";
		return;
	}
	
	int index = tmp.indexOf("(");
	if (index != -1) {
		tmp.truncate(index);
		retval_ += "." + tmp;
	}
}

bool OidTranslator::match(QStandardItem *item) {
	last_ = false;
	QString tmpoid = oid_;
	int index = tmpoid.indexOf(".");
	if (index != -1)
		tmpoid.truncate(index);

	QString current = item->text();
	int index1 = current.indexOf("(");
	int index2 = current.indexOf(")");
	QString right = current;
	right = right.mid(index1 + 1, index2 - index1 - 1);

//printf("right %s\n", right.toStdString().c_str());
	
	if (tmpoid == right) {
		if (index == -1)
			last_ = true;
		else
			oid_ = oid_.mid(index + 1);
		return true;
	}
	
	return false;
}

bool OidTranslator::travers(QStandardItem *item) {

	int rows = item->rowCount();
	int columns = item->columnCount();

	int i;
	int j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			QStandardItem *child = item->child(i, j);
			if (child) {
				if (match(child)) {
					print(item);
					if (last_) {
						print(child);
						return true;
					}
					return travers(child);
				}
				
			}
		}
	}
	print(item);
	return false;
}

OidTranslator::OidTranslator(QStandardItem *top): top_(top) {}

QString OidTranslator::translate(QString input) {
	oid_ = input;
	retval_ = "";
	travers(top_);
	retval_ += "." + oid_;
	return retval_;
}
