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

void Bundle::build(QTextEdit *view) {
	QString rv = run();
	view->setText(rv);
}

QString SystemBundle::run() {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrDeviceReport hrdevrep(version_, community_, port_, ip_);
	HrStorageReport hrstoragerep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>System Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		return QString("Error: cannot connect to host\n");
	}
	
	rv += rv1;
	rv += hrdevrep.get();
	rv += hrstoragerep.get();
	return rv;
}

QString SoftwareBundle::run() {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrSoftwareReport hrsoftrep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Software Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		return QString("Error: cannot connect to host\n");		
	}
	
	rv += rv1;
	rv += hrsoftrep.get();
	return rv;
}

QString ProcessBundle::run() {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrProcessReport hrprocrep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Process Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		return QString("Error: cannot connect to host\n");		
	}
	
	rv += rv1;
	rv += hrprocrep.get();
	return rv;
}

QString InterfaceBundle::run() {
	SystemReport sysrep(version_, community_, port_, ip_);
	InterfaceReport ifrep(version_, community_, port_, ip_);
	IfipReport ifip(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Interface Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		return QString("Error: cannot connect to host\n");		
	}
	
	QString rv2 = ifip.get();
	if (rv2.isEmpty()) {
		return QString("Error: cannot connect to host\n");		
	}
	
	rv += rv1 + rv2;
	rv += ifrep.get();
	return rv;
}

QString RouteBundle::run() {
	SystemReport sysrep(version_, community_, port_, ip_);
	RouteReport routerep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Route Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		return QString("Error: cannot connect to host\n");		
	}
	
	QString rv2 = routerep.get();
	if (rv2.isEmpty()) {
		return QString("Error: cannot connect to host\n");		
	}
	
	rv += rv1 + rv2;
	return rv;
}

QString ConnectionBundle::run() {
	SystemReport sysrep(version_, community_, port_, ip_);
	TcpConnectionReport tcprep(version_, community_, port_, ip_);
	UdpConnectionReport udprep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Connection Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		return QString("Error: cannot connect to host\n");		
	}
	
	QString rv2 = tcprep.get();
	QString rv3 = udprep.get();
	
	rv += rv1 + rv2 + rv3;
	return rv;
}

