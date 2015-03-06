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
#include <QtGui>
#if QT_VERSION >= 0x040700
#include <QElapsedTimer>
#else
#include <QTime>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "transactionthread.h"
#include "dev_storage.h"
#include "dev_db.h"
#include "../common/exec_prog.h"
#include "qtmib_socket.h"
#include "qtmib_discover.h"

TransactionThread::TransactionThread(): ending_(0), sock_(0) {
	start();
}


TransactionThread::~TransactionThread() {
	ending_ = 1;
	close(sock_);   
	wait();
}


void TransactionThread::addTransaction(DevStorage *dev) {
	QMutexLocker locker(&mutex);
	// allocate a new storage to be used in the new thread 
	DevStorage *newdev = new DevStorage(dev);
	queue_range_.append(newdev);
	if (dbg)
		printf("transaction started\n");
}

void TransactionThread::addTransactionResponse(DevStorage *dev) {
	QMutexLocker locker(&mutex);
	// allocate a new storage to be used in the new thread 
	DevStorage *newdev = new DevStorage(dev);
	queue_response_.append(newdev);
	if (dbg)
		printf("response transaction started\n");
}

static QString extract_string(QString line) {
	int index = line.indexOf(" = STRING: \"");
	if (index != -1) {
		index += 12;
		QString right = line.mid(index);
		index = right.indexOf("\"");
		if (index != -1)
			right.truncate(index); 
		return right;
	}
	
	return QString(" ");
}

static QString extract_timeticks(QString line) {
	int index = line.indexOf(" = Timeticks: ");
	if (index != -1) {
		index += 14;
		QString right = line.mid(index);
		index = right.indexOf(")");
		if (index != -1)
			right = right.mid(index + 2); 
		return right;
	}
	
	return QString(" ");
}

void TransactionThread::checkDeviceResponse(DevStorage *dev, TransactionThread *th) {
	time_t t = time(NULL);
	if (dbg)		
		printf("%u: send request to %s\n", (unsigned) t, dev->ip_.toStdString().c_str());

	int port = atoi(dev->port_.toStdString().c_str());
	int type = 1;
	if (dev->version_ == "v1")
		type = 0;
	
	// start timer and send reques
#if QT_VERSION >= 0x040700
	QElapsedTimer timer;
#else
	QTime timer;
#endif
	timer.start();
    	tx_packet(th->sock_, dev->range_start_,
		port, type, dev->community_.toStdString().c_str());
	
	// wait for the response	
	fd_set rfds;
	struct timeval tv;
	int retval;
	FD_ZERO(&rfds);
	FD_SET(th->sock_, &rfds);
	tv.tv_sec = 2; // maximum 2 seconds wait
	tv.tv_usec = 0;
	retval = select(th->sock_ + 1, &rfds, NULL, NULL, &tv);
	if (retval == -1) {
		perror("select");
		return;
	}
	
	bool sent = false;
	if (retval == 0) {
		if (dbg)
			printf("timeout\n");
	}
	else {
		// print response time
		uint32_t ip = rx_packet(th->sock_);
		if (ip == dev->range_start_) {
			int long long elapsed = timer.elapsed();
			if (dbg)
				printf("elapsed time %lld ms\n", elapsed);
			QString out = "response\t" + dev->ip_ + "\t ";
			if (elapsed == 0)
				out += " < 1 ms";
			else
				out += QString::number(elapsed) + " ms";
			emit th->displayResult(out);
			sent = true;
		}
	}
	
	if (!sent) {
		QString out = "response\t" + dev->ip_ + "\t timeout ";
		emit th->displayResult(out);
	}
}

