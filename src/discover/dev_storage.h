#ifndef DEV_STORAGE
#define DEV_STORAGE
#include <QString>
#include <stdint.h>
#include "qtmib_ip.h"

struct DevStorage {
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
		port_(QString("")), timeout_(0), remove_(0) {}
	DevStorage(const DevStorage *orig): timeout_(0), remove_(0) {
		char ip[30];
		sprintf(ip, "%d.%d.%d.%d", RCP_PRINT_IP(orig->range_start_));
		ip_ = QString(ip);
		version_ = orig->version_;
		community_ = orig->community_;
		port_ = orig->port_;
		range_start_ = orig->range_start_;
		range_end_ = orig->range_start_;
	}		
};
#endif