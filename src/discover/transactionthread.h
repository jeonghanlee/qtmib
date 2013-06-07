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
