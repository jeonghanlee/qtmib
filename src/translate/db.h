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
#ifndef DB_H
#define DB_H
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// the database is organized as a simple hash table 

struct DbEntry {
	DbEntry *next;
	typedef DbEntry *(*fptr_t)(const char *name);
	
	char *name;
	char *base;
	char *oid;
	int oid_number;
	
	// tree
	int level;
	bool top;
	DbEntry **chld;
	int chld_cnt;
	
	
	DbEntry(const char *n, const char *b, const char *o): next(0), level(0), top(false),
		chld(0), chld_cnt(0) {
		
		assert(n);
		assert(b);
		assert(o);
		
		name = new char[strlen(n) + 1];
		strcpy(name, n);
		base = new char[strlen(b) + 1];
		strcpy(base, b);
		oid = new char[strlen(o) + 1];
		strcpy(oid, o);
		oid_number = atoi(oid);
	}
	
	DbEntry(const char *n, const char *o): next(0), level(1), top(true),
		chld(0), chld_cnt(0) {
		
		assert(n);
		assert(o);
		
		name = new char[strlen(n) + 1];
		strcpy(name, n);
		oid = new char[strlen(o) + 1];
		strcpy(oid, o);
		oid_number = atoi(oid);
	}
	
	~DbEntry() {
		delete [] name;
		delete [] base;
		delete [] oid;
		if (chld_cnt)
			delete [] chld;
		if (next)
			delete next;
	}
	
	int setLevel(fptr_t find) {
		if (level)
			return level;
		
		DbEntry *parent = find(base);
		if (parent) {
			level = parent->setLevel(find) + 1;
			parent->setChild(this);
		}
		
		return level;
	}
	
	void setChild(DbEntry *child) {
		if (chld == 0) {
			chld = new DbEntry*[1];
			chld[0] = child;
			chld_cnt = 1;
			return;
		}
		
		DbEntry **tmp = chld;
		chld = new DbEntry*[chld_cnt + 1];

		// insert the child ordered by oid number
		int i;
		for (i = 0; i < chld_cnt; i++) {
			if (child->oid_number < tmp[i]->oid_number)
				break;
		}

		int position = i;
		for (i = 0; i < position; i++)
			chld[i] = tmp[i];
		chld[i] = child;
		i++;
		for (; i <= chld_cnt; i++)
			chld[i] = tmp[i - 1];		
		chld_cnt++;
		delete [] tmp;
	}
	
	void print(fptr_t find) {
		if (top)
			printf("%s(%s)", name, oid);
		else {
			DbEntry *parent = find(base);
			if (parent)
				parent->print(find);
			
			printf(".%s(%s)", name, oid);
		}
	}
	
	bool is_defined(fptr_t find) {
		if (top)
			return true;
		else {
			DbEntry *parent = find(base);
			if (parent)
				return parent->is_defined(find);
			else
				return false;
		}
	}
};

void dbAdd(DbEntry *dbe);
void dbClean();
void dbBuildTree();
void dbPrint(DbEntry *top);

#endif