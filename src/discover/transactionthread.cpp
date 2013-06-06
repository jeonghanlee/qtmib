#include <QtGui>

#include "transactionthread.h"
#include "dev_storage.h"
#include "dev_db.h"
#include "exec_prog.h"

TransactionThread::TransactionThread(): ending_(0) {
	start();
}


TransactionThread::~TransactionThread() {
	ending_ = 1;
	wait();
}


void TransactionThread::addTransaction(DevStorage *dev) {
	QMutexLocker locker(&mutex);
	queue_add_.append(dev);
}

void TransactionThread::delTransaction(DevStorage *dev) {
	QMutexLocker locker(&mutex);
	queue_del_.append(dev);
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
	if (--dev->timeout_ <= 0) {
		dev->timeout_ = MONITOR_TIMER;
		
		QString cmd = "snmpbulkget -m \"\" ";
		cmd += "-" + dev->version_ + " ";
		cmd += "-c " + dev->community_ + " ";
		cmd += "-t 1 ";
		cmd += "-r 0 ";
		cmd += dev->ip_ + ":" + dev->port_ + " ";
		cmd += ".1.3.6.1.2.1.1 2>&1";
		// execute command
		char *rv = exec_prog(cmd.toStdString().c_str());
		if (rv) {
			QString out = "add\t" + dev->ip_ + "\t";
			QString input = rv;
			QStringList lines = input.split( "\n", QString::SkipEmptyParts );
			foreach (QString line, lines) {
				if (line.startsWith("iso.3.6.1.2.1.1.4.0"))
					out += extract_string(line) + "\t";
				else if (line.startsWith("iso.3.6.1.2.1.1.5.0"))
					out += extract_string(line) + "\t";
				else if (line.startsWith("iso.3.6.1.2.1.1.6.0"))
					out += extract_string(line) + "\t";
printf("out %s\n", out.toStdString().c_str());
					
			}
			emit th->displayResult(out);
			free(rv);
		}
		
	}
		
	printf("%s %d\n", dev->ip_.toStdString().c_str(), dev->timeout_);
}





void TransactionThread::run() {

	forever {
		sleep(1);

		mutex.lock();
		int addcnt = queue_add_.count();
		int delcnt = queue_del_.count();
		if (addcnt == 0 && delcnt == 0)
			printf("sleep\n");
		else {
			for (int i = 0; i < addcnt; i++) {
				DevStorage *dev = queue_add_.at(0);
				printf("sleep; add ip %s\n", dev->ip_.toStdString().c_str());
				emit transactionDone(dev->ip_ + " added");
				// remove dev from input queue
				queue_add_.removeFirst();
				// add dev to device database
				DevDb::add(dev);
				// display result
				QString result = "add\t" + dev->ip_;
				emit displayResult(result);
				
			}
			for (int i = 0; i < delcnt; i++) {
				DevStorage *dev = queue_del_.at(0);
				printf("sleep; del ip %s\n", dev->ip_.toStdString().c_str());
				emit transactionDone(dev->ip_ + " removed");
				QString ip = dev->ip_;
				delete dev;
				// remove dev from input queue
				queue_del_.removeFirst();
				// remove device from database
				DevDb::remove(ip);
				// display result
				QString result = "del\t" + ip;
				emit displayResult(result);
			}
		}
		mutex.unlock();
		
		// retrieve data
		DevDb::walk(TransactionThread::checkDevice, this);
		
		// test exit falg	
		if (ending_)
			break;
	}
}


