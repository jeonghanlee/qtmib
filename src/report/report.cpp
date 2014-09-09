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

#include <QStringList>
#include <stdio.h>
#include "report.h"
#include "../common/exec_prog.h"
#include "qtmib_report.h"

char *Report::snmpwalk(QString oid) {
	QString cmd = QString("snmpwalk") + " -m \"\" ";
	cmd += "-" + version_ + " ";
	cmd += "-c " + community_ + " ";
	cmd += "-t " + timeout_ + " -r " + retries_ + " ";
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
		