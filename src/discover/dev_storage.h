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
#ifndef DEV_STORAGE
#define DEV_STORAGE
#include <QString>
#include <stdint.h>
#include "qtmib_ip.h"

struct DevStorage {
	static int instances_;
	uint32_t range_start_;
	uint32_t range_end_;
	QString ip_;
	QString version_;
	QString community_;
	QString port_;
	int timeout_;
	int remove_;
	DevStorage(): range_start_(0), range_end_(0),
		ip_(QString("")), version_(QString("")), community_(QString("")),
		port_(QString("")), timeout_(0), remove_(0) {
	
		instances_++;
	}
	
	DevStorage(const DevStorage *orig): timeout_(0), remove_(0) {
		char ip[30];
		sprintf(ip, "%d.%d.%d.%d", RCP_PRINT_IP(orig->range_start_));
		ip_ = QString(ip);
		version_ = orig->version_;
		community_ = orig->community_;
		port_ = orig->port_;
		range_start_ = orig->range_start_;
		range_end_ = orig->range_end_;
		instances_++;
	}

	~DevStorage() {
		instances_--;
	}		
};
#endif