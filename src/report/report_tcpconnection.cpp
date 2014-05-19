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
#include "qtmib_report.h"

QString TcpConnectionReport::get() {
	char *rv1 = snmpwalk(".1.3.6.1.2.1.6.13");
	if (!rv1)
		return "";

	// index and ip
	QString out = "";
	QStringList index;
	QStringList status;
	QStringList localip;
	QStringList remoteip;
	
	int cnt = 0;
	QString input = rv1;
	QStringList lines = input.split( "\n", QString::SkipEmptyParts );
	foreach (QString line, lines) {
		if (line.startsWith("iso.3.6.1.2.1.6.13.1.1.")) {
			QString mystatus = extract_integer(line);
//printf("#%s#\n", mystatus.toStdString().c_str());
			int myindex = line.indexOf(" = ");
			if (myindex != -1) {
				line.truncate(myindex);
				index += line.mid(23);
				cnt++;
				if (mystatus == "1")
					mystatus = "Closed";
				else if (mystatus == "2")
					mystatus = "Listen";
				else if (mystatus == "3")
					mystatus = "SynSent";
				else if (mystatus == "4")
					mystatus = "SynReceived";
				else if (mystatus == "5")
					mystatus = "Established";
				else if (mystatus == "6")
					mystatus = "FinWait1";
				else if (mystatus == "7")
					mystatus = "FinWait2";
				else if (mystatus == "8")
					mystatus = "CloseWait";
				else if (mystatus == "9")
					mystatus = "LastAck";
				else if (mystatus == "10")
					mystatus = "Closing";
				else if (mystatus == "11")
					mystatus = "Time Wait";
				else if (mystatus == "12")
					mystatus = "DeleteTCB";

				status += mystatus;
				
				// parse the index
				QStringList list = line.mid(23).split(".");
				if (list.count() != 10) {
					localip += " ";
					remoteip += " ";
				}
				else {
					QString addr1 = list[0] + "." +
						list[1] + "." +
						list[2] + "." +
						list[3] + ":" +
						QString((list[4] == "0")? "*": list[4]);
					QString addr2 = list[5] + "." +
						list[6] + "." +
						list[7] + "." +
						list[8] + ":" +
						QString((list[9] == "0")? "*": list[9]);
					localip += addr1;
					remoteip += addr2;
				}
			}
		}
	}
	if (cnt == 0)
		return "Error: .1.3.6.1.2.1.6.13 MIB not found<br/>\n";
		
	if (cnt != index.count() || cnt != status.count() || cnt != localip.count() || cnt != remoteip.count())
		return "Error: cannot parse .1.3.6.1.2.1.6.13 MIB<br/>\n";


	// print table
	out += "<b>TCP Connection Table:</b><br/><br/>";
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Local IP</td><td>Remote IP</td><td>status</td></tr>\n";
	
	for (int i = 0; i < cnt; i++) {
		out += "<tr>";
		out += QString("<td>") + localip[i].toStdString().c_str() + "</td>\n";
		out += QString("<td>") + remoteip[i].toStdString().c_str() + "</td>\n";
		out += QString("<td>") + status[i].toStdString().c_str() + "</td>\n";
		out += "</tr>\n";
	}
	out += "</table><br/><br/><br/>\n";

	if (dbg)
		printf("#%s#\n", out.toStdString().c_str());
	return out;
}



