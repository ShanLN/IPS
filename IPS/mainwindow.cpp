#include "mainwindow.h"
#include <qaction.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qfiledialog.h>
#include <qmdisubwindow.h>

#include "Imageview.h"

IPSMainwindow::IPSMainwindow(QWidget *parent)
	: QMainWindow(parent)
	, m_currSubWindow(0)
{
	setWindowTitle("IPS");
	setWindowIcon(QIcon("./Images/Icon/46.png"));
	setup();
	m_mdiarea = new QMdiArea();
	m_mdiarea->setViewMode(QMdiArea::TabbedView);
	m_mdiarea->setTabsClosable(true);
	m_mdiarea->setTabsMovable(true);
	setCentralWidget(m_mdiarea);
	connect(m_mdiarea, &QMdiArea::subWindowActivated, this, &IPSMainwindow::slot_activateChanged);

	m_dockImgInfo = new DockImgInfo(this);
	m_dockImgInfo->setAllowedAreas(Qt::LeftDockWidgetArea| Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, m_dockImgInfo);

	showMaximized();

	m_threadIO = new ThreadIO();
	connect(m_threadIO, &ThreadIO::signal_threadEnd, this, &IPSMainwindow::slot_threadIO);
}

IPSMainwindow::~IPSMainwindow()
{

}

void IPSMainwindow::setup()
{
	openAction = new QAction("open", this);
	openAction->setIcon(QIcon("./Images/Icon/35.png"));
	openAction->setStatusTip("open an image.");
	connect(openAction, &QAction::triggered, this, &IPSMainwindow::slot_openFile);
	
	saveAction = new QAction("save", this);
	saveAction->setIcon(QIcon("./Images/Icon/34.png"));
	saveAction->setStatusTip("save an image.");

	saveAsAction = new QAction("save as", this);
	saveAsAction->setIcon(QIcon("./Images/Icon/44.png"));
	saveAsAction->setStatusTip("save as an image.");

	exitAction = new QAction("exit", this);
	exitAction->setIcon(QIcon("./Images/Icon/40.png"));
	exitAction->setStatusTip("exit the software.");
	connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

	aboutAction = new QAction("about", this);
	aboutAction->setIcon(QIcon("./Images/Icon/52.png"));
	aboutAction->setStatusTip("information about the software.");

	//menu
	fileMenu = menuBar()->addMenu("file");
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu("edit");

	aboutMenu = menuBar()->addMenu("about");
	aboutMenu->addAction(aboutAction);

	//tool bar
	fileToolBar = addToolBar("file");
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	fileToolBar->addAction(saveAsAction);
	fileToolBar->addSeparator();
	fileToolBar->addAction(exitAction);

	editToolBar = addToolBar("edit");

}

Imageview *IPSMainwindow::getCurrentView()
{
	if (m_currSubWindow != NULL)
	{
		Imageview *currentView = static_cast<Imageview*>(m_currSubWindow->widget());
		return currentView;
	}
	else
		return NULL;
}

void IPSMainwindow::setImgInfo()
{
	//set the image information.
	Imageview *currentView = getCurrentView();
	if (currentView != NULL)
	{
		CImage *currCImg = currentView->getCImg();

		QStringList imgname;
		QString currImgPath = currCImg->getImgPath().c_str();
		QFileInfo fileInfo = QFileInfo(currImgPath);
		imgname.push_back(fileInfo.absoluteFilePath());
		imgname.push_back(fileInfo.completeBaseName());

		double imageInfo[2];
		imageInfo[0] = currCImg->getMean();
		imageInfo[1] = currCImg->getStd();

		m_dockImgInfo->setImgInfo(0, imageInfo, imgname);
	}
	else
	{
		m_dockImgInfo->setImgInfo(-1);
	}
}


void IPSMainwindow::slot_openFile()
{
	QString img_path = QFileDialog::getOpenFileName(this, tr("open an image."), ".",
		                                           tr("Images (*.jpg *.png *.bmp)"));

	m_threadIO->toDoOpenFile(img_path);
	m_pathVec.push_back(img_path);
}


void IPSMainwindow::slot_threadIO()
{
	QVector<CImage*> imgVec = m_threadIO->getImage();
	if (imgVec.empty())
		return;

	for (int i = 0; i < imgVec.size(); ++i)
	{
		CImage *tempCImg = imgVec.at(i);

		Imageview *imgview = new Imageview();
		imgview->setImage(tempCImg);

		QMdiSubWindow *subWindow = new QMdiSubWindow();
		subWindow->setWidget(imgview);
		subWindow->setAttribute(Qt::WA_DeleteOnClose);

		m_mdiarea->addSubWindow(subWindow);
	}
}


void IPSMainwindow::slot_activateChanged(QMdiSubWindow *subWindow)
{
	if (subWindow != m_currSubWindow)
	{
		m_currSubWindow = subWindow;
	}

	setImgInfo();
}