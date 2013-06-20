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
#include <QApplication>
#include <stdio.h>
#include "mainwindow.h"
#include "../../qtmib_config.h"
#include "qtmib_discover.h"

// global debug flag
bool dbg = false;

static void help() {
	printf(
"qtmib is an easy-to-use SNMP MIB Browser based on QT4 library. It is build\n"
"as a front-end for net-snmp tools, and it allows the user to query any SNMP\n"
"enabled device. It supports SNMPv1, SNMPv2c. qtmib is released under GPL v2\n"
"license. See http://qtmib.sourceforge.net for more details.\n\n");
	printf("qtmib-discover is the network discovery component of qtmib\n\n");
	printf("Usage: qtmib-discover [options]\n");
	
	printf("Options:\n");
	printf("\t-v, --version: version information\n");
	printf("\t-h, --help: this help screen\n");
	printf("\t--debug: enable debug messages\n");
	printf("\n");
}

int main(int argc, char *argv[]) {
	// parse arguments
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
			printf("%s version %s\n", argv[0], PACKAGE_VERSION);
			return 0;
		}
		else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			help();
			return 0;
		}
		else if (strcmp(argv[i], "--debug") == 0) {
			dbg = true;
		}
		else {
			printf("Error: unknown program argument\n\n");
			help();
			return 1;
		}
	}

	QApplication app(argc, argv);
	MainWindow mainWin;
	mainWin.resize(500, 400);
	mainWin.show();
	return app.exec();
}
