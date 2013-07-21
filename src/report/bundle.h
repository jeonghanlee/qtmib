/*
 * Copyright (C) 2013 RCP100 Team (rcpteam@yahoo.com)
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


// report bundles
class Bundle {
public:
	Bundle(QString version, QString community, QString port, QString ip):
		version_(version), community_(community), port_(port), ip_(ip) {}
	virtual ~Bundle() {}
	
	void setHost(QString version, QString community, QString port, QString ip) {
		version_ = version;
		community_ = community;
		port_ = port;
		ip_ = ip;
	}
	QString getVersion() {
		return version_;
	}
	QString getCommunity() {
		return community_;
	}
	QString getPort() {
		return port_;
	}
	QString getIp() {
		return ip_;
	}
	
	// template pattern
	void build(QTextEdit *view);
	
private:
	virtual QString run() = 0;

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
	~SystemBundle() {}
private:
	QString run();
};

class SoftwareBundle: public Bundle {
public:
	SoftwareBundle(QString version, QString community, QString port, QString ip):
		Bundle(version, community, port, ip) {}
	~SoftwareBundle() {}
private:
	QString run();
};

class ProcessBundle: public Bundle {
public:
	ProcessBundle(QString version, QString community, QString port, QString ip):
		Bundle(version, community, port, ip) {}
	~ProcessBundle() {}
private:
	QString run();
};

class InterfaceBundle: public Bundle {
public:
	InterfaceBundle(QString version, QString community, QString port, QString ip):
		Bundle(version, community, port, ip) {}
	~InterfaceBundle() {}
	QString run();
};

class RouteBundle: public Bundle {
public:
	RouteBundle(QString version, QString community, QString port, QString ip):
		Bundle(version, community, port, ip) {}
	~RouteBundle() {}
private:
	QString run();
};

class ConnectionBundle: public Bundle {
public:
	ConnectionBundle(QString version, QString community, QString port, QString ip):
		Bundle(version, community, port, ip) {}
	~ConnectionBundle() {}
private:
	QString run();
};

#endif