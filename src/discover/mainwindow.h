#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "transactionthread.h"

class QAction;
class QLabel;
class QMenu;
class QTableWidget;

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

private:
	void createMenus();
	void addTransaction(QString type);

	TransactionThread thread;

	QMenu *fileMenu;
	QTableWidget *result_;

	QAction *exitAction;
	QAction *aboutAction;
};
#endif
