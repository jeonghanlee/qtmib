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

#include "report.h"
#include <QStringList>
#include <stdio.h>
#include "qtmib_report.h"

QString HrProcessReport::get() {
	char *rv = snmpwalk(".1.3.6.1.2.1.25.4.2");
	if (!rv)
		return "";
	char *rv2 = snmpwalk(".1.3.6.1.2.1.25.5.1");
	if (!rv2)
		return "";
	
	QString out = "<table border=\"1\" cellpadding=\"10\">\n";

	// pid and name
	QString input = rv;
	QStringList lines = input.split( "\n", QString::SkipEmptyParts );
	int cnt = 0;
	QStringList pid;
	QStringList name;
	foreach (QString line, lines) {
		if (line.startsWith("iso.3.6.1.2.1.25.4.2.1.2.")) {
			int index = line.indexOf(" = ");
			if (index != -1) {
				QString pname = extract_string(line);
//printf("#%s#\n", pname.toStdString().c_str());
				name += pname;
				cnt++;
				
				line.truncate(index);
				pid += line.mid(25);
			}
		}
	}
	if (cnt == 0)
		return "Error: .1.3.6.1.2.1.25.4.2 MIB not found.<br/>\n";
	
	// cpu
	QStringList cpu;
	for (int i = 0; i < cnt; i++) {	
		QString oid = "iso.3.6.1.2.1.25.5.1.1.1." + pid[i];
		char *ptr = strstr(rv2, oid.toStdString().c_str());
		if (ptr == NULL)
			cpu += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				cpu += extract_integer(line);
			}
			else
				cpu += " ";
		}
	}
	
	// memory	
	QStringList mem;
	for (int i = 0; i < cnt; i++) {	
		QString oid = "iso.3.6.1.2.1.25.5.1.1.2." + pid[i];
		char *ptr = strstr(rv2, oid.toStdString().c_str());
		if (ptr == NULL)
			mem += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				mem += extract_integer(line);
			}
			else
				mem += " ";
		}
	}	
	
	// build table	
	out += "<tr><td>PID</td><td width=\"200\">Process Name</td><td>CPU Time</td><td>Memory</td></tr>";
	for (int i = 0; i < cnt; i++) {
		out += "<tr><td>" + pid[i] + "</td><td>" + name[i] + "</td><td>" +
			cpu[i] + "</td><td>" + mem[i] + "</td></tr>\n";
	}
		
	out += "</table><br/><br/>\n";

	// legend
	out += "<i><ul><li>PID - process ID</li>";
	out += "<li>CPU Time - the number of centi-seconds of the total system's " +
       		QString("CPU resources consumed by this process.</li>");
       	out += "<li>Memory - The total amount of real system memory allocated " +
        	QString("to this process.</li></ul></i>");
        	
	if (dbg)
		printf("#%s#\n", out.toStdString().c_str());

	char buf[20];
	sprintf(buf, "%d", cnt);
	return "<b>Processes:</b><br/><br/>" + QString(buf) + " processes running<br/>" + out + "<br/><br/><br/>";
}
