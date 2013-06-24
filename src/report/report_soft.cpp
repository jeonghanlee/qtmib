#include "report.h"
#include <QStringList>
#include <stdio.h>

QString HrSoftwareReport::get() {
	char *rv = snmpwalk(".1.3.6.1.2.1.25.6.3");
	if (!rv)
		return "";
		
	QString out = "<table border=\"1\" cellpadding=\"10\">\n";

	QString input = rv;
	QStringList lines = input.split( "\n", QString::SkipEmptyParts );
	int cnt = 0;
	foreach (QString line, lines) {
		if (line.startsWith("iso.3.6.1.2.1.25.6.3.1.2.")) {
			QString soft = extract_string(line);
//printf("#%s#\n", soft.toStdString().c_str());
			out += "<tr><td>" + soft + "</td></tr>\n";
			cnt++;
		}
	}
	
	out += "</table>\n";

//printf("#%s#\n", out.toStdString().c_str());

	char buf[20];
	sprintf(buf, "%d", cnt);
	return "<b>Installed Software:</b><br/><br/>" + QString(buf) + " packages installed<br/>" + out + "<br/><br/><br/>";
}
