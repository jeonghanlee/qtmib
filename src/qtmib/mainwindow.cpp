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
#include "../../qtmib_config.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
	pref = 0;
	createMenu();

	//*************************************************
	// OID
	//*************************************************
	mymodel = modelFromFile(":/resources/treemodel.txt");
	QLabel *mibsLabel = new QLabel;
	mibsLabel->setText(tr("MIBs"));
	treeView = new QTreeView;
	treeView->setModel(mymodel);
	treeView->header()->hide();
	//    treeView->expandAll();
	connect(treeView->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection &,
		const QItemSelection &)),
		this, SLOT(updateActions()));

	// oid
	oidView = new QTextEdit;
	oidView->setEnabled(false);
	oidView->setPlainText("Name:\nOID:\nMIB:\n");
	oidView->setLineWrapMode(QTextEdit::NoWrap);

	QGridLayout *oidLayout = new QGridLayout;
	oidLayout->addWidget(mibsLabel, 0, 0);
	oidLayout->addWidget(treeView, 1, 0);
	oidLayout->addWidget(oidView, 2, 0);
	oidLayout->setRowStretch(1, 10);
	oidLayout->setRowStretch(2, 2);
	QWidget *oidWidget = new QWidget;
	oidWidget->setLayout(oidLayout);

	//*************************************************
	// result
	//*************************************************
	QLabel *resultLabel = new QLabel;
	resultLabel->setText(tr("Result"));
	result = new QTextEdit;
	result->setReadOnly(true);

	// connection
	connectionView = new QTextEdit;
	connectionView->setEnabled(false);
	connectionView->setPlainText("SNMP version: v2c\nCommunity: public\n");
	connectionView->setLineWrapMode(QTextEdit::NoWrap);
	QLabel *connectionLabel = new QLabel;
	connectionLabel->setText(tr("Protocol Preferences"));
	
	ClickedLabel *clearButton = new ClickedLabel("<font color=\"blue\">Clear</font>", this);
	clearButton->setText("<font color=\"blue\">Clear</font>");
	connect(clearButton, SIGNAL(clicked()), this, SLOT(handleClear()));

	QGridLayout *resLayout = new QGridLayout;
	resLayout->addWidget(resultLabel, 0, 0);
	resLayout->addWidget(clearButton, 0, 9);
	resLayout->addWidget(result, 1, 0, 1, 10);
	resLayout->addWidget(connectionView, 2, 0, 1, 10);
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
	actionIp = new QLineEdit;
	actionIp->setText("127.0.0.1");
	QLabel *actionOidLabel = new QLabel;
	actionOidLabel->setText(tr("OID"));
	actionOid = new QLineEdit;
	actionOid->setText(".1.3.6.1");
	action = new QComboBox;
	action->addItem(tr("Get"));
	action->addItem(tr("Get Next"));
	action->addItem(tr("Get Bulk"));
	action->addItem(tr("Walk"));
	action->setCurrentIndex(0);
	QPushButton *actionButton = new QPushButton(tr("Go"));
	connect(actionButton, SIGNAL(released()), this, SLOT(handleAction()));

	QGroupBox *group1Box = new QGroupBox(tr("Query"));
	QGridLayout *group1BoxLayout = new QGridLayout;
	group1BoxLayout->addWidget(actionIpLabel, 0, 0);
	group1BoxLayout->addWidget(actionIp, 0, 1);
	group1BoxLayout->addWidget(actionOidLabel, 0, 2);
	group1BoxLayout->addWidget(actionOid, 0, 3);
	group1BoxLayout->addWidget(action, 0, 4);
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


QAbstractItemModel *MainWindow::modelFromFile(const QString& fileName) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) {
		QMessageBox msgBox;
		msgBox.setText("Cannot read the MIB tree.");
		msgBox.exec();
		return new QStringListModel(this);
	}

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
	QStringList words;

	QStandardItemModel *model = new QStandardItemModel(this);
	QVector<QStandardItem *> parents(10);
	parents[0] = model->invisibleRootItem();

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
		item->setEditable(false);
		item->setText(trimmedLine);
		parents[level]->appendRow(item);
		parents[level+1] = item;
	}

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	return model;
}


void MainWindow::createMenu() {
	QAction *prefAction = new QAction(tr("Preferences"), this);
	QAction *aboutAction = new QAction(tr("About"), this);
	QAction *exitAction = new QAction(tr("Exit"), this);

	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	connect(prefAction, SIGNAL(triggered()), this, SLOT(preferences()));

	QMenu* fileMenu = menuBar()->addMenu(tr("File"));
	fileMenu->addAction(prefAction);
	fileMenu->addAction(aboutAction);
	fileMenu->addAction(exitAction);
}


void MainWindow::about() {
	QString msg = "qtmib " + tr("version") + " " + PACKAGE_VERSION + "\n\n";
	msg += tr(
		"qtmib is an easy-to-use SNMP MIB Browser based on QT4 library. It is build as "
		"a front-end for net-snmp tools, and it allows the user to query any SNMP "
		"enabled device. It supports SNMPv1, SNMPv2c and SNMPv3. qtmib is released "
		"under GPL v2 license.\n\n");
	msg += "Copyright (C) 2013 RCP100 Team (rcpteam@yahoo.com)\n";
	msg += QString(PACKAGE_URL) + "\n";
	
	QMessageBox::about(this, tr("About"), msg);
}

void MainWindow::preferences() {
	if (!pref)
		pref = new PrefDialog();
		
	pref->exec();

	QString str = "SNMP version: " + pref->getVersion() + "\n";
	if (pref->getCommunity() == "public")
		str += "Community: public\n";
		
	connectionView->setPlainText(str);

}

void MainWindow::updateActions() {
	bool hasCurrent = treeView->selectionModel()->currentIndex().isValid();
	if (hasCurrent) {
		QModelIndex mindex = treeView->selectionModel()->currentIndex();
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
		oidView->setPlainText(txt);
		if (!has_children)
			oid += ".0";
		actionOid->setText(oid);

	}
	else
		statusBar()->showMessage("updated");
}

void MainWindow::handleAction() {
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
	QString act = action->currentText();

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
	if (pref)
		version = pref->getVersion();
	
	// community
	QString community = "public";
	if (pref)
		community = pref->getCommunity();
	
	// build command string	
	QString cmd = act + " -m \"\" ";
	cmd += "-" + version + " ";
	cmd += "-c " + community + " ";
	cmd += actionIp->text() + " ";
	cmd += actionOid->text() + " 2>&1";
printf("\n%s\n", cmd.toStdString().c_str());

	// execute command
	char *rv = exec_prog(cmd.toStdString().c_str());
	if (rv) {
		QString qrv = rv;
		qrv.replace(QString("iso.3.6.1.2.1"), QString("mib-2"));
		qrv.replace(QString("iso.3.6.1.4.1"), QString("enterprises"));
		qrv.replace(QString("iso.3.6.1.6"), QString("snmpV2"));
		result->append(qrv);
		free(rv);
	}
	
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif
}

void MainWindow::handleClear() {
	result->setText(QString(""));
}
