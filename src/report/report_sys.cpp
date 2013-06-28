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
	if (devlist.count() == 0) {
		return "Error: .1.3.6.1.2.1.25.3.2 MIB not found<br/>\n";	
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
		out += "CPU load averages (1/5/15 minutes):<br/><table border=\"1\" cellpadding=\"10\"><tr>";
		bool second_raw = false;
		input = rv;
		lines = input.split( "\n", QString::SkipEmptyParts );
		bool found = false;
		foreach (QString line, lines) {
			if (line.startsWith("iso.3.6.1.4.1.2021.10.1.2.")) {
				out += "<td>" + extract_string(line) + "</td>";
				found = true;
			}
			else if (line.startsWith("iso.3.6.1.4.1.2021.10.1.3.")) {
				if (!second_raw) {
					out += "</tr><tr>";
					second_raw = true;
				}
				found = true;
				out += "<td>" + extract_string(line) + "</td>";
			}
		}
		if (!found)
			return "Error: .1.3.6.1.4.1.2021.10 MIB not found.<br/>\n";
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
		QStringList availlist;
		QStringList usedpercentlist;
		
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
		if (cnt == 0)
			return "Error: .1.3.6.1.2.1.25.2.3 MIB not found<br/>\n";
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
		for (i = 0; i < cnt; i++) {
			QString oid = "iso.3.6.1.2.1.25.2.3.1.6." + devid[i];
			char *ptr = strstr(rv, oid.toStdString().c_str());
			if (ptr == NULL)
				usedlist += "0";
			else {
				QString line = ptr;
				int index = line.indexOf("\n");
				if (index != -1) {
					line.truncate(index);
					usedlist += extract_integer(line);
				}
				else
					usedlist += "0";
			}
		}
		
		// build available and used percentage
		for (i = 0; i < cnt; i++) {
			bool ok;
			quint64 size = sizelist[i].toULongLong(&ok, 10);
			if (!ok)
				size = 0;					
			quint64 used = usedlist[i].toULongLong(&ok, 10);
			if (!ok)
				used = 0;
			quint64 avail = size - used;
			quint64 usedpercent = 0;
			if (size)
				usedpercent = 100 - ((avail * 100) / size);
			availlist += QString("%1").arg(avail);
			usedpercentlist += QString("%1").arg(usedpercent) + "%";
		}
		
		out += "<table border=\"1\" cellpadding=\"10\">";
		out += "<tr><td>Description</td><td>Size (kB)</td><td>Used (kB)</td><td>Available (kB)</td><td>Used (%)</td></tr>";
		for (i = 0; i < cnt; i++) {
			out += "<tr><td>" + desclist[i] + "</td>";
			out += "<td>" + sizelist[i] + "</td>";
			out += "<td>" + usedlist[i] + "</td>";
			out += "<td>" + availlist[i] + "</td>";
			out += "<td>" + usedpercentlist[i] + "</td></tr>";
		}
		out += "</table>";
	}
printf("#%s#\n", out.toStdString().c_str());
	
	return out + "<br/><br/>";
}

		