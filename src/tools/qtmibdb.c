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

// Original copyright notice form net-snmp project

/*
 * mib.c
 *
 * $Id$
 *
 * Update: 1998-07-17 <jhy@gsu.edu>
 * Added print_oid_report* functions.
 *
 */
/* Portions of this file are subject to the following copyrights.  See
 * the Net-SNMP's COPYING file for more details and other copyrights
 * that may apply:
 */
/**********************************************************************
	Copyright 1988, 1989, 1991, 1992 by Carnegie Mellon University

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of CMU not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.
******************************************************************/
/*
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * Use is subject to license terms specified in the COPYING file
 * distributed with the Net-SNMP package.
 */


#include "net-snmp/net-snmp-config.h"
#include "net-snmp/net-snmp-includes.h"

static int print_subtree_oid_report_suffix = 1;
static int print_subtree_oid_report_labeledoid = 0;
static int print_subtree_oid_report_symbolic = 0;
static int print_subtree_oid_report_mibchildoid = 0;
static int print_subtree_oid_report_oid = 0;

static void
print_parent_labeledoid(FILE * f, struct tree *tp)
{
    if (tp) {
        if (tp->parent) {
            print_parent_labeledoid(f, tp->parent);
         /*RECURSE*/}
        fprintf(f, ".%s(%lu)", tp->label, tp->subid);
    }
}

static void
print_parent_oid(FILE * f, struct tree *tp)
{
    if (tp) {
        if (tp->parent) {
            print_parent_oid(f, tp->parent);
         /*RECURSE*/}
        fprintf(f, ".%lu", tp->subid);
    }
}

static void print_parent_mibchildoid(FILE * f, struct tree *tp)
{
    static struct tree *temp;
    unsigned long elems[100];
    int elem_cnt = 0;
    int i = 0;
    temp = tp;
    if (temp) {
        while (temp->parent) {
                elems[elem_cnt++] = temp->subid;
                temp = temp->parent;
        }
        elems[elem_cnt++] = temp->subid;
    }
    for (i = elem_cnt - 1; i >= 0; i--) {
        if (i == elem_cnt - 1) {
            fprintf(f, "%lu", elems[i]);           
            } else {
            fprintf(f, ".%lu", elems[i]);          
        }
    }
}

static void
print_parent_label(FILE * f, struct tree *tp)
{
    if (tp) {
        if (tp->parent) {
            print_parent_label(f, tp->parent);
         /*RECURSE*/}
        fprintf(f, ".%s", tp->label);
    }
}

static void
print_subtree_oid_report(FILE * f, struct tree *tree, int count) {
	struct tree    *tp;

	count++;

	/*
	 * sanity check
	 */
	if (!tree) {
		return;
	}

	/*
	 * find the not reported peer with the lowest sub-identifier.
	 * if no more, break the loop and cleanup.
	 * set "reported" flag, and create report for this peer.
	 * recurse using the children of this peer, if any.
	 */
	while (1) {
		register struct tree *ntp;

		tp = NULL;
		for (ntp = tree->child_list; ntp; ntp = ntp->next_peer) {
			if (ntp->reported)
				continue;

			if (!tp || (tp->subid > ntp->subid))
				tp = ntp;
		}
		if (!tp)
			break;

		tp->reported = 1;

		if (print_subtree_oid_report_labeledoid) {
			print_parent_labeledoid(f, tp);
			fprintf(f, "\n");
		}
		if (print_subtree_oid_report_oid) {
			print_parent_oid(f, tp);
			fprintf(f, "\n");
		}
		if (print_subtree_oid_report_symbolic) {
			print_parent_label(f, tp);
			fprintf(f, "\n");
		}
		if (print_subtree_oid_report_mibchildoid) {
			fprintf(f, "\"%s\"\t", tp->label);
			fprintf(f, "\t\t\"");
			print_parent_mibchildoid(f, tp);
			fprintf(f, "\"\n");
		}
		if (print_subtree_oid_report_suffix) {
			int             i;
			for (i = 0; i < count; i++)
				fprintf(f, "\t");
			fprintf(f, "%s", tp->label);
#if 0				
			fprintf(f, "%s(%ld) type=%d", tp->label, tp->subid, tp->type);
			if (tp->tc_index != -1)
				fprintf(f, " tc=%d", tp->tc_index);
			if (tp->hint)
				fprintf(f, " hint=%s", tp->hint);
			if (tp->units)
				fprintf(f, " units=%s", tp->units);
#endif
			fprintf(f, "(%lu)", tp->subid);
			fprintf(f, "\n");
		}
		print_subtree_oid_report(f, tp, count);
		/*RECURSE*/
	}
}

struct tree* find_internet(struct tree *tree) {
	struct tree    *tp;
	for (tp = tree->child_list; tp; tp = tp->next_peer) {
		if (tp->child_list) {
			if (strcmp(tp->label, "internet") == 0)
				return tp;
			return find_internet(tp);
		}
	}
	return NULL;
}

int main(int argc, char ** argv) {
	char buff[100];
	struct tree *node=NULL;

	init_mib();
	node=read_all_mibs();
	if( node == NULL ) {
		exit(2);
	}
	
	struct tree *internet = find_internet(node);
	if (internet == NULL) {
		fprintf(stderr, "Error: invalid MIB tree, cannot find .iso.org.dod.internet\n");
		return 1;
	}
	fprintf(stdout, "iso.org.dod.internet(1.3.6.1)\n");
	print_subtree_oid_report(stdout, internet, 0);
	//	print_s(node);
}
