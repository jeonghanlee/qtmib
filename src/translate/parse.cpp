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

static Token *parse_imports(Token *t, const char *fname) {
	assert(t);
	
	// advance until SEMICOLUMN
	while (t && t->type != Token::SEMICOLUMN)
		t = t->next;
	
	if (!t) {
		fprintf(stderr, "Error: IMPORTS should end with ; character, dropping file %s\n", fname);
		return 0;
	}
	return t->next;
}

static Token *parse_macro(Token *t, const char *fname) {
	assert(t);

	// advance until END
	while (t && t->type != Token::END)
		t = t->next;
	
	if (!t) {
		fprintf(stderr, "Error: MACRO should end with END, dropping file %s\n", fname);
		return 0;
	}
	return t->next;
}


static Token *remove_token(Token *prev, int cnt) {
	Token *current = prev->next;
	
	for (int i = 0; i < cnt; i++) {
		// remove current
		Token *next = current->next;
		current->next = 0;
		delete current;
		prev->next = next;
		current = next;
	}
	
	return prev;
}

// return false if invalid sequence
static bool clear_sequence(Token *start, Token *end) {
	// the sequence starts with equal & rightcb
	
	// check for name(number) sequences
	Token *seq = start->next->next;
	int state = 0;
	bool firstword = false;
	while (seq != end) {
		if (state == 0) {
			if (seq->type == Token::NUMBER) {
				seq = seq->next;
				continue;
			}
			else if (seq->type == Token::WORD) {
				state = 1;
				seq = seq->next;
				continue;
			}
		}
		else if (state == 1) {
			if (seq->type == Token::LEFTB) {
				state = 2;
				seq = seq->next;
				continue;
			}
			else if (seq->type == Token::RIGHTB)
				return false;
			else {
				state = 0;
				continue;
			}
		}
		else if (state == 2) {
			if (seq->type == Token::NUMBER) {
				state = 3;
				seq = seq->next;
				continue;
			}
			else
				return false;
		}
		else if (state == 3) {
			if (seq->type == Token::RIGHTB) {
				state = 0;
				seq = seq->next;
				continue;
			}
			else
				return false;
		}
	}
	if (state != 0)
		return false;

	Token *prev = start->next; // this is LEFTCB
	Token *current = start->next->next; // this is the first entry

	// clean the first word(number) sequence
	if (current->type == Token::WORD && current->next->type == Token::LEFTB) {
		prev = prev->next;

		// remove 3 tokens
		prev = remove_token(prev, 3);
		current = current->next;
	}
	else {
		current = current->next;
		prev = prev->next;
	}
	
	// remove names
	while (current != end) {
		if (current->type != Token::NUMBER) {
			if (debug) {
				printf("Debug: (sequence) removing ");
				current->print();			
			}
			// remove the token
			prev = remove_token(prev, 1);
			current = prev->next;
		}
		else {
			current = current->next;
			prev = prev->next;	
		}
	}

	// recognize and replace iso(1) org(3) dod(6) internet(1) private(4) enterprises(1) 1000 sequence
	// the sequence in this moment looks like this: iso 3 6 1 4 1
	prev = start->next; // this is LEFTCB
	current = start->next->next; // this is the first entry
	bool found = false;
	if (current->type == Token::WORD && strcmp(current->name, "iso") == 0)  {
		current = current->next;
		if (current->type == Token::NUMBER && strcmp(current->name, "3") == 0) {
			current = current->next;
			if (current->type == Token::NUMBER && strcmp(current->name, "6") == 0) {
				current = current->next;
				if (current->type == Token::NUMBER && strcmp(current->name, "1") == 0) {
					current = current->next;
					if (current->type == Token::NUMBER && strcmp(current->name, "4") == 0) {
						current = current->next;
						if (current->type == Token::NUMBER && strcmp(current->name, "1") == 0) {
							found = true;
						}
					}
				}
			}
		}
	}
	if (found) {
		current = start->next->next;
		delete [] current->name;
		current->name = new char[11 + 1];
		strcpy(current->name, "enterprises");
		remove_token(current, 5);
	}

	return true;
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
	if (!clear_sequence(t, tkn)) {
		fprintf(stderr, "Error: invalid OID sequence %s in MIB file %s (%d)\n",
			name->name, current_fname, __LINE__);
		return tkn;	
	}
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

// poarsing object definition
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

void parse(const char *fname) {
	if (!head)
		return;
	

	// look for WORD "BEGIN"
	Token *t = head;
	while (t) {
		if (t->type == Token ::WORD && strcmp(t->name, "BEGIN") == 0)
			break;
		t = t->next;	
	}
	if (!t) {
		fprintf(stderr, "Error: cannot find BEGIN, dropping file %s\n", fname);
		return;
	}
	t = t->next;
	
	while (t) {
		if (t->type == Token::IMPORTS)
			t = parse_imports(t, fname);
		else if (t->type == Token::MACRO)
			t = parse_macro(t, fname);
		else
			t = parse_def(t);
	}
}
