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

static char *ifmib_storage = 0;

QString IfipReport::get() {
	message("Loading MIBs ...");
	char *rv1 = snmpwalk(".1.3.6.1.2.1.2.2");
	if (!rv1)
		return "";
	ifmib_storage = rv1;

	char *rv2 = snmpwalk(".1.3.6.1.2.1.4.20");
	if (!rv2)
		return "";

	// kindex and name
	message("Extracting data ...");
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
		if (line.startsWith("iso.3.6.1.2.1.4.20.1.1.")) {
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
		return "Error: cannot parse .1.3.6.1.2.1.4.20 MIB<br/>\n";

	// extract kindex
	QStringList addr_kindex;
	for (int i = 0; i < addr_cnt; i++) {
		QString oid = "iso.3.6.1.2.1.4.20.1.2." + addr_index[i];
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
		return "Error: cannot parse .1.3.6.1.2.1.4.20 MIB<br/>\n";

	// extract mask
	QStringList addr_mask;
	for (int i = 0; i < addr_cnt; i++) {
		QString oid = "iso.3.6.1.2.1.4.20.1.3." + addr_index[i];
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
		return "Error: cannot parse .1.3.6.1.2.1.4.20 MIB<br/>\n";


	// print table
	out += "<b>Interface Address:</b><br/><br/>";
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Interface</td><td>Address</td><td>Mask</td></tr>\n";
	
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
			out += QString("<tr><td>") + ifname.toStdString().c_str() + "</td>";
			out += QString("<td>") + addr_addr[i].toStdString().c_str() + "</td>";
			out += QString("<td>") + addr_mask[i].toStdString().c_str() + "</td></tr>\n";
			
		}
		
	}
	out += "</table><br/><br/><br/>\n";

	if (dbg)
		printf("#%s#\n", out.toStdString().c_str());
	return out;
}



QString InterfaceReport::get() {
	char *rv = 0;
	if (ifmib_storage)
		rv = ifmib_storage;
	else
		rv = snmpwalk(".1.3.6.1.2.1.2.2");
	if (!rv)
		return "";
		
	QString out = "";
	QStringList kindex;
	QStringList name;
	int cnt = 0;

	QString input = rv;
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
	
	// extract MTU
	QStringList mtu;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.4." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			mtu += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				mtu += extract_integer(line);
			}
			else
				mtu += " ";
		}
	}
	if (cnt != mtu.count())
		return "Error: cannot parse .1.3.6.1.2.1.2.2 MIB<br/>\n";
	
	// extract speed
	QStringList speed;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.5." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			speed += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				speed += extract_gauge32(line);
			}
			else
				speed += " ";
		}
	}
	if (cnt != speed.count())
		return "Error: cannot parse .1.3.6.1.2.1.2.2 MIB<br/>\n";
		
	// extract MAC address
	QStringList mac;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.6." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			mac += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				mac += extract_hexstring(line);
			}
			else
				mac += " ";
		}
	}
	if (cnt != mac.count())
		return "Error: cannot parse .1.3.6.1.2.1.2.2 MIB<br/>\n";
		
	// extract admin status
	QStringList admin;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.7." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			admin += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				QString status = extract_integer(line);
				if (status == "1")
					admin += "Up";
				else
					admin += "Down";
			}
			else
				admin += " ";
		}
	}
	if (cnt != admin.count())
		return "Error: cannot parse .1.3.6.1.2.1.2.2 MIB<br/>\n";
		
	// extract oper status
	QStringList oper;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.8." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			oper += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				QString status = extract_integer(line);
				if (status == "1")
					oper += "Up";
				else
					oper += "Down";
			}
			else
				oper += " ";
		}
	}
	if (cnt != oper.count())
		return "Error: cannot parse .1.3.6.1.2.1.2.2 MIB<br/>\n";
		
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Interface</td><td>MTU</td><td>Speed</td><td>MAC Address</td>";
	out += "<td>Admin/Oper<br/>Status</td></tr>\n";
	for (int i = 0; i < cnt; i++) {
		out += "<tr><td>" + name[i] + "</td>";
		out += "<td>" + mtu[i] + "</td><td>" + speed[i] + "</td>";
		out += "<td>" + mac[i] + "</td>";
		out += "<td>" + admin[i] + "/" + oper[i] + "</td></tr>\n";
	
	}
	out += "</table>\n";
	
	// packet counts
	// extract input unicast
	QStringList ucast;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.11." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			ucast += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				ucast += extract_counter32(line);
			}
			else
				ucast += " ";
		}
	}
	if (cnt != ucast.count())
		return out;
	
	// extract input nunicast
	QStringList nucast;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.12." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			nucast += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				nucast += extract_counter32(line);
			}
			else
				nucast += " ";
		}
	}
	if (cnt != nucast.count())
		return out;
	
	// extract input discards
	QStringList disc;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.13." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			disc += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				disc += extract_counter32(line);
			}
			else
				disc += " ";
		}
	}
	if (cnt != disc.count())
		return out;
	
	// extract input errors
	QStringList err;
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.14." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			err += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				err += extract_counter32(line);
			}
			else
				err += " ";
		}
	}
	if (cnt != err.count())
		return out;
	
	out += "<br/><br/><br/><b>Input Packet Counts:</b><br/><br/>";
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Interface</td><td>Unicast</td><td>Multicast &<br/>Broadcast</td><td>Discards</td><td>Errors</td></td>";
	for (int i = 0; i < cnt; i++) {
		out += "<tr><td>" + name[i] + "</td><td>" + ucast[i] + "</td>";
		out += "<td>" + nucast[i] + "</td>";
		out += "<td>" + disc[i] + "</td>";
		out += "<td>" + err[i] + "</td></tr>";
	
	}
	out += "</table>\n";
	
	
	// extract output unicast
	ucast.clear();
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.17." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			ucast += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				ucast += extract_counter32(line);
			}
			else
				ucast += " ";
		}
	}
	if (cnt != ucast.count())
		return out;
	
	// extract output nunicast
	nucast.clear();
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.18." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			nucast += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				nucast += extract_counter32(line);
			}
			else
				nucast += " ";
		}
	}
	if (cnt != nucast.count())
		return out;
	
	// extract output discards
	disc.clear();
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.19." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			disc += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				disc += extract_counter32(line);
			}
			else
				disc += " ";
		}
	}
	if (cnt != disc.count())
		return out;
	
	// extract output errors
	err.clear();
	for (int i = 0; i < cnt; i++) {
		QString oid = "iso.3.6.1.2.1.2.2.1.20." + kindex[i];
		char *ptr = strstr(rv, oid.toStdString().c_str());
		if (ptr == NULL)
			err += " ";
		else {
			QString line = ptr;
			int index = line.indexOf("\n");
			if (index != -1) {
				line.truncate(index);
				err += extract_counter32(line);
			}
			else
				err += " ";
		}
	}
	if (cnt != err.count())
		return out;
	
	out += "<br/><br/><br/><b>Output Packet Counts:</b><br/><br/>";
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Interface</td><td>Unicast</td><td>Multicast &<br/>Broadcast</td><td>Discards</td><td>Errors</td></td>";
	for (int i = 0; i < cnt; i++) {
		out += "<tr><td>" + name[i] + "</td><td>" + ucast[i] + "</td>";
		out += "<td>" + nucast[i] + "</td>";
		out += "<td>" + disc[i] + "</td>";
		out += "<td>" + err[i] + "</td></tr>";
	
	}
	out += "</table>\n";
	
	
	if (dbg)
		printf("#%s#\n", out.toStdString().c_str());

	return "<b>Interfaces:</b><br/>" + out + "<br/><br/><br/>";
}

	