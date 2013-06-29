#include <QStringList>
#include <stdio.h>
#include "report.h"
#include "exec_prog.h"
#include "qtmib_report.h"

char *Report::snmpwalk(QString oid) {
	QString cmd = QString("snmpwalk") + " -m \"\" ";
	cmd += "-" + version_ + " ";
	cmd += "-c " + community_ + " ";
	cmd += "-t 1 -r 1 ";
	cmd += ip_ + ":" + port_ + " ";
	cmd += oid + " 2>&1";
	if (dbg)
		printf("\n%s\n", cmd.toStdString().c_str());
	
	// execute command
	char *rv = exec_prog(cmd.toStdString().c_str());
	if (!rv || strncmp(rv, "Timeout", 7) == 0)
		return NULL;
	
	return rv;
}		

QString Report::extract_string(QString line) {
	int index = line.indexOf(" = STRING: \"");
	if (index != -1) {
		index += 12;
		QString left = line.mid(index);
		index = left.indexOf("\"");
		if (index != -1)
			left.truncate(index); 
		return left;
	}
	
	return QString(" ");
}

QString Report::extract_timeticks(QString line) {
	int index = line.indexOf(" = Timeticks: ");
	if (index != -1) {
		index += 14;
		QString rv = line.mid(index);
		return rv;
	}
	
	return QString(" ");
}

QString Report::extract_integer(QString line) {
	int index = line.indexOf(" = INTEGER: ");
	if (index != -1) {
		index += 12;
		QString rv = line.mid(index);
		return rv;
	}
	
	return QString(" ");
}

QString Report::extract_oid(QString line) {
	int index = line.indexOf(" = OID: ");
	if (index != -1) {
		index += 8;
		QString rv = line.mid(index);
		return rv;
	}
	
	return QString(" ");
}

QString Report::extract_gauge32(QString line) {
	int index = line.indexOf(" = Gauge32: ");
	if (index != -1) {
		index += 12;
		QString left = line.mid(index);
		index = left.indexOf("\"");
		if (index != -1)
			left.truncate(index); 
		return left;
	}
	
	return QString(" ");
}

QString Report::extract_hexstring(QString line) {
	int index = line.indexOf(" = Hex-STRING: ");
	if (index != -1) {
		index += 15;
		QString left = line.mid(index);
		index = left.indexOf("\"");
		if (index != -1)
			left.truncate(index); 
		return left;
	}
	
	return QString(" ");
}

QString Report::extract_counter32(QString line) {
	int index = line.indexOf(" = Counter32: ");
	if (index != -1) {
		index += 14;
		QString left = line.mid(index);
		index = left.indexOf("\"");
		if (index != -1)
			left.truncate(index); 
		return left;
	}
	
	return QString(" ");
}

QString Report::extract_ipaddress(QString line) {
	int index = line.indexOf(" = IpAddress: ");
	if (index != -1) {
		index += 14;
		QString rv = line.mid(index);
		return rv;
	}
	
	return QString(" ");
}
		