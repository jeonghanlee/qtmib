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

#include "report.h"
#include <QStringList>
#include <stdio.h>
#include "qtmib_report.h"

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
	if (cnt == 0)
		return "Error: .1.3.6.1.2.1.25.6.3 MIB not found.<br/>\n";
	
	out += "</table>\n";

	if (dbg)
		printf("#%s#\n", out.toStdString().c_str());

	char buf[20];
	sprintf(buf, "%d", cnt);
	return "<b>Installed Software:</b><br/><br/>" + QString(buf) + " packages installed<br/>" + out + "<br/><br/><br/>";
}
