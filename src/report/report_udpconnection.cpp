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

#include <QStringList>
#include <stdio.h>
#include "report.h"
#include "qtmib_report.h"

QString UdpConnectionReport::get() {
	char *rv1 = snmpwalk(".1.3.6.1.2.1.7");
	if (!rv1)
		return "";

	// index and ip
	QString out = "";
	QStringList index;
	QStringList ip;
	int cnt = 0;
	QString input = rv1;
	QStringList lines = input.split( "\n", QString::SkipEmptyParts );
	foreach (QString line, lines) {
		if (line.startsWith("iso.3.6.1.2.1.7.5.1.1.")) {
			QString myip = extract_ipaddress(line);
//printf("#%s#\n", myip.toStdString().c_str());
			int myindex = line.indexOf(" = ");
			if (myindex != -1) {
				line.truncate(myindex);
				index += line.mid(22);
				cnt++;
				ip += myip;
			}
		}
	}
	if (cnt == 0)
		return "Error: .1.3.6.1.2.1.7 MIB not found<br/>\n";
		
	if (cnt != index.count() || cnt != ip.count())
		return "Error: cannot parse .1.3.6.1.2.1.7 MIB<br/>\n";

	// extract port
	QStringList port;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.7.5.1.2." + index[i];
//printf("#%s#\n", oid.toStdString().c_str());
		char *ptr = strstr(rv1, oid.toStdString().c_str());
		if (ptr == NULL)
			port += " ";
		else {
			QString line = ptr;
			int myindex = line.indexOf("\n");
			if (myindex != -1) {
				line.truncate(myindex);
				port += extract_integer(line);
			}
			else {
				port += " ";
			}
		}
	}
	if (cnt != port.count())
		return "Error: cannot parse .1.3.6.1.2.1.7 MIB<br/>\n";
		
	// extract remote address
	QStringList remote;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.7.7.1.8.1.4." + index[i] + ".1.4.";
//printf("#%s#\n", oid.toStdString().c_str());
		char *ptr = strstr(rv1, oid.toStdString().c_str());
		if (ptr == NULL)
			remote += " ";
		else {
			QString line = ptr;
			int myindex = line.indexOf(" = ");
			if (myindex != -1) {
				line.truncate(myindex);
				line = line.mid(oid.length());
				
				// toknize and extract the ip address
				QStringList list = line.split(".");
				if (list.count() != 6)
					remote += " ";
				else {
					QString addr = list[0] + "." +
						list[1] + "." +
						list[2] + "." +
						list[3] + ":" +
						QString((list[4] == "0")? "*": list[4]);
					remote += addr;
				}
			}
			else {
				remote += " ";
			}
		}
	}
	if (cnt != remote.count())
		return "Error: cannot parse .1.3.6.1.2.1.7 MIB<br/>\n";

	// print table
	out += "<b>UDP Connection Table:</b><br/><br/>";
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Local Address</td><td>Remote Address</td></tr>\n";
	
	for (int i = 0; i < cnt; i++) {
		out += "<tr>";
		out += QString("<td>") + ip[i].toStdString().c_str() + ":" + port[i].toStdString().c_str() + "</td>\n";
		out += QString("<td>") + remote[i].toStdString().c_str() + "</td>";
		out += "</tr>\n";
	}
	out += "</table><br/><br/><br/>\n";

	if (dbg)
		printf("#%s#\n", out.toStdString().c_str());
	return out;
}



