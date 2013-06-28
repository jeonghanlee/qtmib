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
#include <QStatusBar>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include "mainwindow.h"
#include "qtmib.h"
#include "clicked_label.h"
#include "pref_dialog.h"
#include "search_dialog.h"
#include "oid_translator.h"
#include "exec_prog.h"
#include "../../qtmib_config.h"
#include "../../qtmib_prefix.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), pref_(0), search_(0) {
	checkEnvironment();
	createMenu();
	
	//*************************************************
	// OID
	//*************************************************
	mymodel_ = modelFromFile(":/resources/treemodel.txt");
	QLabel *mibsLabel = new QLabel;
	mibsLabel->setText(tr("MIBs"));
	treeView_ = new QTreeView;
	treeView_->setModel(mymodel_);
	treeView_->header()->hide();
	//    treeView->expandAll();
	connect(treeView_->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection &,
		const QItemSelection &)),
		this, SLOT(updateActions()));

	// oid
	oidView_ = new QTextEdit;
	oidView_->setEnabled(false);
	oidView_->setPlainText("Name:\nOID:\nMIB:\n");
	oidView_->setLineWrapMode(QTextEdit::NoWrap);

	ClickedLabel *searchButton = new ClickedLabel("<font color=\"blue\">Search</font>", this);
	searchButton->setText("<font color=\"blue\">Search</font>");
	connect(searchButton, SIGNAL(clicked()), this, SLOT(handleSearch()));

	QGridLayout *oidLayout = new QGridLayout;
	oidLayout->addWidget(mibsLabel, 0, 0);
	oidLayout->addWidget(searchButton, 0, 8);
	oidLayout->addWidget(treeView_, 1, 0, 1, 10);
	oidLayout->addWidget(oidView_, 2, 0, 1, 10);
	oidLayout->setRowStretch(1, 10);
	oidLayout->setRowStretch(2, 2);
	QWidget *oidWidget = new QWidget;
	oidWidget->setLayout(oidLayout);

	//*************************************************
	// result
	//*************************************************
	QLabel *resultLabel = new QLabel;
	resultLabel->setText(tr("Result"));
	result_ = new QTextEdit;
	result_->setReadOnly(true);

	// connection
	connectionView_ = new QTextEdit;
	connectionView_->setEnabled(false);
	connectionView_->setPlainText("SNMP version: v2c\nCommunity: public\n");
	connectionView_->setLineWrapMode(QTextEdit::NoWrap);
	QLabel *connectionLabel = new QLabel;
	connectionLabel->setText(tr("Protocol Preferences"));
	
	ClickedLabel *clearButton = new ClickedLabel("<font color=\"blue\">Clear</font>", this);
	clearButton->setText("<font color=\"blue\">Clear</font>");
	connect(clearButton, SIGNAL(clicked()), this, SLOT(handleClear()));

	ClickedLabel *translateButton = new ClickedLabel("<font color=\"blue\">Translate</font>", this);
	translateButton->setText("<font color=\"blue\">Translate |</font>");
	connect(translateButton, SIGNAL(clicked()), this, SLOT(handleTranslate()));

	QGridLayout *resLayout = new QGridLayout;
	resLayout->addWidget(resultLabel, 0, 0);
	resLayout->addWidget(translateButton, 0, 8);
	resLayout->addWidget(clearButton, 0, 9);
	resLayout->addWidget(result_, 1, 0, 1, 10);
	resLayout->addWidget(connectionView_, 2, 0, 1, 10);
	resLayout->setRowStretch(1, 10);
	resLayout->setRowStretch(2, 2);
	QWidget *resWidget = new QWidget;
	resWidget->setLayout(resLayout);

	//*************************************************
	// OID/result splitter
	//*************************************************
	QSplitter *splitter = new QSplitter();
	splitter->setOrientation(Qt::Horizontal);
	splitter->addWidget(oidWidget);
	splitter->addWidget(resWidget);
	splitter->setStretchFactor(0, 2);
	splitter->setStretchFactor(1, 5);

	//*************************************************
	// action bar
	//*************************************************
	QLabel *actionIpLabel = new QLabel;
	actionIpLabel->setText(tr("IP Address"));
	actionIp_ = new QLineEdit;
	actionIp_->setText("127.0.0.1");
	QLabel *actionOidLabel = new QLabel;
	actionOidLabel->setText(tr("OID"));
	actionOid_ = new QLineEdit;
	actionOid_->setText(".1.3.6.1");
	action_ = new QComboBox;
	action_->addItem(tr("Get"));
	action_->addItem(tr("Get Next"));
	action_->addItem(tr("Get Bulk"));
	action_->addItem(tr("Walk"));
	action_->setCurrentIndex(0);
	QPushButton *actionButton = new QPushButton(tr("Go"));
	connect(actionButton, SIGNAL(released()), this, SLOT(handleAction()));

	QGroupBox *group1Box = new QGroupBox(tr("Query"));
	QGridLayout *group1BoxLayout = new QGridLayout;
	group1BoxLayout->addWidget(actionIpLabel, 0, 0);
	group1BoxLayout->addWidget(actionIp_, 0, 1);
	group1BoxLayout->addWidget(actionOidLabel, 0, 2);
	group1BoxLayout->addWidget(actionOid_, 0, 3);
	group1BoxLayout->addWidget(action_, 0, 4);
	group1BoxLayout->addWidget(actionButton, 0, 5);
	group1Box->setLayout(group1BoxLayout);
	group1BoxLayout->setColumnStretch(1, 7);
	group1BoxLayout->setColumnStretch(3, 10);

	// grid
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(group1Box, 0, 0, 1, 2);
	layout->addWidget(splitter);
	layout->setRowStretch(0, 1);
	layout->setRowStretch(1, 50);

	QWidget *centralWidget = new QWidget;
	centralWidget->setLayout(layout);
	setCentralWidget(centralWidget);

	// set title
	setWindowTitle(tr("qtmib"));

	// add the status bar
	statusBar();
	statusBar()->showMessage("Ready");
}

