#ifndef DEV_STORAGE
#define DEV_STORAGE
#include <QString>

struct DevStorage {
	QString ip_;
	QString version_;
	QString community_;
	QString port_;
	int timeout_;
	DevStorage():
		ip_(QString("")), version_(QString("")), community_(QString("")),
		port_(QString("")), timeout_(0) {}
};
#endif