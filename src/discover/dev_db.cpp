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
#include <stdio.h>
#include "dev_db.h"
#include "dev_storage.h"

DevDb::DevDb() {}

DevDb &DevDb::get() {
	static DevDb obj;

	return obj;
}

DevStorage *DevDb::add(DevStorage *dev) {
	DevStorage *mydev = DevDb::find(dev->ip_);
	
	if (mydev) {
		// modify device
		mydev->version_ = dev->version_;
		mydev->community_ = dev->community_;
		mydev->port_ = dev->port_;
		mydev->timeout_ = 0;
	}
	else {
		// add a new device
		DevDb::get().list_.append(dev);
	}
	
	return dev;
}

DevStorage *DevDb::find(QString ip) {
	QList<DevStorage *> &mylist = DevDb::get().list_;
	int cnt = mylist.count();
	for (int i  = 0; i < cnt; i++) {
		DevStorage *dev = mylist.at(i);
		if (ip == dev->ip_)
			return dev;
	}
	
	return 0;	
}

DevStorage *DevDb::find(uint32_t ip) {
	QList<DevStorage *> &mylist = DevDb::get().list_;
	int cnt = mylist.count();
	for (int i  = 0; i < cnt; i++) {
		DevStorage *dev = mylist.at(i);
		if (ip == dev->range_start_)
			return dev;
	}
	
	return 0;	
}

void DevDb::remove(QString ip) {
	QList<DevStorage *> &mylist = DevDb::get().list_;
	int cnt = mylist.count();
	for (int i  = 0; i < cnt; i++) {
		DevStorage *dev = mylist.at(i);
		if (ip == dev->ip_) {
			mylist.removeAt(i);
			delete dev;
			break;
		}
	}
}

void DevDb::print() {
	QList<DevStorage *> &mylist = DevDb::get().list_;
	int cnt = mylist.count();
	for (int i  = 0; i < cnt; i++) {
		DevStorage *dev = mylist.at(i);
		printf("%s\n", dev->ip_.toStdString().c_str());
	}
}

void DevDb::walk(void (*f)(DevStorage *dev, TransactionThread *th), TransactionThread *th) {
	QList<DevStorage *> &mylist = DevDb::get().list_;
	int cnt = mylist.count();
	for (int i  = 0; i < cnt; i++) {
		DevStorage *dev = mylist.at(i);
		if (dev) {
			f(dev, th);
			if (dev->remove_) {
				cnt--;
				mylist.removeAt(i);
				delete dev;
			}
		}
	}
}