void MainWindow::checkEnvironment() {
	char *rv = exec_prog("which snmpget");
	QString ref = "";
	if (rv == 0 || strstr(rv, "snmpget") == 0) {
		QMessageBox::warning(this, tr("qtmib Environment"),
			tr("<br/><b>net-snmp</b> tools not found.<br/><br/>"
			"Please install net-snmp package from<br/>http://www.net-snmp.org/<br/<br/>"));
	}
}

QAbstractItemModel *MainWindow::modelFromFile(const QString& fileName) {
	//*************************
	// load global mibs
	//*************************
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) {
		QMessageBox msgBox;
		msgBox.setText("Cannot read the MIB tree.");
		msgBox.exec();
		return new QStringListModel(this);
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QStringList words;

	QStandardItemModel *model = new QStandardItemModel(this);
	QVector<QStandardItem *> parents(10);
	parents[0] = model->invisibleRootItem();

	topitem_ = 0;

	while (!file.atEnd()) {
		QString line = file.readLine();
		QString trimmedLine = line.trimmed();
		//printf("line#%s#\n", line.toStdString().c_str());
		//printf("trimedLine#%s#\n", trimmedLine.toStdString().c_str());
		if (line.isEmpty() || trimmedLine.isEmpty())
			continue;

		QRegExp re("^\\s+");
		int nonws = re.indexIn(line);
		int level = 0;
		if (nonws == -1) {
			level = 0;
		}
		else {
			if (line.startsWith("\t")) {
				level = re.cap(0).length();
			}
			else {
				level = re.cap(0).length()/4;
			}
		}

		if (level+1 >= parents.size())
			parents.resize(parents.size()*2);

		QStandardItem *item = new QStandardItem;
		if (!topitem_)
			topitem_ = item;
		item->setEditable(false);
		item->setText(trimmedLine);
		parents[level]->appendRow(item);
		parents[level+1] = item;
	}


	//*************************
	// load user mibs
	//*************************
	QString mibsdir = QDir::homePath();
	mibsdir += "/.config/qtmib/mibs";
	QDir dir(mibsdir);
	QStringList filelist = dir.entryList(QDir::Files | QDir::NoDot | QDir::NoDotDot);
	if (filelist.count() != 0) {
		QString cmd = QString("snmptranslate -M ") + "~/.config/qtmib/mibs/:" + QTMIB_PREFIX + "/share/qtmib -Tl";
		QString usr = "";
		char *rv = exec_prog(cmd.toStdString().c_str());
		if (rv) {
			usr += rv;
			free(rv);
		}
	
		// update tree
		QStringList diff = usr.split( "\n", QString::SkipEmptyParts);
		foreach (QString line, diff) {
//printf("processing %s\n", line.toStdString().c_str());		
			// split the line
			QStringList oidlist = line.split( ".", QString::SkipEmptyParts );
			int cnt = oidlist.count();
			int level = 1;
			QStandardItem *parent = 0;
	
			for (int i = 4; i < cnt; i++, level++) {
				if (level+1 >= parents.size())
					parents.resize(parents.size()*2);
				if (!parent)
					parent = parents[level];
					
//printf("level %d, parent %s\n", level, parent->text().toStdString().c_str());
//printf("oidlist[i] %s\n", oidlist[i].toStdString().c_str());
				QStandardItem *child = qtfind_child(parent, oidlist[i]);
				if (child)
					parent = child;
				else {
					if (dbg)
						printf("adding %s\n", line.toStdString().c_str());
					QStandardItem *item = new QStandardItem;
					item->setEditable(false);
					item->setText(oidlist[i]);
					parent->appendRow(item);
				}
			}	
		}
	}
		
	QApplication::restoreOverrideCursor();

	return model;
}


