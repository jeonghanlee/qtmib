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
class Bundle;
class QAction;
class QLabel;
class QMenu;
class QTableWidget;
class QLineEdit;
class QComboBox;
class QTextEdit;

class MainWindow : public QMainWindow
{
Q_OBJECT

	public:
	MainWindow(Bundle *bundle);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void about();
	void runReport();
	void sysr();
	void procr();
	void softr();
	void intfr();
	void router();
	void connection();
	void handleAction();

private:
	void createMenus();
	
	QMenu *fileMenu;
	QAction *exitAction;
	QAction *aboutAction;
	QComboBox *report_;
	
	QTextEdit *resultView_;
	Bundle *bundle_;
};
#endif
