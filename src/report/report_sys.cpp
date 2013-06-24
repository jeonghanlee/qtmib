#include <QStringList>
#include <stdio.h>
#include "report.h"

QString SystemReport::get() {
	char *rv = snmpwalk(".1.3.6.1.2.1.1");
	if (!rv)
		return "";
		
	QString out = "";
	out += "<b>IP Address:</b> " + ip_ + "<br/>";
	QString input = rv;
	QStringList lines = input.split( "\n", QString::SkipEmptyParts );
	foreach (QString line, lines) {
		if (line.startsWith("iso.3.6.1.2.1.1.1.0")) {
			out += "Description: " + extract_string(line) + "<br/>";
		}
		else if (line.startsWith("iso.3.6.1.2.1.1.3.0")) {
			out += "Uptime: " + extract_timeticks(line) + "<br/>";
		}
		else if (line.startsWith("iso.3.6.1.2.1.1.4.0")) {
			out += "Contact: " + extract_string(line) + "<br/>";
		}
		else if (line.startsWith("iso.3.6.1.2.1.1.5.0")) {
			out += "Name: " + extract_string(line) + "<br/>";
		}
		else if (line.startsWith("iso.3.6.1.2.1.1.6.0")) {
			out += "Location: " + extract_string(line) + "<br/>";
		}
			
	}
	
	return out + "<br/><br/>";
}

QString HrDeviceReport::get() {
	char *rv = snmpwalk(".1.3.6.1.2.1.25.3.2");
	if (!rv)
		return "";
		
	QString out = "";
	QStringList devlist;

	QString input = rv;
	QStringList lines = input.split( "\n", QString::SkipEmptyParts );
	foreach (QString line, lines) {
		if (line.startsWith("iso.3.6.1.2.1.25.3.2.1.2.")) {
			QString oid = extract_oid(line);
//printf("#%s#\n", oid.toStdString().c_str());
			if (oid == "iso.3.6.1.2.1.25.3.1.3") {
				int index = line.indexOf(" = ");
				if (index != -1) {
					line.truncate(index);
					devlist += line.mid(25);
				}
			}
		}
	}
	
	int i;
	for (i = 0; i < devlist.count(); i++) {
		devlist[i] = "iso.3.6.1.2.1.25.3.2.1.3." + devlist[i];
		
	}
	
	QStringList devnames;
	foreach (QString line, lines) {
		
		for (i = 0; i < devlist.count(); i++) {
			if (line.startsWith(devlist[i]))
				devnames += extract_string(line);
		}
	}
	out += "<ul>";
	foreach (QString line, devnames) {
		out += "<li>" + line + "</li>\n";
	}
	out += "</ul><br/><br/>";



	rv = snmpwalk(".1.3.6.1.4.1.2021.10");
	if (rv) {
		out += "CPU load:<br/><table border=\"1\" cellpadding=\"10\"><tr>";
		bool second_raw = false;
		input = rv;
		lines = input.split( "\n", QString::SkipEmptyParts );
		foreach (QString line, lines) {
			if (line.startsWith("iso.3.6.1.4.1.2021.10.1.2.")) {
				out += "<td>" + extract_string(line) + "</td>";
			}
			else if (line.startsWith("iso.3.6.1.4.1.2021.10.1.3.")) {
				if (!second_raw) {
					out += "</tr><tr>";
					second_raw = true;
				}
				out += "<td>" + extract_string(line) + "</td>";
			}
		}
		out += "</tr>";
		out += "</table>";
	}

//printf("#%s#\n", out.toStdString().c_str());

	return "<b>CPU:</b><br/>" + out + "<br/><br/><br/>";
}

QString HrStorageReport::get() {
	int i;
	QString out = "<b>Memory:</b><br/><br/>";

	char *rv = snmpwalk(".1.3.6.1.4.1.2021.4");
	if (rv) {
		// extract device id and descriptions
		QString input = rv;
		QStringList lines = input.split( "\n", QString::SkipEmptyParts );
		foreach (QString line, lines) {
			if (line.startsWith("iso.3.6.1.4.1.2021.4.3.0"))
				out += "Total Swap: " + extract_integer(line) + "<br/>";
			else if (line.startsWith("iso.3.6.1.4.1.2021.4.4.0"))
				out += "Available Swap: " + extract_integer(line) + "<br/>";
			else if (line.startsWith("iso.3.6.1.4.1.2021.4.5.0"))
				out += "Total Real: " + extract_integer(line) + "<br/>";
			else if (line.startsWith("iso.3.6.1.4.1.2021.4.6.0"))
				out += "Available Real: " + extract_integer(line) + "<br/>";
			else if (line.startsWith("iso.3.6.1.4.1.2021.4.11.0"))
				out += "Total Free: " + extract_integer(line) + "<br/>";
			else if (line.startsWith("iso.3.6.1.4.1.2021.4.14.0"))
				out += "Buffers: " + extract_integer(line) + "<br/>";
			else if (line.startsWith("iso.3.6.1.4.1.2021.4.15.0"))
				out += "Cached: " + extract_integer(line) + "<br/>";
		}
	}	
	
	rv = snmpwalk(".1.3.6.1.2.1.25.2.3");
	if (rv) {
		QStringList devid;
		QStringList desclist;
		QStringList sizelist;
		QStringList usedlist;
		
		// extract device id and descriptions
		QString input = rv;
		QStringList lines = input.split( "\n", QString::SkipEmptyParts );
		foreach (QString line, lines) {
			if (line.startsWith("iso.3.6.1.2.1.25.2.3.1.3.")) {
				int index = line.indexOf(" = ");
				if (index != -1) {
					QString line1 = line;
					line1.truncate(index);
					devid += line1.mid(25);
					desclist += extract_string(line);
				}
			}
		}
	
		//extract device size
		int cnt = devid.size();
		for (i = 0; i < cnt; i++) {
			QString oid = "iso.3.6.1.2.1.25.2.3.1.5." + devid[i];
			char *ptr = strstr(rv, oid.toStdString().c_str());
			if (ptr == NULL)
				sizelist += " ";
			else {
				QString line = ptr;
				int index = line.indexOf("\n");
				if (index != -1) {
					line.truncate(index);
					sizelist += extract_integer(line);
				}
				else
					sizelist += " ";
			}
		}
		
		//extract used size
		cnt = devid.size();
		for (i = 0; i < cnt; i++) {
			QString oid = "iso.3.6.1.2.1.25.2.3.1.6." + devid[i];
			char *ptr = strstr(rv, oid.toStdString().c_str());
			if (ptr == NULL)
				usedlist += " ";
			else {
				QString line = ptr;
				int index = line.indexOf("\n");
				if (index != -1) {
					line.truncate(index);
					usedlist += extract_integer(line);
				}
				else
					usedlist += " ";
			}
		}
		
					
		
		out += "<table border=\"1\" cellpadding=\"10\">";
		out += "<tr><td>Description</td><td>Size</td><td>Used</td></tr>";
		for (i = 0; i < cnt; i++) {
			out += "<tr><td>" + desclist[i] + "</td>";
			out += "<td>" + sizelist[i] + "</td>";
			out += "<td>" + usedlist[i] + "</td></td>";
		}
		out += "</table>";
	}
printf("#%s#\n", out.toStdString().c_str());
	
	return out + "<br/><br/>";
}

		