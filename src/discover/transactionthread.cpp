#include <QtGui>

#include "transactionthread.h"
#include "dev_storage.h"
#include "dev_db.h"
#include "exec_prog.h"
#include "qtmib_socket.h"

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
	queue_add_.append(dev);
}

static QString extract_string(QString line) {
printf("extract from %s\n", line.toStdString().c_str());
	int index = line.indexOf(" = STRING: \"");
	if (index != -1) {
		index += 12;
		QString left = line.mid(index);
		index = left.indexOf("\"");
		if (index != -1)
			left.truncate(index); 
printf("extracted %s\n", left.toStdString().c_str());
		return left;
	}
	
	return QString(" ");
}

void TransactionThread::checkDevice(DevStorage *dev, TransactionThread *th) {
	// test exit falg	
	if (th->ending_)
		return;
		
	if (dev->timeout_ == 0 || dev->timeout_ == 20) {
		int port = atoi(dev->port_.toStdString().c_str());
		int type = 1;
		if (dev->version_ == "v1")
			type = 0;
			
		tx_packet(th->sock_, dev->range_start_,
			port, type, dev->community_.toStdString().c_str());
	}
	
	else if (dev->timeout_ > 40) {
		QString cmd = "snmpbulkget -m \"\" ";
		cmd += "-" + dev->version_ + " ";
		cmd += "-c " + dev->community_ + " ";
		cmd += "-t 1 ";
		cmd += "-r 0 ";
		cmd += dev->ip_ + ":" + dev->port_ + " ";
		cmd += ".1.3.6.1.2.1.1 2>&1";
		// execute command
		char *rv = exec_prog(cmd.toStdString().c_str());
		bool found = false;
		if (rv) {
			QString out = "add\t" + dev->ip_ + "\t";
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
					
			}
			if (found) {
printf("out %s\n", out.toStdString().c_str());
				emit th->displayResult(out);
			}
			free(rv);
		}
		dev->remove_ = 1;
	}
		
//	printf("%s %d\n", dev->ip_.toStdString().c_str(), dev->timeout_);
	if (dev->timeout_ == 40)
		dev->remove_ = 1;
	dev->timeout_++;
}





void TransactionThread::run() {
	sock_ = rx_open(29456);

	forever {
		msleep(200);

		// test exit falg	
		if (ending_)
			break;

		mutex.lock();
		int addcnt = queue_add_.count();
		if (addcnt == 0)
;//			printf("sleep\n");
		else {
			for (int i = 0; i < addcnt; i++) {
				DevStorage *dev = queue_add_.at(0);
//				printf("sleep; add ip %s\n", dev->ip_.toStdString().c_str());

				for (int j = 0; j < 5; j++) {
					if (dev->range_start_ > dev->range_end_)
						break;

					DevStorage *newdev = new DevStorage(dev);
					// add dev to device database
					DevDb::add(newdev);
					dev->range_start_++;
				}
				
				if (dev->range_start_ > dev->range_end_)
					// remove dev from input queue
					queue_add_.removeFirst();

				emit transactionDone("adding range to processing queue");
				// display result
//				QString result = "add\t" + dev->ip_;
//				emit displayResult(result);
			}
		}
		mutex.unlock();
		
		// check the socket
		uint32_t ip;
		while ((ip = rx_packet(sock_)) != 0) {
printf("*** ip %d.%d.%d.%d ***\n", RCP_PRINT_IP(ip));
			// find the device and mark it for snmpbulkget
			DevStorage *dev = DevDb::find(ip);
			if (dev)
				dev->timeout_ = 41;
		}
		
		// retrieve data
		DevDb::walk(TransactionThread::checkDevice, this);
		
		// test exit falg	
		if (ending_)
			break;
	}
	
	close(sock_);
}


