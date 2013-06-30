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
#include "qtmib_report.h"
#include "bundle.h"

// global debug flag
bool dbg = false;

static void help() {
	printf(
"qtmib-report generates reports based on SNMP information extracted from an\n"
"SNMP agent. It is part of qtmib SNMP MIB Browser package. The program is\n"
"released under GPL v2 license. See http://qtmib.sourceforge.net for more\n"
"details.\n\n");
	printf("Usage: qtmib-report [options]\n");
	
	printf("Options:\n");
	printf("\t-v, --version: version information\n");
	printf("\t-h, --help: this help screen\n");
	printf("\t--debug: enable debug messages\n");
	printf("\t--system: generate a system report (default)\n");
	printf("\t--interface: generate an interface report\n");
	printf("\t--process: list all running processes\n");
	printf("\t--software: list all installed software packages\n");
	printf("\n");
}

int main(int argc, char *argv[]) {
	Bundle *bundle = 0;
	
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
		else if (strcmp(argv[i], "--system") == 0) {
			bundle = new SystemBundle("v2c", "public", "161", "127.0.0.1");
		}
		else if (strcmp(argv[i], "--interface") == 0) {
			bundle = new InterfaceBundle("v2c", "public", "161", "127.0.0.1");
		}
		else if (strcmp(argv[i], "--process") == 0) {
			bundle = new ProcessBundle("v2c", "public", "161", "127.0.0.1");
		}
		else if (strcmp(argv[i], "--software") == 0) {
			bundle = new SoftwareBundle("v2c", "public", "161", "127.0.0.1");
		}
		else {
			printf("Error: unknown program argument\n\n");
			help();
			return 1;
		}
	}

	if (!bundle)
		bundle = new SystemBundle("v2c", "public", "161", "127.0.0.1");
		
	QApplication app(argc, argv);
	MainWindow mainWin(bundle);
	mainWin.resize(600, 400);
	mainWin.show();
	return app.exec();
}
