#include <QtGui>
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>

#include "qtmib_ip.h"
#include "mainwindow.h"
#include "dev_storage.h"
#include "../../qtmib_config.h"

MainWindow::MainWindow() {
	createMenus();

	//*************************************************
	// action1 layout
	//*************************************************
	// network
	QLabel *networkLabel = new QLabel;
	networkLabel->setText(tr("Network"));
	network_ = new QComboBox;
	addInterfaces(network_);
	network_->setEditable(true);

	// protocol version
	QLabel *pLabel = new QLabel;
	pLabel->setText(tr("Protocol"));
	pBox_ = new QComboBox;
	pBox_->addItem("v1");
	pBox_->addItem("v2c");
	pBox_->setCurrentIndex(1);
	pBox_->setEditable(false);

	QWidget *s1 = new QWidget;
	s1->setFixedWidth(30);
	
	QHBoxLayout *a1Layout = new QHBoxLayout;
	a1Layout->addWidget(networkLabel);
	a1Layout->addWidget(network_);
	a1Layout->addWidget(s1);
	a1Layout->addWidget(pLabel);
	a1Layout->addWidget(pBox_);
	QWidget *a1Widget = new QWidget;
	a1Widget->setLayout(a1Layout);

	//*************************************************
	// action2 layout
	//*************************************************
	// community
	QLabel *cLabel = new QLabel;
	cLabel->setText(tr("Community"));
	cBox_ = new QLineEdit;
	cBox_->setText("public");
	
	// port number
	QLabel *portLabel = new QLabel;
	portLabel->setText(tr("Port Number"));
	portBox_ = new QLineEdit;
	portBox_->setText("161");

	QPushButton *okButton = new QPushButton("Start");
	connect(okButton, SIGNAL(released()),this, SLOT(handleButton()));

	QHBoxLayout *a2Layout = new QHBoxLayout;
	a2Layout->addWidget(cLabel);
	a2Layout->addWidget(cBox_);
	a2Layout->addWidget(portLabel);
	a2Layout->addWidget(portBox_);
	a2Layout->addWidget(portLabel);
	a2Layout->addWidget(okButton);
	QWidget *a2Widget = new QWidget;
	a2Widget->setLayout(a2Layout);
	
	//*************************************************
	// result
	//*************************************************
	result_ = new QTableWidget(0, 4);
	QStringList header;
	header.append("IP Address");
	header.append("sysContact");
	header.append("sysName");
	header.append("sysLocation");
	result_->setHorizontalHeaderLabels(header);
	result_->verticalHeader()->setVisible(true);
//	result_->setReadOnly(true);

	//*************************************************
	// main layout
	//*************************************************
	QVBoxLayout *mLayout = new QVBoxLayout;
	mLayout->addWidget(a1Widget);
	mLayout->addWidget(a2Widget);
	mLayout->addWidget(result_);
	QWidget *mWidget = new QWidget;
	mWidget->setLayout(mLayout);
	setCentralWidget(mWidget);
	
	statusBar()->showMessage(tr("Ready"), 2000);

	connect(&thread, SIGNAL(transactionDone(const QString &)),
		this, SLOT(transactionDone(const QString &)));
	connect(&thread, SIGNAL(displayResult(const QString &)),
		this, SLOT(displayResult(const QString &)));

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
			uint32_t ip = ntohl(si->sin_addr.s_addr);
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
	int port = atoi(portBox_->text().toStdString().c_str());
	qs = "A port number between 1 and 65535 is expected.<br/><br/>";
	if (port <= 0 || port > 65535) {
		QMessageBox::warning(this, tr("Port Number"), qs);
		return;
	}
	sprintf(cidr, "%d", port);
	if (portBox_->text().toStdString().c_str() != QString(cidr)) {
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

	dev->version_ = pBox_->currentText();
	dev->community_ = cBox_->text();
	dev->port_ = portBox_->text();
	thread.addTransaction(dev);
}


void MainWindow::transactionDone(const QString &msg) {
	statusBar()->showMessage(msg, 2000);
}

void MainWindow::displayResult(const QString &msg) {
printf("message %s\n", msg.toStdString().c_str());
	QStringList lst = msg.split( "\t", QString::SkipEmptyParts);
	
	QString op;
	QString ip;
	QString sysContact;
	QString sysName;
	QString sysLocation;
	int cnt = 0;
	foreach (QString word, lst) {
		switch (cnt) {
			case 0:
				op = word;
				ip = "";
				sysName = "";
				sysLocation = "";
				sysContact = "";
				break;
			case 1:
				ip = word;
				break;
			case 2:
				sysContact = word;
				break;
			case 3:
				sysName = word;
				break;
			case 4:
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
			for (int i = 0; i < 4; i++) {
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
		result_->setItem(found, 1, new QTableWidgetItem(sysContact));		
		result_->setItem(found, 2, new QTableWidgetItem(sysName));		
		result_->setItem(found, 3, new QTableWidgetItem(sysLocation));		
	}
	else if (op == "del") {
		if (found == -1)
			return;
		
		// delete existing items
		for (int i = 0; i < 4; i++) {
			QTableWidgetItem *item = result_->item(found, i);
			result_->removeCellWidget(found, i);
			delete item;
		}

		// delete row
		result_->removeRow(found);
	}
}

