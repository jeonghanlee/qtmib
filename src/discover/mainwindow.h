#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "transactionthread.h"

class QAction;
class QLabel;
class QMenu;
class QTableWidget;
class QLineEdit;
class QComboBox;

class MainWindow : public QMainWindow
{
Q_OBJECT

	public:
	MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void about();
	void transactionDone(const QString &message);
	void displayResult(const QString &message);
	void handleButton();

private:
	void createMenus();
	void addTransaction(QString type);
	void addInterfaces(QComboBox *net);

	TransactionThread thread;

	QMenu *fileMenu;
	QTableWidget *result_;
	QComboBox *network_;
	QComboBox *pBox_;
	QLineEdit *cBox_;
	QLineEdit *portBox_;
	
	QAction *exitAction;
	QAction *aboutAction;
};
#endif
