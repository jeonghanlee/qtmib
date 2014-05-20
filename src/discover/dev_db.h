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
#ifndef DEV_DB_H
#define DEV_DB_H

struct DevStorage;
class TransactionThread;
#include <QStringList>
#include <stdint.h>

class DevDb {
	DevDb();

public:
	static DevStorage *add(DevStorage *dev);
	static DevStorage *find(QString ip);
	static DevStorage *find(uint32_t ip);
	static void remove(QString ip);
	static int walk(void (*f)(DevStorage *dev, TransactionThread *th), TransactionThread *th);
	static void print();
	static int isEmpty();

private:
	static DevDb &get();
	QList<DevStorage *> list_;
};
#endif
