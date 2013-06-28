#include <QTextEdit>
#include "bundle.h"
#include "report.h"

void SystemBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrDeviceReport hrdevrep(version_, community_, port_, ip_);
	HrStorageReport hrstoragerep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>System Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1;
	rv += hrdevrep.get();
	rv += hrstoragerep.get();
	view->setText(rv);
}

void SoftwareBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrSoftwareReport hrsoftrep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Software Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1;
	rv += hrsoftrep.get();
	view->setText(rv);
}

void ProcessBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	HrProcessReport hrprocrep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Process Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1;
	rv += hrprocrep.get();
	view->setText(rv);
}

void InterfaceBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	InterfaceReport ifrep(version_, community_, port_, ip_);
	IfipReport ifip(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>Interface Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	QString rv2 = ifip.get();
	if (rv2.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1 + rv2;
	rv += ifrep.get();
	view->setText(rv);
}

void TcpipBundle::run(QTextEdit *view) {
	SystemReport sysrep(version_, community_, port_, ip_);
	InterfaceReport ifrep(version_, community_, port_, ip_);

	QString rv = "<br/><center><h1>TCP/IP Report</h1></center><br/><br/>\n";
	
	QString rv1 = sysrep.get();
	if (rv1.isEmpty()) {
		view->setText("Error: cannot connect to host\n");		
		return;
	}
	
	rv += rv1;
	rv += ifrep.get();
	view->setText(rv);
}