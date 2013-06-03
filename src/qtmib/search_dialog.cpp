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

#include <QtGui>
#include <QButtonGroup>
#include "search_dialog.h"

SearchDialog::SearchDialog(): QDialog() {
	// protocol version
	QLabel *sLabel = new QLabel;
	sLabel->setText(tr("Target"));
	sBox_ = new QComboBox;
	sBox_->setEditable(true);

	// OK
    	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
	QGridLayout *grid = new QGridLayout;
	grid->addItem(new QSpacerItem(30, 30), 0, 0);
	grid->addWidget(sLabel, 1, 1);
	grid->addWidget(sBox_, 1, 3, 1, 2);

	grid->addItem(new QSpacerItem(30, 30), 2, 1);
	grid->addWidget(buttonBox, 3, 3);
	grid->addItem(new QSpacerItem(30, 30), 3, 1);
	grid->setColumnMinimumWidth(2, 100);
	grid->setColumnMinimumWidth(4, 150);
	grid->setColumnMinimumWidth(5, 30);
	setLayout(grid);
	setWindowTitle(tr("Search"));
}

	
QString SearchDialog::getSearch() {
	return sBox_->currentText();
}

void SearchDialog::accept() {
	// if the string is not already stored, ...
	QString str = sBox_->currentText();
	int cnt = sBox_->count();
	bool found = false;
	for (int i = 0; i < cnt; i++) {
		if (sBox_->itemText(i) == str) {
			found = true;
			break;
		}
	}
	
	// ... store it
	if (!found)
		sBox_->addItem(sBox_->currentText());

	QDialog::accept();
}
