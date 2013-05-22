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
#include "pref_dialog.h"

PrefDialog::PrefDialog(): QDialog() {
	// protocol version
	QLabel *pLabel = new QLabel;
	pLabel->setText(tr("Protocol Version"));
	pBox = new QComboBox;
	pBox->addItem("v1");
	pBox->addItem("v2c");
	pBox->addItem("v3");
	pBox->setCurrentIndex(1);

	// community
	QLabel *cLabel = new QLabel;
	cLabel->setText(tr("Read Community"));
	cBox = new QComboBox;
	cBox->addItem("public");
	cBox->setEditable(true);
	
	// port number
	QLabel *portLabel = new QLabel;
	portLabel->setText(tr("UDP Port Number"));
	portBox = new QComboBox;
	portBox->addItem("161");
	portBox->setEditable(true);

	// timeout
	QLabel *timeoutLabel = new QLabel;
	timeoutLabel->setText(tr("Timeout (seconds)"));
	timeoutBox = new QComboBox;
	timeoutBox->addItem("1");
	timeoutBox->setEditable(true);

	// retries
	QLabel *retriesLabel = new QLabel;
	retriesLabel->setText(tr("Retries"));
	retriesBox = new QComboBox;
	retriesBox->addItem("5");
	retriesBox->setEditable(true);

	// OK
	QPushButton *okButton = new QPushButton(tr("OK"));
	connect(okButton, SIGNAL(clicked()), SLOT(accept()));
	
	QGridLayout *grid = new QGridLayout;
	grid->addItem(new QSpacerItem(30, 30), 0, 0);
	grid->addWidget(pLabel, 1, 1);
	grid->addWidget(pBox, 1, 3, 1, 2);
	grid->addWidget(cLabel, 2, 1);
	grid->addWidget(cBox, 2, 3, 1, 2);
	grid->addWidget(portLabel, 3, 1);
	grid->addWidget(portBox, 3, 3, 1, 2);
	grid->addWidget(timeoutLabel, 4, 1);
	grid->addWidget(timeoutBox, 4, 3, 1, 2);
	grid->addWidget(retriesLabel, 5, 1);
	grid->addWidget(retriesBox, 5, 3, 1, 2);

	grid->addItem(new QSpacerItem(30, 30), 6, 1);
	grid->addWidget(okButton, 7, 3);
	grid->setColumnMinimumWidth(2, 50);
	grid->setColumnMinimumWidth(4, 150);
	grid->setColumnMinimumWidth(5, 30);
	setLayout(grid);
	setWindowTitle(tr("Preferences"));
}

QString PrefDialog::getVersion() {
	return pBox->currentText();
}

QString PrefDialog::getCommunity() {
	return cBox->currentText();
}

QString PrefDialog::getPort() {
	return portBox->currentText();
}

QString PrefDialog::getTimeout() {
	return timeoutBox->currentText();
}

QString PrefDialog::getRetries() {
	return retriesBox->currentText();
}
