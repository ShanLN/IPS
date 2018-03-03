#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <qmdiarea.h>


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

	QAction *aboutAction;


	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *aboutMenu;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;

	QMdiArea *m_mdiarea;

private slots:
    void slot_openFile();


};

#endif // MAINWINDOW_H
