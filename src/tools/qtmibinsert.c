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
		fprintf(stderr, "Usage: qtmibinsert mibtree-file mib-name-before new-mib-file \n");
		return 1;
	}
	char *mib_before = argv[2];
	char *tree_file = argv[1];
	char *new_mib_file = argv[3];
	char *tmp_file = "tmpfile";
	char *tmp_file_out = "tmpfile.out";

	//**************************************************************
	// copy original tree until mib-before
	//**************************************************************
	FILE *fp1 = fopen(tree_file, "r");
	if (fp1 == NULL) {
		fprintf(stderr, "Error: cannot open %s\n", tree_file);
		return 1;
	}
	
	char buf[BUFSIZE];
	while (fgets(buf, BUFSIZE, fp1)) {
		if (strstr(buf, mib_before))
			break;
		
		printf("%s", buf);
	}

	//**************************************************************
	// copy the new mib
	//**************************************************************
	FILE *fp2 = fopen(new_mib_file, "r");
	if (fp2 == NULL) {
		fprintf(stderr, "Error: cannot open %s\n", new_mib_file);
		fclose(fp1);
		return 1;
	}
	
	char buf2[BUFSIZE];
	while (fgets(buf2, BUFSIZE, fp2))
		printf("%s", buf2);
	fclose(fp2);

	//**************************************************************
	// continue printing the tree
	//**************************************************************
	printf("%s", buf);
	while (fgets(buf, BUFSIZE, fp1))
		printf("%s", buf);

	fclose(fp1);
	return 0;
}