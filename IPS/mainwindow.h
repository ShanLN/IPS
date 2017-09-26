#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>


class IPSMainwindow : public QMainWindow
{
	Q_OBJECT

public:
	IPSMainwindow(QWidget *parent = 0);
	~IPSMainwindow();

private:
	void setup();

private:
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *exitAction;


	QMenu *fileMenu;
	QMenu *editMenu;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;

};

#endif // MAINWINDOW_H
