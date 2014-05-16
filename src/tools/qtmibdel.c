#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUFSIZE 1024

static inline int tabcnt(const char *str) {
	assert(str);
	int rv = 0;
	while (*str == '\t') {
		rv++;
		str++;
	}
	return rv;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: qtmibprint mibtree-file mib-name\n");
		return 1;
	}
	char *tree_file = argv[1];
	char *mib_name = argv[2];

	FILE *fp1 = fopen(tree_file, "r");
	if (fp1 == NULL) {
		fprintf(stderr, "Error: cannot open %s\n", tree_file);
		return 1;
	}
	
	char buf[BUFSIZE];
	int print = 1;
	int tabs = 0;
	while (fgets(buf, BUFSIZE, fp1)) {
		int tabset = 0;
		if (tabs == 0 && strstr(buf, mib_name)) {
			print = 0;
			tabs = tabcnt(buf);
			tabset = 1;
		}

		if (tabset == 0 && tabs == tabcnt(buf))
			print = 1;
		if (print)
			printf("%s", buf);
	}

	fclose(fp1);
	return 0;
}