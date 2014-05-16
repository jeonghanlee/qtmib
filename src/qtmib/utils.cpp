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
#include "qtmib.h"
#include <stdio.h>
#include <QTreeView>

QStringList qt_line_diff(QString str1, QString str2) {
	QStringList list1 = str1.split( "\n", QString::SkipEmptyParts );
	QStringList list2 = str2.split( "\n", QString::SkipEmptyParts );
	int cnt1 = list1.count();
	int cnt2 = list2.count();
	
	if (cnt1 == 0 || cnt2 == 0)
		return list2;
	
	QStringList diff;
	int i;
	for (i = 0; i <cnt2; i++) {
		if (!list1.contains(list2[i]))
			diff += list2[i];
	}
	
	return diff;
}


QStandardItem *qtfind_child(QStandardItem *parent, QString name) {
	// find it in the tree
	int rows = parent->rowCount();
	int columns = parent->columnCount();

	int i;
	int j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			QStandardItem *child = parent->child(i, j);
			if (child) {
				if (child->text() == name) {
					return child;
				}
			}
		}
	}
	
	return 0;
}


QString qtfind_entry(QStandardItem *item, QString entry, QTreeView *treeView, bool partial) {
	int rows = item->rowCount();
	int columns = item->columnCount();
	QString current = item->text();
	int index1 = current.indexOf("(");
	int index2 = current.indexOf(")");
	QString right = current;
	right = right.mid(index1 + 1, index2 - index1 - 1);
	current.truncate(index1);
		
//printf("%s\n", current.toStdString().c_str());
	bool found = false;
	if (partial && current.startsWith(entry, Qt::CaseInsensitive))
		found = true;
	else if (current == entry)
		found = true;
	if (found) {
		treeView->setExpanded(item->index(), true);
		treeView->scrollTo(item->index(), QAbstractItemView::PositionAtTop);
		return QString(".") + item->text();
	}

	int i;
	int j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			QStandardItem *child = item->child(i, j);
			if (child) {
				QString rv = qtfind_entry(child, entry, treeView, partial);
				if (rv != "String not found") {
					treeView->setExpanded(item->index(), true);
					rv = QString(".") + item->text() + rv;
					return rv;
				}
			}
		}
	}

	return "String not found";
}
