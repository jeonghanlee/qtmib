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

static char *ifmib_storage = 0;

QString RouteReport::get() {
	char *rv1 = snmpwalk(".1.3.6.1.2.1.2.2");
	if (!rv1)
		return "";
	ifmib_storage = rv1;

	char *rv2 = snmpwalk(".1.3.6.1.2.1.4.21");
	if (!rv2)
		return "";

	// kindex and name
	QString out = "";
	QStringList kindex;
	QStringList name;
	int cnt = 0;
	QString input = rv1;
	QStringList lines = input.split( "\n", QString::SkipEmptyParts );
	foreach (QString line, lines) {
		if (line.startsWith("iso.3.6.1.2.1.2.2.1.2.")) {
			QString ifname = extract_string(line);
//printf("#%s#\n", oid.toStdString().c_str());
			int index = line.indexOf(" = ");
			if (index != -1) {
				line.truncate(index);
				kindex += line.mid(22);
				cnt++;
				name += ifname;
			}
		}
	}
	if (cnt == 0)
		return "Error: .1.3.6.1.2.1.2.2 MIB not found<br/>\n";
		
	if (cnt != kindex.count() || cnt != name.count())
		return "Error: cannot parse .1.3.6.1.2.1.2.2 MIB<br/>\n";


	// addresses
	int addr_cnt = 0;
	QStringList addr_index;
	QStringList addr_addr;
	input = rv2;
	lines = input.split( "\n", QString::SkipEmptyParts );
	foreach (QString line, lines) {
		if (line.startsWith("iso.3.6.1.2.1.4.21.1.1.")) {
			QString addr = extract_ipaddress(line);
//printf("#%s#\n", oid.toStdString().c_str());
			int index = line.indexOf(" = ");
			if (index != -1) {
				line.truncate(index);
				addr_index += line.mid(23);
				addr_addr += addr;
				addr_cnt++;
			}
		}
	}
	if (addr_cnt != addr_index.count() || addr_cnt != addr_addr.count())
		return "Error: cannot parse .1.3.6.1.2.1.4.21 MIB<br/>\n";

	// extract kindex
	QStringList addr_kindex;
	for (int i = 0; i < addr_cnt; i++) {
		QString oid = "iso.3.6.1.2.1.4.21.1.2." + addr_index[i];
//printf("#%s#\n", oid.toStdString().c_str());
		char *ptr = strstr(rv2, oid.toStdString().c_str());
		if (ptr == NULL)
			addr_kindex += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				addr_kindex += extract_integer(line);
			}
			else {
				addr_kindex += " ";
			}
		}
	}
	if (addr_cnt != addr_kindex.count())
		return "Error: cannot parse .1.3.6.1.2.1.4.21 MIB<br/>\n";

	// extract mask
	QStringList addr_mask;
	for (int i = 0; i < addr_cnt; i++) {
		QString oid = "iso.3.6.1.2.1.4.21.1.11." + addr_index[i];
		char *ptr = strstr(rv2, oid.toStdString().c_str());
		if (ptr == NULL)
			addr_mask += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				addr_mask += extract_ipaddress(line);
			}
			else
				addr_mask += " ";
		}
	}
	if (addr_cnt != addr_mask.count())
		return "Error: cannot parse .1.3.6.1.2.1.4.21 MIB<br/>\n";

	// extract next hop
	QStringList next_hop;
	for (int i = 0; i < addr_cnt; i++) {
		QString oid = "iso.3.6.1.2.1.4.21.1.7." + addr_index[i];
		char *ptr = strstr(rv2, oid.toStdString().c_str());
		if (ptr == NULL)
			next_hop += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				next_hop += extract_ipaddress(line);
			}
			else
				next_hop += " ";
		}
	}
	if (addr_cnt != next_hop.count())
		return "Error: cannot parse .1.3.6.1.2.1.4.21 MIB<br/>\n";


	// extract route type
	QStringList addr_type;
	for (int i = 0; i < addr_cnt; i++) {
		QString oid = "iso.3.6.1.2.1.4.21.1.8." + addr_index[i];
		char *ptr = strstr(rv2, oid.toStdString().c_str());
		if (ptr == NULL)
			addr_type += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				addr_type += extract_integer(line);
			}
			else
				addr_type += " ";
		}
	}
	if (addr_cnt != addr_type.count())
		return "Error: cannot parse .1.3.6.1.2.1.4.21 MIB<br/>\n";

	// extract metric
	QStringList addr_metric;
	for (int i = 0; i < addr_cnt; i++) {
		QString oid = "iso.3.6.1.2.1.4.21.1.3." + addr_index[i];
		char *ptr = strstr(rv2, oid.toStdString().c_str());
		if (ptr == NULL)
			addr_metric += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				addr_metric += extract_integer(line);
			}
			else
				addr_metric += " ";
		}
	}
	if (addr_cnt != addr_metric.count())
		return "Error: cannot parse .1.3.6.1.2.1.4.21 MIB<br/>\n";

	// print table
	out += "<b>Routing Table:</b><br/><br/>";
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Address</td><td>Mask</td><td>Type</td><td>Metric</td><td>Next Hop</td><td>Interface</td></tr>\n";
	
	for (int i = 0; i < addr_cnt; i++) {
		// find interface name
		QString ifname = "";
		for (int j = 0; j < cnt; j++) {
			if (addr_kindex[i] == kindex[j]) {
				ifname = name[j];
				break;
			}
		}
		if (!ifname.isEmpty()) {
			out += "<tr>";
			out += QString("<td>") + addr_addr[i].toStdString().c_str() + "</td>";
			out += QString("<td>") + addr_mask[i].toStdString().c_str() + "</td>\n";
			
			if (addr_type[i] == "1")
				out += "<td>other</td>";
			else if (addr_type[i] == "2")
				out += "<td>invalid</td>";
			else if (addr_type[i] == "3")
				out += "<td>direct</td>";
			else if (addr_type[i] == "4")
				out += "<td>indirect</td>";
			else
				out += "<td> </td>";

			out += QString("<td>") + addr_metric[i].toStdString().c_str() + "</td>";
			
			if (addr_type[i] == "3")
				out += "<td> </td>";
			else
				out += QString("<td>") + next_hop[i].toStdString().c_str() + "</td>\n";

			out += QString("<td>") + ifname.toStdString().c_str() + "</td>";
			out += "</tr>";
		}
		
	}
	out += "</table><br/><br/><br/>\n";

	if (dbg)
		printf("#%s#\n", out.toStdString().c_str());
	return out;
}



