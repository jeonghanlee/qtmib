#ifndef REPORT_H
#define REPORT_H

#include <QString>

class Report {

public:
	Report(QString version, QString community, QString port, QString ip):
		version_(version), community_(community), port_(port), ip_(ip) {}
	virtual QString get() = 0;
		
protected:
	char *snmpwalk(QString oid);
	static QString extract_string(QString line);
	static QString extract_timeticks(QString line);
	static QString extract_integer(QString line);
	static QString extract_oid(QString line);
	
	QString version_;
	QString community_;
	QString port_;
	QString ip_;
};

class SystemReport: public Report {
public:
	SystemReport(QString version, QString community, QString port, QString ip):
		Report(version, community, port, ip) {}
	QString get();
};

class HrDeviceReport: public Report {
public:
	HrDeviceReport(QString version, QString community, QString port, QString ip):
		Report(version, community, port, ip) {}
	QString get();
};

class HrStorageReport: public Report {
public:
	HrStorageReport(QString version, QString community, QString port, QString ip):
		Report(version, community, port, ip) {}
	QString get();
};

#endif


