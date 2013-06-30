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

#include <QTextEdit>
#include "bundle.h"
#include "report.h"

void SystemBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrDeviceReport hrdevrep(version_, community_, port_, ip_);
	HrStorageReport hrstoragerep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>System Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1;
	rv += hrdevrep.get();
	rv += hrstoragerep.get();
	view->setText(rv);
}

void SoftwareBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrSoftwareReport hrsoftrep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Software Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1;
	rv += hrsoftrep.get();
	view->setText(rv);
}

void ProcessBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrProcessReport hrprocrep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Process Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1;
	rv += hrprocrep.get();
	view->setText(rv);
}

void InterfaceBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	InterfaceReport ifrep(version_, community_, port_, ip_);
	IfipReport ifip(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Interface Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	QString rv2 = ifip.get();
	if (rv2.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1 + rv2;
	rv += ifrep.get();
	view->setText(rv);
}

