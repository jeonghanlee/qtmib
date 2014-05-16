#include <stdio.h>
#include <string.h>
#include "../../qtmib_config.h"

#define BUFMAX 1024

static void help() {
	printf(
"qtmib is an easy-to-use SNMP MIB Browser based on QT4 library. It is build\n"
"as a front-end for net-snmp tools, and it allows the user to query any SNMP\n"
"enabled device. It supports SNMPv1, SNMPv2c. qtmib is released under GPL v2\n"
"license. See http://qtmib.sourceforge.net for more details.\n\n");
	printf("qtmib-list prints a list of all available mibs\n\n");
	printf("Usage: qtmib-list [options] [filename]\n");
	printf("where\n");
	printf("\tfilename - optional file containing the MIB tree model; by default, the program uses\n");
	printf("\t\tthe model embedded in qtmib\n");
	printf("\n");
	printf("Options:\n");
	printf("\t-h, --help: this help screen\n");
	printf("\t-v, --version: version information\n");
	printf("\n");
}

// count the number of tabs at the start of the file
static inline int tabs(const char *str) {
	if (!str)
		return 0;
	
	int rv = 0;
	while (*str == '\t') {
		rv++;
		str++;
	}
	
	return rv;
}

int main(int argc, char **argv) {
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
	}
	
	FILE *fp;
	if (argc == 2) {
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			fprintf(stderr, "Error: cannot open %s file\n", argv[1]);
			return 1;
		}
	}
	else {
		printf("not implemented\n");
		return 1;
	}
	
	char buf[BUFMAX + 1];
	int t = 3;
	int enterprises = 0;
	while (fgets(buf, BUFMAX, fp)) {
		if (strstr(buf, "mib-2(1)")) {
			printf("mib-2(1.2.6.1.2)\n");
			t = 3;
			continue;
		}
		
		if (enterprises == 0 && strstr(buf, "experimental(3)")) {
			printf("experimental(1.2.6.1.3)\n");
			t = 2;
			continue;
		}

		if (strstr(buf, "enterprises(1)")) {
			printf("enterprises(1.3.6.1.4.1)\n");
			t = 3;
			enterprises = 1;
			continue;
		}

		if (strstr(buf, "snmpV2(6)")) {
			printf("snmpV2(1.3.6.1.6)\n");
			t = 2;
			continue;
		}


		if (tabs(buf) == t)
			printf("\t%s", buf + t);
	}
	fclose(fp);
	return 0;
}
