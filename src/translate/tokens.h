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
#ifndef TOKENS_H
#define TOKENS_H
#include <stdio.h>
#include <assert.h>

struct Token {
	typedef enum token_type_t {
		START = 0,
		END,
		WORD,
		NUMBER,
		STRING,
		EQUAL,
		SEMICOLUMN,
		LEFTCB,
		LEFTB,
		RIGHTCB,
		RIGHTB,
		OBJECT,
		IDENTIFIER,
		OBJECT_TYPE,
		NOTIFICATION_TYPE,
		IMPORTS,
		SYNTAX,
		VALUE,
		MODULE_IDENTITY,
		OBJECT_IDENTITY,
		MACRO
	} TYPE;
	
	Token *next;
	TYPE type;
	char *name;
	
	Token(): next(0), type(START), name(0) {}
	~Token() {
		if (name)
			delete [] name;
		if (next)
			delete next;
	}
	
	void add(Token *t) {
		if (next == 0)
			next = t;
		else
			next->add(t);
	}
	
	void print() {
		switch (type) {
			case START:
				printf("START\n");
				break;
			case END:
				printf("END\n");
				break;
			case WORD:
				printf("WORD #%s#\n", name);
				break;
			case NUMBER:
				printf("NUMBER #%s#\n", name);
				break;
			case IMPORTS:
				printf("IMPORTS\n");
				break;
			case OBJECT:
				printf("OBJECT\n");
				break;
			case IDENTIFIER:
				printf("IDENTIFIER\n");
				break;
			case OBJECT_TYPE:
				printf("OBJECT_TYPE\n");
				break;
			case NOTIFICATION_TYPE:
				printf("NOTIFICATION_TYPE\n");
				break;
			case STRING:
				printf("STRING #%s#\n", name); 
				break;
			case EQUAL:
				printf("EQUAL\n");
				break;
			case SEMICOLUMN:
				printf("SEMICOLUMN\n");
				break;
			case LEFTCB:
				printf("LEFTCB\n");
				break;
			case LEFTB:
				printf("LEFTB\n");
				break;
			case RIGHTCB:
				printf("RIGHTCB\n");
				break;
			case RIGHTB:
				printf("RIGHTB\n");
				break;
			case SYNTAX:
				printf("SYNTAX\n");
				break;
			case VALUE:
				printf("VALUE\n");
				break;
			case MACRO:
				printf("MACRO\n");
				break;
			case MODULE_IDENTITY:
				printf("MODULE_IDENTITY\n");
				break;
			case OBJECT_IDENTITY:
				printf("OBJECT_IDENTITY\n");
				break;
			default:
				assert(0);
		}
		
	}

	void printall() {
		print();
		if (next)
			next->printall();
	}
};

extern Token *head;
const char *tokenize(const char *str);



#endif