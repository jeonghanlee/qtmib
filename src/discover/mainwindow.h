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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "transactionthread.h"

class QAction;
class QLabel;
class QMenu;
class QTableWidget;
class QLineEdit;
class QComboBox;

class MainWindow : public QMainWindow
{
Q_OBJECT

	public:
	MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void about();
	void transactionDone(const QString &message);
	void displayResult(const QString &message);
	void handleButton();

private:
	void createMenus();
	void addTransaction(QString type);
	void addInterfaces(QComboBox *net);

	TransactionThread thread;

	QMenu *fileMenu;
	QTableWidget *result_;
	QComboBox *network_;
	QComboBox *pBox_;
	QLineEdit *cBox_;
	QLineEdit *portBox_;
	
	QAction *exitAction;
	QAction *aboutAction;
};
#endif