void MainWindow::createMenu() {
	QAction *sysrAction = new QAction(tr("&System Report"), this);
	sysrAction->setStatusTip(tr("Extract system information"));
	QAction *procrAction = new QAction(tr("Pro&cess Report"), this);
	procrAction->setStatusTip(tr("Extract running process information"));
	QAction *softrAction = new QAction(tr("Soft&ware Report"), this);
	softrAction->setStatusTip(tr("Extract installed software information"));

	QAction *intfrAction = new QAction(tr("&Interface Report"), this);
	intfrAction->setStatusTip(tr("Extract interface information"));
	QAction *tcpiprAction = new QAction(tr("&TCP/IP Report"), this);
	tcpiprAction->setStatusTip(tr("Extract TCP/IP information"));
	connect(sysrAction, SIGNAL(triggered()), this, SLOT(sysr()));
	connect(procrAction, SIGNAL(triggered()), this, SLOT(procr()));
	connect(softrAction, SIGNAL(triggered()), this, SLOT(softr()));
	connect(intfrAction, SIGNAL(triggered()), this, SLOT(intfr()));
	connect(tcpiprAction, SIGNAL(triggered()), this, SLOT(tcpipr()));

	QAction *discAction = new QAction(tr("&Network Discovery"), this);
	discAction->setStatusTip(tr("Run network discovery"));
	QAction *prefAction = new QAction(tr("&Preferences"), this);
	prefAction->setStatusTip(tr("SNMP protocol preferences"));
	QAction *aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("Show application's About box"));
	QAction *exitAction = new QAction(tr("E&xit"), this);
	exitAction->setStatusTip(tr("Exit the application"));

	connect(discAction, SIGNAL(triggered()), this, SLOT(discovery()));
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	connect(prefAction, SIGNAL(triggered()), this, SLOT(preferences()));

	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	QMenu* reportMenu = fileMenu->addMenu(tr("&Reports"));
	reportMenu->addAction(sysrAction);
	reportMenu->addAction(intfrAction);
	reportMenu->addAction(tcpiprAction);
	reportMenu->addAction(procrAction);
	reportMenu->addAction(softrAction);
	fileMenu->addAction(discAction);
	fileMenu->addAction(prefAction);
	fileMenu->addSeparator();
	fileMenu->addAction(aboutAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
}

void MainWindow::sysr() {
	int rv = system("qtmib-report --system&");
	(void) rv;
}

void MainWindow::procr() {
	int rv = system("qtmib-report --process&");
	(void) rv;
}

void MainWindow::softr() {
	int rv = system("qtmib-report --software&");
	(void) rv;
}

void MainWindow::intfr() {
	int rv = system("qtmib-report --interface&");
	(void) rv;
}

