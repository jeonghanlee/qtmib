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

#ifndef PREF_DIALOG_H
#define PREF_DIALOG_H

#include <QWidget>
#include <QDialog>
#include <QComboBox>

class QCheckBox;
class QLabel;
class QErrorMessage;

class PrefDialog: public QDialog {
Q_OBJECT

public:
	PrefDialog(QString prefname);
	PrefDialog(QString prefname, QString community, QString port, QString timeout, QString retries);
	QString getVersion();
	QString getCommunity();
	QString getPort();
	QString getTimeout();
	QString getRetries();
	int write_file_storage();

public slots:
	void accept();
	int exec();

private:
	void gui();
	void store_combo_text(QComboBox *box);	
	int read_file_storage();
	
private:
	QComboBox *pBox_;	// protocol version
	QComboBox *cBox_;	// community
	QComboBox *portBox_;	// port number
	QComboBox *timeoutBox_;	// timeout
	QComboBox *retriesBox_;	// retries

	QString protocol_;
	QString community_;
	QString port_;
	QString timeout_;
	QString retries_;
	QString prefname_;
};

#endif