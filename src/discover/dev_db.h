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
	static void walk(void (*f)(DevStorage *dev, TransactionThread *th), TransactionThread *th);
	static void print();

private:
	static DevDb &get();
	QList<DevStorage *> list_;
};
#endif
