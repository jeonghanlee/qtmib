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
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>

#include "qtmib_ip.h"
#include "mainwindow.h"
#include "dev_storage.h"
#include "../../qtmib_config.h"
#include "qtmib_discover.h"
#include "pref_dialog.h"
#include "clicked_label.h"
#define MAX_COLUMNS 6

MainWindow::MainWindow(): pref_(0) {
	createMenus();

	//*************************************************
	// preferences
	//*************************************************
	// network
	QLabel *networkLabel = new QLabel;
	networkLabel->setText(tr("Network"));
	network_ = new QComboBox;
	addInterfaces(network_);
	network_->setEditable(true);

	QPushButton *okButton = new QPushButton("Go");
	connect(okButton, SIGNAL(released()),this, SLOT(handleButton()));

	QGroupBox *group1Box = new QGroupBox(tr("Query"));
	QGridLayout *group1BoxLayout = new QGridLayout;
	group1BoxLayout->addWidget(networkLabel, 0, 0);
	group1BoxLayout->addWidget(network_, 0, 2);
	group1BoxLayout->addWidget(okButton, 0, 4);
	QWidget *w = new QWidget(this);
	group1BoxLayout->addWidget(w, 0, 6);
	group1BoxLayout->setColumnMinimumWidth(1, 10);
	group1BoxLayout->setColumnMinimumWidth(3, 10);
	group1BoxLayout->setColumnMinimumWidth(5, 10);
	group1BoxLayout->setColumnMinimumWidth(6, 1);
	group1BoxLayout->setColumnStretch(5, 2);
	group1BoxLayout->setColumnStretch(6, 10);
	group1Box->setLayout(group1BoxLayout);

	//*************************************************
	// result
	//*************************************************
	result_ = new QTableWidget(0, MAX_COLUMNS);
	QStringList header;
	header.append("IP Address");
	header.append("sysUpTime");
	header.append("sysContact");
	header.append("sysName");
	header.append("sysLocation");
	header.append("Response\nTime");
	result_->setHorizontalHeaderLabels(header);
	result_->verticalHeader()->setVisible(true);
//	result_->setReadOnly(true);

	//*************************************************
	// result actions
	//*************************************************
	ClickedLabel *updateButton = new ClickedLabel("<font color=\"blue\">Update</font>", this);
	updateButton->setText("<font color=\"blue\">Update |</font>");
	connect(updateButton, SIGNAL(clicked()), this, SLOT(handleUpdate()));
	
	ClickedLabel *clearButton = new ClickedLabel("<font color=\"blue\">Clear</font>", this);
	clearButton->setText("<font color=\"blue\">Clear</font>");
	connect(clearButton, SIGNAL(clicked()), this, SLOT(handleClear()));
	
	
	QGridLayout *labelActionLayout = new QGridLayout;
	labelActionLayout->addWidget(updateButton, 0, 1);
	labelActionLayout->addWidget(clearButton, 0, 2);
	QWidget *w2 = new QWidget(this);
	labelActionLayout->addWidget(w2, 0, 3);
	group1BoxLayout->setColumnMinimumWidth(3, 20);
	labelActionLayout->setColumnStretch(0, 20);
//	labelActionLayout->setColumnStretch(1, 2);



	//*************************************************
	// main layout
	//*************************************************
	QVBoxLayout *mLayout = new QVBoxLayout;
	mLayout->addWidget(group1Box);
	mLayout->addLayout(labelActionLayout);
	mLayout->addWidget(result_);
	QWidget *mWidget = new QWidget;
	mWidget->setLayout(mLayout);
	setCentralWidget(mWidget);
	
	statusBar()->showMessage(tr("Ready"), 2000);

	connect(&thread, SIGNAL(transactionDone(const QString &)),
		this, SLOT(transactionDone(const QString &)));
	connect(&thread, SIGNAL(displayResult(const QString &)),
		this, SLOT(displayResult(const QString &)));

	pref_ = new PrefDialog("public", "161", "1", "5");
	QTimer::singleShot(60000, this, SLOT(responseTime()));

}

void MainWindow::sendResponseTimeRequest(uint32_t ip) {
	DevStorage *dev = new DevStorage();
	dev->range_start_ = ip;
	dev->range_end_ = ip;
	dev->version_ = pref_->getVersion(); 
	dev->community_ = pref_->getCommunity();
	dev->port_ = pref_->getPort();
	thread.addTransactionResponse(dev);
	delete dev;
}

