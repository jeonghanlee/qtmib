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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdint.h>
#include <QMainWindow>

#include "transactionthread.h"

class QAction;
class QLabel;
class QMenu;
class QTableWidget;
class QLineEdit;
class QComboBox;
class PrefDialog;

class MainWindow : public QMainWindow
{
Q_OBJECT

	public:
	MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void about();
	void preferences();
	void transactionDone(const QString &message);
	void displayResult(const QString &message);
	void handleButton();
	void handleClear();
	void handleUpdate();
	void responseTime();

private:
	void createMenus();
	void addTransaction(QString type);
	void addInterfaces(QComboBox *net);
	void triggerResponseTime();
	void sendResponseTimeRequest(uint32_t ip);
	
	TransactionThread thread;

	QMenu *fileMenu;
	QTableWidget *result_;
	QComboBox *network_;
	PrefDialog *pref_;

	QAction *prefAction;
	QAction *exitAction;
	QAction *aboutAction;
};
#endif
