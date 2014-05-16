#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUFSIZE 1024

static inline int itemcnt(const char *str) {
	assert(str);
	int rv = 0;
	while (*str != '\0') {
		if (*str == '.')
			rv++;
		str++;
	}
	return rv;
}

int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "Usage: qtmibadd mibtree-file new-mib-file mib-name-before\n");
		return 1;
	}
	char *mib_before = argv[2];
	char *tree_file = argv[1];
	char *new_mib_file = argv[3];
	char *tmp_file = "tmpfile";
	char *tmp_file_out = "tmpfile.out";

	FILE *fp1 = fopen(tree_file, "r");
	if (fp1 == NULL) {
		fprintf(stderr, "Error: cannot open %s\n", tree_file);
		return 1;
	}
	
	//**************************************************************
	// copy original tree until mib-before
	//**************************************************************
	char buf[BUFSIZE];
	while (fgets(buf, BUFSIZE, fp1)) {
		if (strstr(buf, mib_before))
			break;
		
		printf("%s", buf);
	}

	//**************************************************************
	// run mib translateion
	//**************************************************************
	char *cmd;
	if (asprintf(&cmd, "qtmib-translate %s | sort -u > %s", new_mib_file, tmp_file) == -1) {
		fprintf(stderr, "Error: cannot allocate memory\n");
		return 1;	
	}
	system(cmd);
	free(cmd);
	
	//**************************************************************
	// add the new mibs
	//**************************************************************
	FILE *fp2 = fopen(tmp_file, "r");
	if (fp2 == NULL) {
		fprintf(stderr, "Error: cannot open %s\n", tmp_file);
		return 1;
	}
	int line = 0;
	char buf2[BUFSIZE];
	while (fgets(buf2, BUFSIZE, fp2)) {
		line++;
		if (strncmp(buf2, "Warning:", 8) == 0)
			continue;
		if (strcmp(buf2, "iso(1)\n") == 0)
			continue;
		if (strcmp(buf2, "iso(1).org(3)\n") == 0)
			continue;
		if (strcmp(buf2, "iso(1).org(3).dod(6)\n") == 0)
			continue;
		if (strcmp(buf2, "iso(1).org(3).dod(6).internet(1)\n") == 0) {
			continue;
		}
		if (strcmp(buf2, "iso(1).org(3).dod(6).internet(1).mgmt(2)\n") == 0)
			continue;
		if (strcmp(buf2, "iso(1).org(3).dod(6).internet(1).mgmt(2).mib-2(1)\n") == 0)
			continue;
		if (strcmp(buf2, "iso(1).org(3).dod(6).internet(1).private(4)\n") == 0)
			continue;
		if (strcmp(buf2, "iso(1).org(3).dod(6).internet(1).private(4).enterprises(1)\n") == 0)
			continue;

		char *last = strrchr(buf2, '.');
		if (!last) {
			fprintf(stderr, "Error: corrupted line %d in %s\n", line, tmp_file);
			return 1;
		}
		last++;

		int items = itemcnt(buf2);
		int i;
		for (i = 0; i < items - 3; i++)
			printf("\t");
		printf(last);
	}

	//**************************************************************
	// continue printing the tree
	//**************************************************************
	printf("%s", buf);
	while (fgets(buf, BUFSIZE, fp1))
		printf("%s", buf);

	fclose(fp1);
	fclose(fp2);
	return 0;
}