void MainWindow::triggerResponseTime() {
	int rows = result_->rowCount();
	for (int i = 0; i < rows; i++) {
		QTableWidgetItem *item = result_->item(i, 0);
		uint32_t  ip;
		if (atoip(item->text().toStdString().c_str(), &ip)) {
			QMessageBox::warning(this, tr("IP address"), "Invalid IP address");
			return;
		}
		result_->setItem(i, 5, new QTableWidgetItem(""));
		result_->removeCellWidget(i, 5);
		sendResponseTimeRequest(ip);
	}
}

void MainWindow::responseTime() {
	triggerResponseTime();
	QTimer::singleShot(60000, this, SLOT(responseTime()));
}

void MainWindow::preferences() {
	if (QDialog::Accepted == pref_->exec()) {
	}
}

void MainWindow::handleUpdate() {
	int rows = result_->rowCount();
	for (int i = 0; i < rows; i++) {
		QTableWidgetItem *item = result_->item(i, 0);
		uint32_t  ip;
		if (atoip(item->text().toStdString().c_str(), &ip)) {
			QMessageBox::warning(this, tr("IP address"), "Invalid IP address");
			return;
		}
		
		DevStorage *dev = new DevStorage();
		dev->range_start_ = ip;
		dev->range_end_ = ip;
		dev->version_ = pref_->getVersion(); 
		dev->community_ = pref_->getCommunity();
		dev->port_ = pref_->getPort();

		if (!thread.isRunning())	
			QMessageBox::warning(this, tr("Network Discovery"), QString("Cannot open network socket<br/><br/>\n"));
		else
			thread.addTransaction(dev);
		
		delete dev;
	}
	triggerResponseTime();
}

void MainWindow::handleClear() {
	int rows = result_->rowCount();
	if (rows == 0)
		return;
	
	 for (int i = rows - 1; i >= 0; i--) {
		for (int j = 0; j < MAX_COLUMNS; j++) {
			QTableWidgetItem *item = result_->item(i, j);
			result_->removeCellWidget(i, j);
			delete item;
		}
		// delete row
		result_->removeRow(i);
	 }
}

void MainWindow::addInterfaces(QComboBox *net) {
	bool added = false;
	struct ifaddrs *ifaddr;

	if (getifaddrs(&ifaddr) == -1) {
		net->addItem("192.168.1.0/24");
		return;
	}
		
	struct ifaddrs *ifa = ifaddr;
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr != NULL && ifa->ifa_netmask != NULL && ifa->ifa_name != NULL) {
			if (strcmp(ifa->ifa_name, "lo") == 0)
				continue;
				
			int family = ifa->ifa_addr->sa_family;
			if (family == AF_PACKET) {
				// no L3 tunnels
				if (ifa->ifa_flags & IFF_NOARP) {
					continue;
				}
			}
			else if (family != AF_INET /* || family == AF_INET6 */) {
				continue;
			}

			struct sockaddr_in *si = (struct sockaddr_in *) ifa->ifa_netmask;
			uint32_t mask = ntohl(si->sin_addr.s_addr);
			si = (struct sockaddr_in *) ifa->ifa_addr;
			uint32_t ip = ntohl(si->sin_addr.s_addr) & mask;
			char cidr[30];
			sprintf(cidr, "%d.%d.%d.%d/%d", RCP_PRINT_IP(ip), mask2bits(mask));
			net->addItem(QString(cidr));
			added = true;
//printf("name %s, %d.%d.%d.%d/%d\n", ifa->ifa_name, RCP_PRINT_IP(ip), mask2bits(mask));
		}
	}
	freeifaddrs(ifaddr);

	if (!added)
		net->addItem("192.168.1.0/24");
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
	msg += "Copyright (C) 2013-2014 RCP100 Team (rcpteam@yahoo.com)<br/><br/>";
	msg += QString(PACKAGE_URL) + "</td></tr></table><br/><br/>";
	
	QMessageBox::about(this, tr("About"), msg);
}


