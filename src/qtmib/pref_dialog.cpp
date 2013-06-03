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

PrefDialog::PrefDialog(): QDialog(), protocol_("v2c"), community_("public"),
	port_("161"), timeout_("1"), retries_("5") {
	// protocol version
	QLabel *pLabel = new QLabel;
	pLabel->setText(tr("Protocol Version"));
	pBox_ = new QComboBox;
	pBox_->addItem("v1");
	pBox_->addItem("v2c");
	pBox_->setCurrentIndex(1);

	// community
	QLabel *cLabel = new QLabel;
	cLabel->setText(tr("Read Community"));
	cBox_ = new QComboBox;
	cBox_->addItem("public");
	cBox_->setEditable(true);
	
	// port number
	QLabel *portLabel = new QLabel;
	portLabel->setText(tr("UDP Port Number"));
	portBox_ = new QComboBox;
	portBox_->addItem("161");
	portBox_->setEditable(true);

	// timeout
	QLabel *timeoutLabel = new QLabel;
	timeoutLabel->setText(tr("Timeout (seconds)"));
	timeoutBox_ = new QComboBox;
	timeoutBox_->addItem("1");
	timeoutBox_->setEditable(true);

	// retries
	QLabel *retriesLabel = new QLabel;
	retriesLabel->setText(tr("Retries"));
	retriesBox_ = new QComboBox;
	retriesBox_->addItem("5");
	retriesBox_->setEditable(true);

	// OK
    	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
	QGridLayout *grid = new QGridLayout;
	grid->addItem(new QSpacerItem(30, 30), 0, 0);
	grid->addWidget(pLabel, 1, 1);
	grid->addWidget(pBox_, 1, 3, 1, 2);
	grid->addWidget(cLabel, 2, 1);
	grid->addWidget(cBox_, 2, 3, 1, 2);
	grid->addWidget(portLabel, 3, 1);
	grid->addWidget(portBox_, 3, 3, 1, 2);
	grid->addWidget(timeoutLabel, 4, 1);
	grid->addWidget(timeoutBox_, 4, 3, 1, 2);
	grid->addWidget(retriesLabel, 5, 1);
	grid->addWidget(retriesBox_, 5, 3, 1, 2);

	grid->addItem(new QSpacerItem(30, 30), 6, 1);
	grid->addWidget(buttonBox, 7, 3);
	grid->addItem(new QSpacerItem(30, 30), 8, 1);
	grid->setColumnMinimumWidth(2, 50);
	grid->setColumnMinimumWidth(4, 150);
	grid->setColumnMinimumWidth(5, 30);
	setLayout(grid);
	setWindowTitle(tr("Preferences"));
}

int PrefDialog::exec() {
	// set defaults
	pBox_->setCurrentIndex(1);
	cBox_->setCurrentIndex(0);
	portBox_->setCurrentIndex(0);
	timeoutBox_->setCurrentIndex(0);
	retriesBox_->setCurrentIndex(0);

	return QDialog::exec();
}

void PrefDialog::store_combo_text(QComboBox *box) {
	// if the string is not already stored, ...
	QString str = box->currentText();
	int cnt = box->count();
	bool found = false;
	for (int i = 0; i < cnt; i++) {
		if (box->itemText(i) == str) {
			found = true;
			break;
		}
	}
	// ... store it
	if (!found)
		box->addItem(box->currentText());
}

void PrefDialog::accept() {
	// protocol
	protocol_ = pBox_->currentText();
	
	// community
	community_ = cBox_->currentText();
	store_combo_text(cBox_);
	
	// port
	port_ = portBox_->currentText();
	store_combo_text(portBox_);

	// timeout
	timeout_ = timeoutBox_->currentText();
	store_combo_text(timeoutBox_);

	// retries
	retries_ = retriesBox_->currentText();
	store_combo_text(retriesBox_);
	
	return QDialog::accept();
}

	
QString PrefDialog::getVersion() {
	return protocol_;
}

QString PrefDialog::getCommunity() {
	return community_;
}

QString PrefDialog::getPort() {
	return port_;
}

QString PrefDialog::getTimeout() {
	return timeout_;
}

QString PrefDialog::getRetries() {
	return retries_;
}
