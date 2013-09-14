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

#include "qtmib_parse.h"
#include "tokens.h"
#include "db.h"
#include "../../qtmib_config.h"
#include <stdio.h>
#include <string.h>

const char *current_fname = 0;
bool debug = false;


static void usage(const char *prog) {
	printf(
"qtmib-translate translates SNMP MIBs from ASN.1 format into qtmib format.\n"
"It is part of qtmib SNMP MIB Browser package. The program is released under\n"
"under GPL v2 license. See http://qtmib.sourceforge.net for more details.\n\n");
	printf("Usage: %s [options] files\n", prog);
	printf("Options:\n");
	printf("\t--debug: enable debug messages\n");
	printf("\t-h, --help: this help screen\n");
	printf("\t-v, --version: version information\n");
	printf("\n");
}

void process_file(const char *fname) {
	if (debug)
		printf("Information: processing file %s\n", fname);
	current_fname = fname;

	// open file
	FILE *fp = fopen(fname, "r");
	if (!fp) {
		fprintf(stderr, "Error: cannot open file %s\n", fname);
		return;
	}
	
	// read file
	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	if (len == 0) {
		fprintf(stderr, "Error: the file %s is empty\n", fname);
		return;
	}
	char *fcontent = new char[len + 1];	
	rewind(fp);	
	size_t rv = fread(fcontent, 1, len, fp);
	fcontent[len] = '\0';
	fclose(fp);
	
	if (rv != len) {
		fprintf(stderr, "Error: cannot read file %s\n", fname);
		return;
	}
	
	// tokenize file
	const char *str = fcontent;
	while (*str != '\0') {
		str = tokenize(str);
	}
	
//	head->printall();
	
	// parse file
	parse();
	
	// clear memory
	delete head;
	head = 0;
	delete [] fcontent;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}
	
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
			usage(argv[0]);
			return 0;
		}
		else if (strcmp(argv[i], "--debug") == 0) {
			debug = true;
		}
		else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
			printf("%s version %s\n", argv[0], PACKAGE_VERSION);
			return 0;
		}
		else
			break;
	}
	
	
	// initialize database
	dbAdd(new DbEntry("iso", "1"));
	dbAdd(new DbEntry("org", "iso", "3"));
	dbAdd(new DbEntry("dod", "org", "6"));
	dbAdd(new DbEntry("internet", "dod", "1"));
	dbAdd(new DbEntry("mgmt", "internet", "2"));
	dbAdd(new DbEntry("mib-2", "mgmt", "1"));
	dbAdd(new DbEntry("private", "internet", "4"));
	dbAdd(new DbEntry("enterprises", "private", "1"));
	
	// process files
	for (; i < argc; i++) {
		process_file(argv[i]);
	}
	
	// print database
	dbPrint();
	
	return 0;
}
