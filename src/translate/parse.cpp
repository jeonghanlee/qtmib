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
#include <string.h>

int unknown_cnt = 0;

static Token *parse_imports(Token *t) {
	assert(t);
	
	// advance until SEMICOLUMN
	while (t && t->type != Token::SEMICOLUMN)
		t = t->next;
	
	if (!t)
		return 0;
	return t->next;
}

static Token *parse_macro(Token *t) {
	assert(t);

	// advance until END
	while (t && t->type != Token::END)
		t = t->next;
	
	if (!t)
		return 0;
	return t->next;
}

static void clear_sequence(Token *start, Token *end) {
	Token *prev = start->next->next;
	Token *current = start->next->next->next; // this should be a number

	while (current != end) {
		if (current->type != Token::NUMBER) {
			if (debug) {
				printf("Debug: (sequence) removing ");
				current->print();			
			}
			// remove the token
			Token *next = current->next;
			current->next = 0;
			delete current;
			prev->next = next;
			current = next;
		}
		else {
			current = current->next;
			prev = prev->next;	
		}
	}
}

static Token *parse_equal(Token *name, Token *t) {
	assert(name);
	assert(t);
	
	// mark the end of the sequence
	int tcnt = 1;
	Token *tkn = t;
	if (debug)
		printf("Debug: (sequence) token list:\n");		
	while (tkn && tkn->type != Token::RIGHTCB) {
		if (debug) {
			printf("Debug: (sequence) ");
			tkn->print();
		}
		tkn = tkn->next;
		tcnt++;
	}
	if (!tkn || tcnt < 5) {
		fprintf(stderr, "Error: invalid MIB file %s (%d)\n", current_fname, __LINE__);
		return 0;	
	}
	if (debug) {
		printf("Debug: (sequence) ");
		tkn->print();
	}


	// validate the start of the sequence
	if (t->next->type != Token::LEFTCB ||
	    t->next->next->type != Token::WORD) {
		fprintf(stderr, "Warning: skeeping entry %s in MIB file %s (%d)\n",
			name->name, current_fname, __LINE__);
		return tkn;	
	}
	
	// go trough the sequence and remove "words before numbers"
	// for example airentry(7) becomes simply 7
	clear_sequence(t, tkn);
	if (debug)
		printf("Debug: (sequence) token list after cleanup:\n");
	Token *tkn2 = t;
	tcnt = 1;
	while (tkn2 && tkn2->type != Token::RIGHTCB) {
		if (debug) {
			printf("Debug: (sequence) ");
			tkn2->print();
		}
		tkn2 = tkn2->next;
		tcnt++;
	}
	if (tcnt < 5) {
		fprintf(stderr, "Error: invalid MIB file %s\n", current_fname);
		return 0;	
	}
	
	if (t->next->next->next->type == Token::NUMBER) {
		if (debug) {
			printf("Debug: parsing %d ", __LINE__);
			name->print();
		}
		Token *base = t->next->next;
		Token *oid1 =  t->next->next->next;
	    
	    	if (tcnt == 5 && 
	    	    t->next->next->next->next->type == Token::RIGHTCB) {
			DbEntry *pdb = new DbEntry(name->name, base->name, oid1->name);
			dbAdd(pdb);
		}
	    	else if (tcnt == 6 &&
	    	    t->next->next->next->next->type == Token::NUMBER &&
	    	    t->next->next->next->next->next->type == Token::RIGHTCB) {
			Token *oid2 =  t->next->next->next->next;
			char oid1_name[20];
			sprintf(oid1_name, "unknown#%d", ++unknown_cnt);
			DbEntry *pdb = new DbEntry(name->name, oid1_name, oid2->name);
			dbAdd(pdb);
			pdb = new DbEntry(oid1_name, base->name, oid1->name);
			dbAdd(pdb);
		}
	    	else if (tcnt == 7 &&
	    	    t->next->next->next->next->type == Token::NUMBER &&
	    	    t->next->next->next->next->next->type == Token::NUMBER &&
	    	    t->next->next->next->next->next->next->type == Token::RIGHTCB) {
			Token *oid2 =  t->next->next->next->next;
			char oid1_name[20];
			sprintf(oid1_name, "unknown#%d", ++unknown_cnt);
			Token *oid3 =  t->next->next->next->next->next;
			char oid2_name[20];
			sprintf(oid2_name, "unknown#%d", ++unknown_cnt);
			
			DbEntry *pdb = new DbEntry(name->name, oid2_name, oid3->name);
			dbAdd(pdb);
			pdb = new DbEntry(oid2_name, oid1_name, oid2->name);
			dbAdd(pdb);
			pdb = new DbEntry(oid1_name, base->name, oid1->name);
			dbAdd(pdb);
		}
		else
			printf("Warning: unrecognized %s in file %s\n", name->name, current_fname);
	}
	else {
		printf("Warning: unrecognized %s in file %s\n", name->name, current_fname);
	}
	return tkn;
	
}


static Token *parse_def(Token *t) {
	assert(t);

	if (debug) {
		printf("Debug: parsing %d ", __LINE__);
		t->print();
	}
	
	Token *name = t;
	if (t->next == 0) {
		if (debug)
			printf("Debug: parsing %d\n", __LINE__);
		return 0;
	}
	if (!t->name || t->type != Token::WORD) {
		if (debug)
			printf("Debug: parsing %d\n", __LINE__);
		return t->next;
	}
		
	bool valid = false;		
	if (t->next->type == Token::OBJECT_TYPE)
		valid = true;
	else if (t->next->type == Token::OBJECT)
		valid = true;
	else if (t->next->type == Token::MODULE_IDENTITY)
		valid = true;
	else if (t->next->type == Token::OBJECT_IDENTITY)
		valid = true;
	else if (t->next->type == Token::NOTIFICATION_TYPE)
		valid = true;
			
	if (!valid) {
		if (debug) {
			printf("Debug: parsing %d ", __LINE__);
			name->print();
		}
		return t->next;
	}
	
	// advance to EQUAL LEFTCB WORD NUMBER RIGHTBRACKT
	// EQUALError
	while (t && t->type != Token::EQUAL)
		t = t->next;
	return parse_equal(name, t);
}	

void parse() {
	if (!head)
		return;
	

	// look for WORD "BEGIN"
	Token *t = head;
	while (t) {
		if (t->type == Token ::WORD && strcmp(t->name, "BEGIN") == 0)
			break;
		t = t->next;	
	}
	if (!t)
		return;
	t = t->next;
	
	while (t) {
		if (t->type == Token::IMPORTS)
			t = parse_imports(t);
		else if (t->type == Token::MACRO)
			t = parse_macro(t);
		else
			t = parse_def(t);
	}
}
