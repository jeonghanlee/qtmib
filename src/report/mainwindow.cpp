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
#include "bundle.h"

MainWindow::MainWindow(Bundle *bundle): bundle_(bundle) {
	createMenus();

	// result view
	resultView_ = new QTextEdit;
	resultView_->setReadOnly(true);

	QVBoxLayout *mLayout = new QVBoxLayout;
	mLayout->addWidget(resultView_);
	QWidget *mWidget = new QWidget;
	mWidget->setLayout(mLayout);
	setCentralWidget(mWidget);

	statusBar()->showMessage(tr("Processing..."), 5000);

	QTimer::singleShot(200, this, SLOT(runReport()));

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
	QAction *sysrAction = new QAction(tr("&System Report"), this);
	sysrAction->setStatusTip(tr("Generate a system report"));
	QAction *procrAction = new QAction(tr("Pro&cess Report"), this);
	procrAction->setStatusTip(tr("List all running processes"));
	QAction *softrAction = new QAction(tr("Soft&ware Report"), this);
	softrAction->setStatusTip(tr("List all installed software packages"));
	QAction *intfrAction = new QAction(tr("&Interface Report"), this);
	intfrAction->setStatusTip(tr("Generate an interface report"));
	QAction *routerAction = new QAction(tr("&Routing Table Report"), this);
	routerAction->setStatusTip(tr("Generate a routing table report"));

	connect(sysrAction, SIGNAL(triggered()), this, SLOT(sysr()));
	connect(procrAction, SIGNAL(triggered()), this, SLOT(procr()));
	connect(softrAction, SIGNAL(triggered()), this, SLOT(softr()));
	connect(intfrAction, SIGNAL(triggered()), this, SLOT(intfr()));
	connect(routerAction, SIGNAL(triggered()), this, SLOT(router()));


	// File menu
	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("Show the application's About box"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	QMenu* reportMenu = fileMenu->addMenu(tr("&Reports"));
	reportMenu->addAction(sysrAction);
	reportMenu->addAction(intfrAction);
	reportMenu->addAction(routerAction);
	reportMenu->addAction(procrAction);
	reportMenu->addAction(softrAction);
	fileMenu->addAction(aboutAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
}

void MainWindow::sysr() {
	Bundle *bundle = new SystemBundle(
					bundle_->getVersion(),
					bundle_->getCommunity(),
					bundle_->getPort(),
					bundle_->getIp());
		
	delete bundle_;
	bundle_ = bundle;
	runReport();
}

void MainWindow::procr() {
	Bundle *bundle = new ProcessBundle(
					bundle_->getVersion(),
					bundle_->getCommunity(),
					bundle_->getPort(),
					bundle_->getIp());
		
	delete bundle_;
	bundle_ = bundle;
	runReport();
}

void MainWindow::softr() {
	Bundle *bundle = new SoftwareBundle(
					bundle_->getVersion(),
					bundle_->getCommunity(),
					bundle_->getPort(),
					bundle_->getIp());
		
	delete bundle_;
	bundle_ = bundle;
	runReport();
}

void MainWindow::intfr() {
	Bundle *bundle = new InterfaceBundle(
					bundle_->getVersion(),
					bundle_->getCommunity(),
					bundle_->getPort(),
					bundle_->getIp());
		
	delete bundle_;
	bundle_ = bundle;
	runReport();
}

void MainWindow::router() {
	Bundle *bundle = new RouteBundle(
					bundle_->getVersion(),
					bundle_->getCommunity(),
					bundle_->getPort(),
					bundle_->getIp());
		
	delete bundle_;
	bundle_ = bundle;
	runReport();
}

void MainWindow::runReport() {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	bundle_->run(resultView_);
	statusBar()->showMessage(tr("Ready"), 5000);
	QApplication::restoreOverrideCursor();
}
