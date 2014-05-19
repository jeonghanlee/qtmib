/*
 * Copyright (C) 2013-2014 RCP100 Team (rcpteam@yahoo.com)
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
#include "db.h"
#include "qtmib_parse.h"

#define HMASK 0x3ff
DbEntry *db[HMASK + 1]; // 1024
bool initialized = false;

static void dbinit() {
	memset(db, 0, sizeof(db));
	initialized = true;
}

static unsigned hash(const char *str) {
	unsigned h = 5381;
	int c;
	while ((c = *str++) != '\0')
		h = ((h << 5) + h) ^ c; // hash * 33 ^ c

	return h & HMASK;
}

void dbAdd(DbEntry *dbe) {
	assert(dbe);
	if (!initialized)
		dbinit();
		
	unsigned h = hash(dbe->name);
	dbe->next = db[h];
	db[h] = dbe;
}

static int stack_guard = 0;
#define STACK_MAX 64
static DbEntry *dbFind(const char *name) {
	if (++stack_guard > STACK_MAX)
		return 0;
	
	assert(name);
	if (!initialized)
		dbinit();
		
	unsigned h = hash(name);
	DbEntry *ptr = db[h];
	while (ptr) {
		if (strcmp(ptr->name, name) == 0)
			return ptr;
		ptr = ptr->next;
	}
	
	return 0;
}

void dbClean() {
	int i;
	for (i = 0; i < (HMASK + 1); i++)
		if (db[i])
			delete db[i];
}

void dbBuildTree() {
	int i;
	
	// set level
	for (i = 0; i < (HMASK + 1); i++) {
		DbEntry *ptr = db[i];
		while (ptr) {
			stack_guard = 0;
			ptr->setLevel(dbFind);
			ptr = ptr->next;
		}
	}
	
	if (debug) {
		for (i = 0; i < (HMASK + 1); i++) {
			DbEntry *ptr = db[i];
			if (ptr) {
				int cnt = 0;
				while (ptr) {
					cnt++;
					ptr = ptr->next;
				}
				printf(" %d: %d ", i, cnt);
				ptr = db[i];
				while (ptr) {
					printf("(level %d, children %d, %s(%s), parent %s) ", ptr->level, ptr->chld_cnt, ptr->name, ptr->oid, ptr->base);
					ptr = ptr->next;
				}
				printf("\n");
			}
		}
	}
}

void dbPrint(DbEntry *top) {
	// print element
	stack_guard = 0;
	top->print(dbFind);
	printf("\n");

	// print children
	for (int i = 0; i < top->chld_cnt; i++)
		dbPrint(top->chld[i]);
	
#if 0
	for (int i = 0; i < (HMASK + 1); i++) {
		DbEntry *ptr = db[i];
		while (ptr) {
			stack_guard = 0;
			ptr->print(dbFind);
			printf("\n");
			ptr = ptr->next;
		}
	}
#endif

}
