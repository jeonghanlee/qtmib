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
#ifndef TRANSACTIONTHREAD_H
#define TRANSACTIONTHREAD_H

#include <QImage>
#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QWaitCondition>
#include <QStringList>

#define MONITOR_TIMER 10
struct DevStorage;

class TransactionThread : public QThread
{
Q_OBJECT

public:
	TransactionThread();
	~TransactionThread();

	void addTransaction(DevStorage *dev);
	static void checkDevice(DevStorage *dev, TransactionThread *th);
signals:
	void transactionDone(const QString &message);
	void displayResult(const QString &message);

protected:
	void run();

private:
	QMutex mutex;

	int ending_; // exit flag
	int sock_;

	// input queues
	QList<DevStorage *> queue_add_;
};
#endif
