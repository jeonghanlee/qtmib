#ifndef BUNDLE_H
#define BUNDLE_H
#include <QString>
class QTextEdit;


// report bundles
class Bundle {
public:
	Bundle(QString version, QString community, QString port, QString ip):
		version_(version), community_(community), port_(port), ip_(ip) {}
	virtual void run(QTextEdit *view) = 0;
protected:	
	QString version_;
	QString community_;
	QString port_;
	QString ip_;
};

class SystemBundle: public Bundle {
public:
	SystemBundle(QString version, QString community, QString port, QString ip):
		Bundle(version, community, port, ip) {}
	void run(QTextEdit *view);
};

class SoftwareBundle: public Bundle {
public:
	SoftwareBundle(QString version, QString community, QString port, QString ip):
		Bundle(version, community, port, ip) {}
	void run(QTextEdit *view);
};

class ProcessBundle: public Bundle {
public:
	ProcessBundle(QString version, QString community, QString port, QString ip):
		Bundle(version, community, port, ip) {}
	void run(QTextEdit *view);
};

#endif