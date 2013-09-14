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

// the database is organized as a simple hash table 

struct DbEntry {
	DbEntry *next;
	typedef DbEntry *(*fptr_t)(const char *name);
	
	char *name;
	char *base;
	char *oid;
	bool top;
	DbEntry(const char *n, const char *b, const char *o): next(0), top(false) {
		assert(n);
		assert(b);
		assert(o);
		
		name = new char[strlen(n) + 1];
		strcpy(name, n);
		base = new char[strlen(b) + 1];
		strcpy(base, b);
		oid = new char[strlen(o) + 1];
		strcpy(oid, o);
	}
	DbEntry(const char *n, const char *o): next(0), top(true) {
		assert(n);
		assert(o);
		
		name = new char[strlen(n) + 1];
		strcpy(name, n);
		oid = new char[strlen(o) + 1];
		strcpy(oid, o);
	}
	~DbEntry() {
		delete [] name;
		delete [] base;
		delete [] oid;
		if (next)
			delete next;
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
void dbPrint();

#endif