void MainWindow::createMenus() {
	// File menu
	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("Show the application's About box"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	prefAction = new QAction(tr("&Preferences"), this);
	prefAction->setStatusTip(tr("SNMP protocol preferences"));
	connect(prefAction, SIGNAL(triggered()), this, SLOT(preferences()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(prefAction);
	fileMenu->addAction(aboutAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
}

void MainWindow::handleButton() {
	// check network address
	QString qs = "A CIDR network address is expected.<br/><br/>";
	qs += "For example an address of 192.168.1.0/24 will include<br/>";
	qs += "all IP addresses from 192.168.1.1 to 192.168.1.254.<br/><br/>";
	uint32_t ip = 0;
	uint32_t mask = 0;
	if (atocidr(network_->currentText().toStdString().c_str(), &ip, &mask) ||
		ip == 0 || mask == 0) {
		
		QMessageBox::warning(this, tr("Network Address"), qs);
		return;
		
	}
	
	char cidr[30];
	sprintf(cidr, "%d.%d.%d.%d/%d", RCP_PRINT_IP(ip), mask2bits(mask));
	if (network_->currentText().toStdString().c_str() != QString(cidr)) {
		QMessageBox::warning(this, tr("Network Address"), qs);
		return;
	}

	// check port number
	int port = atoi(pref_->getPort().toStdString().c_str());
	qs = "A port number between 1 and 65535 is expected.<br/><br/>";
	if (port <= 0 || port > 65535) {
		QMessageBox::warning(this, tr("Port Number"), qs);
		return;
	}
	
	DevStorage *dev = new DevStorage();
	
	if (mask == 0xffffffff) {
		dev->range_start_ = ip;
		dev->range_end_ = ip;
	}
	else {
		dev->range_start_ = (ip & mask) + 1;
		dev->range_end_ = (ip | ~mask) - 1;
	}

	dev->version_ = pref_->getVersion(); 
	dev->community_ = pref_->getCommunity();
	dev->port_ = pref_->getPort();
	
	if (!thread.isRunning())	
		QMessageBox::warning(this, tr("Network Discovery"), QString("Cannot open network socket<br/><br/>\n"));
	else
		thread.addTransaction(dev);
	delete dev;
}


void MainWindow::transactionDone(const QString &msg) {
	statusBar()->showMessage(msg, 4000);
}

void MainWindow::displayResult(const QString &msg) {
	if (dbg)
		printf("message %s\n", msg.toStdString().c_str());
	QStringList lst = msg.split( "\t", QString::SkipEmptyParts);
	
	QString op;
	QString ip;
	QString sysContact;
	QString sysName;
	QString sysLocation;
	QString sysUpTime;
	int cnt = 0;
	foreach (QString word, lst) {
		switch (cnt) {
			case 0: // add, del, response
				op = word;
				ip = "";
				sysName = "";
				sysLocation = "";
				sysContact = "";
				break;
			case 1:
				ip = word;
				break;
			case 2: // sysUpTime or respTime
				sysUpTime = word;
				break;
			case 3:
				sysContact = word;
				break;
			case 4:
				sysName = word;
				break;
			case 5:
				sysLocation = word;
				break;
		}
		cnt++;
	}
	
	// find this ip address
	int rows = result_->rowCount();
	int found = -1;
	for (int i = 0; i < rows; i++) {
		QTableWidgetItem *item = result_->item(i, 0);
		if (ip == item->text()) {
			found = i;
			break;
		}
	}
	
	
	if (op == "add") {
		if (found != -1) {
			// delete existing item
			for (int i = 0; i < MAX_COLUMNS; i++) {
				QTableWidgetItem *item = result_->item(found, i);
				result_->removeCellWidget(found, i);
				delete item;
			}
		}
		else {
			// add new empty row
			int rows = result_->rowCount();
			rows++;
			result_->setRowCount(rows);
			found = rows - 1;
		}
	
		result_->setItem(found, 0, new QTableWidgetItem(ip));		
		result_->setItem(found, 1, new QTableWidgetItem(sysUpTime));		
		result_->setItem(found, 2, new QTableWidgetItem(sysContact));		
		result_->setItem(found, 3, new QTableWidgetItem(sysName));		
		result_->setItem(found, 4, new QTableWidgetItem(sysLocation));
		uint32_t myip;
		if (atoip(ip.toStdString().c_str(), &myip) == 0)
			sendResponseTimeRequest(myip);
	}
	if (op == "response") {
		if (found == -1)
			return;
		result_->setItem(found, 5, new QTableWidgetItem(sysUpTime));		
		if (sysUpTime == " timeout ")
			result_->item(found, 5)->setBackgroundColor(QColor(Qt::red));
	}
	else if (op == "del") {
		if (found == -1)
			return;
		
		// delete existing items
		for (int i = 0; i < MAX_COLUMNS; i++) {
			QTableWidgetItem *item = result_->item(found, i);
			result_->removeCellWidget(found, i);
			delete item;
		}

		// delete row
		result_->removeRow(found);
	}
}

