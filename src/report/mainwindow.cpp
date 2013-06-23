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
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>

#include "mainwindow.h"
#include "../../qtmib_config.h"
#include "qtmib_report.h"
#include "exec_prog.h"
#include "report.h"

MainWindow::MainWindow() {
	createMenus();


	// resutl
	resultView_ = new QTextEdit;
	resultView_->setReadOnly(true);

	QVBoxLayout *mLayout = new QVBoxLayout;
	mLayout->addWidget(resultView_);
	QWidget *mWidget = new QWidget;
	mWidget->setLayout(mLayout);
	setCentralWidget(mWidget);
	
	runReport();
	
	statusBar()->showMessage(tr("Ready"), 2000);
}



void MainWindow::closeEvent(QCloseEvent *event) {
	event->accept();
}

void MainWindow::about() {
	QString msg = "<table cellpadding=\"10\"><tr><td><img src=\":/resources/qtmib-128.png\"></td>";
	msg += "<td>qtmib " + tr("version") + " " + PACKAGE_VERSION + "<br/><br/>";
	msg += tr(
		"qtmib is an easy-to-use SNMP MIB Browser based on QT4 library. It is build as "
		"a front-end for net-snmp tools, and it allows the user to query any SNMP-enabled "
		"device. It supports SNMPv1 and SNMPv2c. qtmib is released "
		"under GPL v2 license.<br/><br/>");
	msg += "Copyright (C) 2013 RCP100 Team (rcpteam@yahoo.com)<br/><br/>";
	msg += QString(PACKAGE_URL) + "</td></tr></table><br/><br/>";
	
	QMessageBox::about(this, tr("About"), msg);
}


void MainWindow::createMenus() {
	// File menu
	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("Show the application's About box"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(aboutAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
}

void MainWindow::runReport() {
	SystemReport sysrep("v2c", "public", "161", "127.0.0.1");
	HrDeviceReport hrdevrep("v2c", "public", "161", "127.0.0.1");
	HrStorageReport hrstoragerep("v2c", "public", "161", "127.0.0.1");

	QString rv = "<br/><center><h1>System Report</h1></center><br/><br/>\n";
	
	rv += sysrep.get();
	if (rv.isEmpty()) {
printf("Error: cannot connect to host\n");		
		return;
	}
	
	rv += hrdevrep.get();
	rv += hrstoragerep.get();
	resultView_->setText(rv);

}
