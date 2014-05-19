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

#ifndef REPORT_H
#define REPORT_H

#include <QString>

class Report {

public:
	Report(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		version_(version), community_(community), port_(port), ip_(ip), timeout_(timeout), retries_(retries) {}
	virtual QString get() = 0;
		
protected:
	char *snmpwalk(QString oid);
	static QString extract_string(QString line);
	static QString extract_timeticks(QString line);
	static QString extract_integer(QString line);
	static QString extract_oid(QString line);
	static QString extract_gauge32(QString line);
	static QString extract_hexstring(QString line);
	static QString extract_counter32(QString line);
	static QString extract_ipaddress(QString line);
	
	QString version_;
	QString community_;
	QString port_;
	QString ip_;
	QString timeout_;
	QString retries_;
};

class SystemReport: public Report {
public:
	SystemReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class HrDeviceReport: public Report {
public:
	HrDeviceReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class HrStorageReport: public Report {
public:
	HrStorageReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class HrSoftwareReport: public Report {
public:
	HrSoftwareReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class HrProcessReport: public Report {
public:
	HrProcessReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class InterfaceReport: public Report {
public:
	InterfaceReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class IfipReport: public Report {
public:
	IfipReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class RouteReport: public Report {
public:
	RouteReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class UdpConnectionReport: public Report {
public:
	UdpConnectionReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

class TcpConnectionReport: public Report {
public:
	TcpConnectionReport(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Report(version, community, port, ip, timeout, retries) {}
	QString get();
};

#endif