void TransactionThread::checkDevice(DevStorage *dev, TransactionThread *th) {
	time_t t = time(NULL);
	
	// test exit flag	
	if (th->ending_)
		return;
		
	if (dev->timeout_ == 0 || dev->timeout_ == 20) {
		int port = atoi(dev->port_.toStdString().c_str());
		int type = 1;
		if (dev->version_ == "v1")
			type = 0;
			
		if (dbg)		
			printf("%u: send request to %s\n", (unsigned) t, dev->ip_.toStdString().c_str());
		tx_packet(th->sock_, dev->range_start_,
			port, type, dev->community_.toStdString().c_str());
	}
	
	else if (dev->timeout_ > 40) {
		QString cmd = "snmpbulkget -m \"\" ";
		if (dev->version_ == "v1")
			cmd = "snmpget -m \"\" ";
		cmd += "-" + dev->version_ + " ";
		cmd += "-c " + dev->community_ + " ";
		cmd += "-t 1 ";
		cmd += "-r 0 ";
		cmd += dev->ip_ + ":" + dev->port_ + " ";
		if (dev->version_ == "v1")
			cmd += ".1.3.6.1.2.1.1.5.0 2>&1";
		else
			cmd += ".1.3.6.1.2.1.1 2>&1";
	
		if (dbg)
			printf("%u: %s\n", (unsigned) t, cmd.toStdString().c_str());
			
		// execute command
		char *rv = exec_prog(cmd.toStdString().c_str());
		bool found = false;
		if (rv) {
			QString out = "add\t" + dev->ip_ + "\t";
			if (dev->version_ == "v1")
				 out = "add\t" + dev->ip_ + "\t \t";
			QString input = rv;
			QStringList lines = input.split( "\n", QString::SkipEmptyParts );
			foreach (QString line, lines) {
				if (line.startsWith("iso.3.6.1.2.1.1.4.0")) {
					out += extract_string(line) + "\t";
					found = true;
				}
				else if (line.startsWith("iso.3.6.1.2.1.1.5.0")) {
					out += extract_string(line) + "\t";
					found = true;
				}
				else if (line.startsWith("iso.3.6.1.2.1.1.6.0")) {
					out += extract_string(line) + "\t";
					found = true;
				}
				else if (line.startsWith("iso.3.6.1.2.1.1.3.0")) {
					out += extract_timeticks(line) + "\t";
					found = true;
				}
					
			}
			if (found) {
				emit th->displayResult(out);
			}
			free(rv);
		}
		dev->remove_ = 1;
	}

//	if (dbg)		
//		printf("%s %d\n", dev->ip_.toStdString().c_str(), dev->timeout_);
	if (dev->timeout_ == 40) {
		if (dbg)
			printf("%u: removing %s\n", (unsigned) t, dev->ip_.toStdString().c_str());
		dev->remove_ = 1;
	}
	dev->timeout_++;
}



#include <QMessageBox>
void TransactionThread::run() {
	int probes = 0;
	
	// pick up a random port above 1024
	srand(time(NULL));
	uint16_t port = (rand() % 30000) + 10000;  // between 10,000 and 40,000
	sock_ = rx_open(port);
	if (sock_ == 0) { // couldn't open port, try anotyher one
		port = (rand() % 30000) + 10000;  // between 10,000 and 40,000
		sock_ = rx_open(port);
	}
	if (sock_ == 0)
		return;

	forever {
		msleep(200);

		// test exit flag	
		if (ending_)
			break;

		mutex.lock();
		int addcnt = queue_range_.count();
		if (addcnt == 0) {
			int respcnt = queue_response_.count();
			if (respcnt && DevDb::isEmpty()) {
				DevStorage *dev = queue_response_.at(0);
				checkDeviceResponse(dev, this);
				if (dbg)
					printf("delete reponse transaction\n");
				queue_response_.removeFirst();
				delete dev;
			}
		}
		else {
			DevStorage *dev = queue_range_.at(0);
			
			uint32_t range_start = dev->range_start_;
			uint32_t range_end = dev->range_start_;
			for (int j = 0; j < 10; j++) {
				if (dev->range_start_ > dev->range_end_)
					break;

				DevStorage *newdev = new DevStorage(dev);
				newdev->range_end_ = newdev->range_start_;
				// add dev to device database
				DevDb::add(newdev);
				dev->range_start_++;
				range_end++;
			}
			
			char msg[100];
			if (dev->range_start_ > dev->range_end_) {
				// remove dev from input queue
				queue_range_.removeFirst();
				sprintf(msg, "Finishing...");
				if (dbg)
					printf("range removed\n");
				delete dev;
			}
			
			else 
				sprintf(msg, "Checking range %d.%d.%d.%d to %d.%d.%d.%d",
					RCP_PRINT_IP(range_start), RCP_PRINT_IP(range_end));

			emit transactionDone(QString(msg));
		}
		mutex.unlock();
		
		// check the socket
		uint32_t ip;
		while ((ip = rx_packet(sock_)) != 0) {
			// find the device and mark it for snmpbulkget
			DevStorage *dev = DevDb::find(ip);
			if (dev)
				dev->timeout_ = 41;
		}
		
		// retrieve data
		int current_probes = DevDb::walk(TransactionThread::checkDevice, this);
		if (current_probes > probes) {
			probes = current_probes;
			if (dbg)
				printf("Maximum %d IP probes active\n", probes);
		}
		
		// test exit flag	
		if (ending_)
			break;
	}
	
	close(sock_);
}


