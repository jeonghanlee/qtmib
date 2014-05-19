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
#include "qtmib_parse.h"
#include "tokens.h"
#include <ctype.h>
#include <string.h>

Token *head = 0;
static Token *last_token = 0;

const char *tokenize(const char *str) {
	assert(str);
	
	if (!head) {
		head = new Token;
		head->type = Token::START;
		last_token = head;
	}

	const char *ptr = str;
	
	// skeep leading blanks
	while (*str != '\0' && (isspace(*str) || iscntrl(*str)))
		str++;
	
	// skeep comments
	if (*str == '-' && *(str + 1) == '-') {
		// advance to the end of the line
		while (*str != '\0' && *str != '\n' && *str != '\r')
			str++;
		return str;
	}
	
	Token *newtoken = new Token;
	last_token->add(newtoken);
	last_token = newtoken;
	
	if (*str == '\0') {
		newtoken->type = Token::END;
		return str;
	}
	
	if (*str == '{') {
		newtoken->type = Token::LEFTCB;
		return ++str;
	}
	if (*str == '(') {
		newtoken->type = Token::LEFTB;
		return ++str;
	}
		
	if (*str == '}') {
		newtoken->type = Token::RIGHTCB;
		return ++str;
	}
	if (*str == ')') {
		newtoken->type = Token::RIGHTB;
		return ++str;
	}
		
	if (*str == ';') {
		newtoken->type = Token::SEMICOLUMN;
		return ++str;
	}
	
	if (*str =='"') {
		str++;
		int cnt = 1;
		while (*str != '"' && *str != '\0') {
			str++;
			cnt++;
		}
		if (*str == '\0') {
			newtoken->type = Token::END;
			return str;
		}
		cnt++;
		str++;
		newtoken->type = Token::STRING;
		char *newc = new char[cnt + 1];
		memcpy(newc, str - cnt, cnt);
		newc[cnt] = '\0';
		newtoken->name = newc;
		return ++str;
	}
	
	if (*str == ':' && *(str + 1) == ':' && *(str + 2) == '=') { 
		newtoken->type = Token::EQUAL;
		return str + 3;
	}
	
	// this is a word
	int cnt = 1;
	str++;
	while (*str != '\0' && !isspace(*str) && !iscntrl(*str) &&
	          *str != ':' && *str != ';' && *str != '{' && *str  != '}' && *str != '(' && *str  != ')' ) {
	          	str++;
	          	cnt++;
	}
	
	if (*str == '\0') {
		newtoken->type = Token::END;
		return str;
	}
	newtoken->type = Token::WORD;
	char *newc = new char[cnt + 1];
	memcpy(newc, str - cnt, cnt);
	newc[cnt] = '\0';
	newtoken->name = newc;
	
	// if the word is composed only of numbers, change the type
	char *s = newtoken->name;
	while (*s != '\0') {
		if (!isdigit(*s))
			break;
		s++;
	}
	if (*s == '\0')
		newtoken->type = Token::NUMBER;
	
	// objects
	else if (strcmp(newtoken->name, "OBJECT") == 0)
		newtoken->type = Token::OBJECT;
	else if (strcmp(newtoken->name, "END") == 0)
		newtoken->type = Token::END;
	else if (strcmp(newtoken->name, "IDENTIFIER") == 0)
		newtoken->type = Token::IDENTIFIER;
	else if (strcmp(newtoken->name, "OBJECT-TYPE") == 0)
		newtoken->type = Token::OBJECT_TYPE;
	else if (strcmp(newtoken->name, "NOTIFICATION-TYPE") == 0)
		newtoken->type = Token::NOTIFICATION_TYPE;
	else if (strcmp(newtoken->name, "IMPORTS") == 0)
		newtoken->type = Token::IMPORTS;
	else if (strcmp(newtoken->name, "SYNTAX") == 0)
		newtoken->type = Token::SYNTAX;
	else if (strcmp(newtoken->name, "VALUE") == 0)
		newtoken->type = Token::VALUE;
	else if (strcmp(newtoken->name, "MACRO") == 0)
		newtoken->type = Token::MACRO;
	else if (strcmp(newtoken->name, "MODULE-IDENTITY") == 0)
		newtoken->type = Token::MODULE_IDENTITY;
	else if (strcmp(newtoken->name, "OBJECT-IDENTITY") == 0)
		newtoken->type = Token::OBJECT_IDENTITY;
	
	return str;
}
