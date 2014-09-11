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

#include <QtGui>
#include <QButtonGroup>
#include "pref_dialog.h"
extern int dbg;

PrefDialog::PrefDialog(QString prefname, QString community, QString port, QString timeout, QString retries):
	QDialog(), prefname_(prefname), protocol_("v2c"), community_(community),
	port_(port), timeout_(timeout), retries_(retries) {
	
	write_file_storage();
	gui();
}

PrefDialog::PrefDialog(QString prefname): QDialog(),prefname_(prefname), protocol_("v2c"), community_("public"),
	port_("161"), timeout_("1"), retries_("5") {
	gui();
	
}

void PrefDialog::gui() {	
	// protocol version
	QLabel *pLabel = new QLabel;
	pLabel->setText(tr("Protocol Version"));
	pBox_ = new QComboBox;
	pBox_->addItem("v1");
	pBox_->addItem("v2c");
	pBox_->setCurrentIndex(1);
	pBox_->setEditable(false);

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
	
	read_file_storage();
	accept();
}


int PrefDialog::read_file_storage() {
	QString conf = QDir::homePath() + prefname_;
	const char *fname = conf.toStdString().c_str();
	if (dbg)
		printf("opening config file %s\n", conf.toStdString().c_str());
	FILE *fp = fopen(fname, "r");
	if (!fp)
		return 0;	// no config file found, leave the defaults in place
	else {
		char buf[1024];
		int line = 0;
		while (fgets(buf, sizeof(buf), fp)) {
			line ++;
			
			// skeep blanks, empty lines
			char *ptr = buf;
			while (*ptr == ' ' || *ptr == '\t')
				ptr++;
			if (*ptr == '\n' || *ptr == '\0')
				continue;
			
			// remove \n
			char *ptr2 = strchr(ptr, '\n');
			if (ptr2 == NULL) {
				if (dbg)
					printf("Error %d: missing \\n on line %d in %s\n", __LINE__, line, fname);
				continue;
			}
			*ptr2 = '\0';
			
			// no blanks check
			int errflag = 0;
			ptr2 = ptr;
			while (*ptr2 != '\0') {
				if (*ptr2 == ' ' || *ptr2 == '\t') {
					errflag = 1;
					break;
				}
				ptr2++;
			}
			if (errflag) {
				if (dbg)
					printf("Error %d: invalid line %d in %s\n", __LINE__, line, fname);
				continue;
			}
			
			// extract the two words
			ptr2 = ptr;
			while (*ptr2 != ':')
				ptr2++;
			if (*ptr2 != ':') {
				if (dbg)
					printf("Error %d: invalid line %d in %s\n", __LINE__, line, fname);
				continue;
			}
			*ptr2 = '\0';
			ptr2++;

			if (strcmp(ptr, "protocol") == 0) {
				if (strcmp(ptr2, "v1") == 0) {
					pBox_->setCurrentIndex(0);
					protocol_ = "v1";
				}
			}
			else if (strcmp(ptr, "community") == 0) {
				if (strcmp(ptr2, "public") != 0) {
					cBox_->addItem(ptr2);
					cBox_->setCurrentIndex(1);
					community_ = QString(ptr2);
				}
			}
			else if (strcmp(ptr, "port") == 0) {
				if (strcmp(ptr2, "161") != 0) {
					portBox_->addItem(ptr2);
					portBox_->setCurrentIndex(1);
					port_ = QString(ptr2);
				}
			}
			else if (strcmp(ptr, "timeout") == 0) {
				if (strcmp(ptr2, "1") != 0) {
					timeoutBox_->addItem(ptr2);
					timeoutBox_->setCurrentIndex(1);
					timeout_ = QString(ptr2);
				}
			}
			else if (strcmp(ptr, "retries") == 0) {
				if (strcmp(ptr2, "5") != 0) {
					retriesBox_->addItem(ptr2);
					retriesBox_->setCurrentIndex(1);
					retries_ = QString(ptr2);
				}
			}
			else {
				if (dbg)
					printf("Error %d: invalid line %d in %s\n", __LINE__, line, fname);
			}
		}		
		fclose(fp);	
	}
	return 0;
}

int PrefDialog::write_file_storage() {
	QString conf = QDir::homePath() + prefname_; //"/.config/qtmib/preferences";
	const char *fname = conf.toStdString().c_str();
	FILE *fp = fopen(fname, "w");
	
	if (dbg)
		printf("saving config file %s\n", conf.toStdString().c_str());
	
	if (!fp) {
		if (dbg)
			printf("Error: cannot open config file\n");
		return 1;
	}
	fprintf(fp, "protocol:%s\n", protocol_.toStdString().c_str());
	fprintf(fp, "community:%s\n", community_.toStdString().c_str());
	fprintf(fp, "port:%s\n", port_.toStdString().c_str());
	fprintf(fp, "timeout:%s\n", timeout_.toStdString().c_str());
	fprintf(fp, "retries:%s\n", retries_.toStdString().c_str());
	fclose(fp);
	
	return 0;
}


int PrefDialog::exec() {
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
	
	write_file_storage();
	
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
