#include "mainwidget.h"
#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(float s, float f, int l, int n, float e, std::string file, QWidget *parent)
{
    //QMenuBar *menuBar = new QMenuBar;
    //QMenu *menuWindow = menuBar->addMenu(tr("&Window"));
    //QAction *addNew = new QAction(menuWindow);
    //addNew->setText(tr("Add new"));
    //menuWindow->addAction(addNew);
    //connect(addNew, SIGNAL(triggered()), this, SLOT(onAddNew()));
    //setMenuBar(menuBar);

    ///onAddNew();

	glWidget = new MainWidget(s, f, l, n, e, file);
	//glWidget = new MainWidget;
	glWidget->setMinimumSize(400,600);

    expSlider = createSlider();
    logFactorSlider = createSlider();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;
    container->addWidget(glWidget);
    container->addWidget(expSlider);
    container->addWidget(logFactorSlider);
    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);
    setLayout(mainLayout);

    expSlider->setValue(50);
    logFactorSlider->setValue(50);

    connect(expSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setOverExposure(int)));
    connect(logFactorSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setLogFactor(int)));

    setWindowTitle(tr("Sonograph"));
}


QSlider *MainWindow::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 200);
    slider->setSingleStep(1);
    slider->setPageStep(200);
    slider->setTickInterval(200);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	switch(e->key()){
		case(Qt::Key_Escape):
		case(Qt::Key_Q):
			close();
			break;
		case(Qt::Key_J): 		glWidget->increaseC(); break;
		case(Qt::Key_K): 		glWidget->decreaseC(); break;
		case(Qt::Key_G): 		glWidget->increaseSigma(); break;
		case(Qt::Key_H): 		glWidget->decreaseSigma(); break;
		case(Qt::Key_E): 		glWidget->increaseF(); break;
		case(Qt::Key_R): 		glWidget->decreaseF(); break;
		case(Qt::Key_Space): 	glWidget->toggleAScan(); break;
		case(Qt::Key_W): 		glWidget->viewUp(); break;
		case(Qt::Key_A): 		glWidget->viewLeft(); break;
		case(Qt::Key_S): 		glWidget->viewDown(); break;
		case(Qt::Key_D): 		glWidget->viewRight(); break;
		case(Qt::Key_Equal): 	glWidget->scaleUp(); break;
		case(Qt::Key_Minus): 	glWidget->scaleDown(); break;
		default:
			QWidget::keyPressEvent(e);
			break;
	}
}

//void MainWindow::onAddNew()
//{
//    if (!centralWidget())
//        setCentralWidget(new Window(this));
//    else
//        QMessageBox::information(0, tr("Cannot add new window"), tr("Already occupied. Undock first."));
//}
