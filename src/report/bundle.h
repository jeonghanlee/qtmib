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

#ifndef BUNDLE_H
#define BUNDLE_H
#include <QString>
class QTextEdit;
class MainWindow;


// report bundles
class Bundle {
public:
	Bundle(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		version_(version), community_(community), port_(port), ip_(ip), timeout_(timeout), retries_(retries), mw_(0) {}
	virtual ~Bundle() {}
	
	void setHost(QString version, QString community, QString port, QString ip, QString timeout, QString retries) {
		version_ = version;
		community_ = community;
		port_ = port;
		ip_ = ip;
		timeout_ = timeout;
		retries_ = retries;
	}
	QString getVersion() {
		return version_;
	}
	QString getCommunity() {
		return community_;
	}
	void setCommunity(QString community) {
		community_ = community;
	}
	QString getPort() {
		return port_;
	}
	void setPort(QString port) {
		port_ = port;
	}
	QString getIp() {
		return ip_;
	}
	void setIp(QString ip) {
		ip_ = ip;
	}
	QString getTimeout() {
		return timeout_;
	}
	void setTimeout(QString timeout) {
		timeout_ = timeout;
	}
	QString getRetries() {
		return retries_;
	}
	void setRetries(QString retries) {
		retries_ = retries;
	}
	
	// template pattern
	void build(QTextEdit *view, MainWindow *mw);
	
private:
	virtual QString run() = 0;

protected:
	QString version_;
	QString community_;
	QString port_;
	QString ip_;
	QString timeout_;
	QString retries_;
	MainWindow *mw_;
};

class SystemBundle: public Bundle {
public:
	SystemBundle(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Bundle(version, community, port, ip, timeout, retries) {}
	~SystemBundle() {}
private:
	QString run();
};

class SoftwareBundle: public Bundle {
public:
	SoftwareBundle(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Bundle(version, community, port, ip, timeout, retries) {}
	~SoftwareBundle() {}
private:
	QString run();
};

class ProcessBundle: public Bundle {
public:
	ProcessBundle(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Bundle(version, community, port, ip, timeout, retries) {}
	~ProcessBundle() {}
private:
	QString run();
};

class InterfaceBundle: public Bundle {
public:
	InterfaceBundle(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Bundle(version, community, port, ip, timeout, retries) {}
	~InterfaceBundle() {}
	QString run();
};

class RouteBundle: public Bundle {
public:
	RouteBundle(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Bundle(version, community, port, ip, timeout, retries) {}
	~RouteBundle() {}
private:
	QString run();
};

class ConnectionBundle: public Bundle {
public:
	ConnectionBundle(QString version, QString community, QString port, QString ip, QString timeout, QString retries):
		Bundle(version, community, port, ip, timeout, retries) {}
	~ConnectionBundle() {}
private:
	QString run();
};

#endif