#include "qtmib.h"
#include <stdio.h>

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

