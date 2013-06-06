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
		f(dev, th);
	}
}