void MainWindow::tcpipr() {
	int rv = system("qtmib-report --tcpip&");
	(void) rv;
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

void MainWindow::discovery() {
	int rv = system("qtmib-discover &");
	if (rv == -1)
		QMessageBox::warning(this, tr("qtmib Environment"),
			tr("<br/><b>qtmib-discover</b> module not found.<br/><br/>"));
}

void MainWindow::preferences() {
	if (!pref_)
		pref_ = new PrefDialog();
		
	if (QDialog::Accepted == pref_->exec()) {
		QString str = "SNMP version: " + pref_->getVersion() + "\n";
		if (pref_->getCommunity() == "public")
			str += "Community: public\n";
		connectionView_->setPlainText(str);
	}

}

void MainWindow::updateActions() {
	bool hasCurrent = treeView_->selectionModel()->currentIndex().isValid();
	if (hasCurrent) {
		QModelIndex mindex = treeView_->selectionModel()->currentIndex();
		const QAbstractItemModel *mod = mindex.model();
		bool has_children = false;
		if (mindex.child(0, 0).isValid())
			has_children = true;

		// build name, oid
		QString origname = mod->data(mindex).toString();

		QString name = mod->data(mindex).toString();
		name.truncate(name.indexOf("("));

		int index1 = origname.indexOf("(");
		int index2 = origname.indexOf(")");
		QString oid = origname.mid(index1 + 1, index2 - index1 - 1);

		while (mindex.parent().isValid()) {
			mindex = mindex.parent();
			QString current = mod->data(mindex).toString();
			index1 = current.indexOf("(");
			index2 = current.indexOf(")");
			QString left = current;
			QString right = current;
			left.truncate(index1);
			right = right.mid(index1 + 1, index2 - index1 - 1);

			name = left + "." + name;
			oid = right + "." + oid;
			//printf("%s\n", oid.toStdString().c_str());
		}
		oid = "." + oid;
		statusBar()->showMessage(name);

		//set data in oidView
		origname.truncate(origname.indexOf("("));
		QString txt = "Name: " + origname + "\nOID: " + oid + "\n";
		oidView_->setPlainText(txt);
		if (!has_children)
			oid += ".0";
		actionOid_->setText(oid);

	}
	else
		statusBar()->showMessage("updated");
}

void MainWindow::handleAction() {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QString act = action_->currentText();

	// action
	if (act == "Get")
		act = "snmpget";
	else if (act == "Get Next")
		act = "snmpgetnext";
	else if (act == "Get Next")
		act = "snmpgetnext";
	else if (act == "Get Bulk")
		act = "snmpbulkget";
	else if (act == "Walk")
		act = "snmpwalk";
	else
		return;
	
	// version
	QString version = "v2c";
	if (pref_)
		version = pref_->getVersion();
	
	// community
	QString community = "public";
	if (pref_)
		community = pref_->getCommunity();

	// timeout
	QString timeout = "1";
	if (pref_)
		timeout = pref_->getTimeout();
	
	// retries
	QString retries = "5";
	if (pref_)
		retries = pref_->getRetries();
	
	// port
	QString port = "161";
	if (pref_)
		port = pref_->getPort();
	
	
	
	// build command string	
	QString cmd = act + " -m \"\" ";
	cmd += "-" + version + " ";
	cmd += "-c " + community + " ";
	cmd += "-t " + timeout + " ";
	cmd += "-r " + retries + " ";
	cmd += actionIp_->text() + ":" + port + " ";
	cmd += actionOid_->text() + " 2>&1";
//printf("\n%s\n", cmd.toStdString().c_str());

	// execute command
	char *rv = exec_prog(cmd.toStdString().c_str());
	if (rv) {
		result_->append(rv);
		
		// prepare next OID for getnext
		if (act == "snmpgetnext" || act == "snmpbulkget") {
			QString input = rv;
			QStringList lines = input.split( "\n", QString::SkipEmptyParts );
			foreach (QString line, lines) {
				if (line.startsWith("iso.3.6.1.")) {
					QString oid = line.mid(3);
					oid = ".1" + oid;
					int index = oid.indexOf(" = ");
					if (index != -1) {
						oid.truncate(index);
//printf("%s\n", oid.toStdString().c_str());
						actionOid_->setText(oid);
					}
				}
			}					
		}
		
		free(rv);
	}
	
	QApplication::restoreOverrideCursor();
}

void MainWindow::handleSearch() {
	if (!search_)
		search_ = new SearchDialog();
		
	if (QDialog::Accepted == search_->exec()) {
		QString entry = search_->getSearch();
		QString oid = qtfind_entry(topitem_, entry, treeView_);
		statusBar()->showMessage(oid);
			
	}
}

void MainWindow::handleClear() {
	result_->setText(QString(""));
}

void MainWindow::handleTranslate() {
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	
	OidTranslator oidt(topitem_);

	QString input = result_->toPlainText();
	QString output = "";
	
	QStringList lines = input.split( "\n", QString::SkipEmptyParts );
	foreach (QString line, lines) {
//printf("%s\n", line.toStdString().c_str());
		if (line.startsWith("iso.3.6.1.")) {
			int index = line.indexOf(" = ");
			if (index == -1) {
				output += line + "\n";
			}
			else {
				QString right = line.mid(index);
				line.truncate(index);
				QString left = line.mid(10);
//printf("#%s#%s#\n", left.toStdString().c_str(), right.toStdString().c_str());
				QString rv = oidt.translate(left);
				output += rv + right + "\n";
				
			}
		}
		else
			output += line + "\n";
	}
	result_->setText(output);	
	QApplication::restoreOverrideCursor();
}

