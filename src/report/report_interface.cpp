#include <QStringList>
#include <stdio.h>
#include "report.h"


QString InterfaceReport::get() {
	char *rv = snmpwalk(".1.3.6.1.2.1.2.2");
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
	
	if (cnt != kindex.count() || cnt != name.count())
		return "";
	
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
		return "";
	
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
		return "";
		
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
		return "";
		
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
		return "";
		
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
		return "";
		
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Name</td><td>MTU</td><td>Speed</td><td>MAC Address</td>";
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
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Name</td><td>Unicast</td><td>Multicast</td><td>Discards</td><td>Errors</td></td>";
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
	out += "<table border=\"1\" cellpadding=\"10\"><tr><td>Name</td><td>Unicast</td><td>Multicast</td><td>Discards</td><td>Errors</td></td>";
	for (int i = 0; i < cnt; i++) {
		out += "<tr><td>" + name[i] + "</td><td>" + ucast[i] + "</td>";
		out += "<td>" + nucast[i] + "</td>";
		out += "<td>" + disc[i] + "</td>";
		out += "<td>" + err[i] + "</td></tr>";
	
	}
	out += "</table>\n";
	
	
//printf("#%s#\n", out.toStdString().c_str());

	return "<b>Interfaces:</b><br/>" + out + "<br/><br/><br/>";
}

	