#include "mainwidget.h"
#include "mainwidget_tfm.h"
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
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <TCP_controller/networkcontroller.h>
#include <QLineEdit>
#include <QLabel>

MainWindow::MainWindow(float s, float f, int l, int n, float e, std::string file, bool tfm, QWidget *parent)
	: tfm(tfm)
{
    //QMenuBar *menuBar = new QMenuBar;
    //QMenu *menuWindow = menuBar->addMenu(tr("&Window"));
    //QAction *addNew = new QAction(menuWindow);
    //addNew->setText(tr("Add new"));
    //menuWindow->addAction(addNew);
    //connect(addNew, SIGNAL(triggered()), this, SLOT(onAddNew()));
    //setMenuBar(menuBar);

    ///onAddNew();
	
    expSlider = createSlider();
    logFactorSlider = createSlider();
	QString speedStr;
	speedStr.setNum(s);
	speedBox = new QLineEdit(speedStr);
    speedLabel = new QLabel("Speed");
	e_sepButton = new QPushButton("&Change Element Seperation");
	e_sepButton->setCheckable(true);

	QHBoxLayout *container = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
	QVBoxLayout *controls1 = new QVBoxLayout;
	
	if(tfm){
		glWidgetTFM = new MainWidgetTFM(s, f, l, n, e, file, tfm);
		glWidgetTFM->setMinimumSize(400,600);
		container->addWidget(glWidgetTFM);
		connect(expSlider, SIGNAL(valueChanged(int)), glWidgetTFM, SLOT(setOverExposure(int)));
		connect(logFactorSlider, SIGNAL(valueChanged(int)), glWidgetTFM, SLOT(setLogFactor(int)));
		widgetPointer = glWidgetTFM;
	}
	else{
		glWidget = new MainWidget(s, f, l, n, e, file, tfm);
		//glWidget->setMinimumSize(400,600);
		glWidget->setMinimumSize(500,700);
		//glWidget->setBaseSize(400,600);
		container->addWidget(glWidget);
		connect(expSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setOverExposure(int)));
		connect(logFactorSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setLogFactor(int)));
		//connect(glWidget, SIGNAL(frameSwapped()), this, SLOT(finishedRendering()));
		connect(speedBox, SIGNAL(returnPressed()), this, SLOT(changeSpeed()));
		connect(e_sepButton, SIGNAL(pressed()), this, SLOT(toggle_e_sep()));
		widgetPointer = glWidget;
	}

	controls1->addWidget(speedLabel);
	controls1->addWidget(speedBox);
	controls1->addWidget(e_sepButton);

    container->addWidget(expSlider);
    container->addWidget(logFactorSlider);
	container->addLayout(controls1);
	//container->setStretchFactor(controls1, 0);
    //QWidget *w = new QWidget;
    //w->setLayout(container);
    //mainLayout->addWidget(w);
    //setLayout(mainLayout);
	setLayout(container);

    expSlider->setValue(50);
    logFactorSlider->setValue(50);

	nc = new NetworkController();
	nc->setSize(n, l);
	nc->initBuffer();

	connect(nc, SIGNAL(newData()), this, SLOT(newData()));
	sendSetup(f, l, n);

    setWindowTitle(tr("Sonograph"));
}

void MainWindow::toggle_e_sep(){
	glWidget->e_sepMode ^= 1; 
}

void MainWindow::changeSpeed(){
	int newSpeed = speedBox->text().toInt();
	glWidget->sampleSpeed = newSpeed;
	glWidget->update();
	speedBox->clearFocus();
}

void MainWindow::finishedRendering(){
	qDebug() << "Finished rendering";
}

void MainWindow::newData(){
	//qDebug() << ;
	//qDebug() << "New Data: " << QDateTime::currentMSecsSinceEpoch();
	glWidget->setNewTextureData(nc->dataBuffer);
	sendCal();
	
}

QSlider *MainWindow::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 200);
    slider->setSingleStep(100);
    slider->setPageStep(200);
    slider->setTickInterval(200);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void MainWindow::sendSetup(float f, int l, int n)
{
	
	QByteArray data("RST 25\r");
	//QByteArray data("");
	data.append("DXN 0 0\rDLYS 0 0\rSTS -1\rXXA 256\rDOF 1\rPRF 2000\rTXF 1 0 -1\r");
	
	for(int i = 0; i < n; i++){
		QByteArray txf = QByteArray("TXF 1 ");
		txf.append(QByteArray::number(i+1));
		txf.append(" 0\r");
		data.append(txf);
	}
	data.append("RXF 1 0 -1 0\r");
	for(int i = 0; i < n; i++){
		QByteArray rxf = QByteArray("RXF 1 ");
		rxf.append(QByteArray::number(i+1));
		rxf.append(" 0 0\r");
		data.append(rxf);
	}

	//\rTXF 1 1 0\rTXF 1 2 0\rTXF 1 3 0\rTXF 1 4 0\rTXF 1 5 0\rTXF 1 6 0\rTXF 1 7 0\rTXF 1 8 0\rTXF 1 9 0\rTXF 1 10 0\rTXF 1 11 0\rTXF 1 12 0\rTXF 1 13 0\rTXF 1 14 0\rTXF 1 15 0\rTXF 1 16 0\rTXF 1 17 0\rTXF 1 18 0\rTXF 1 19 0\rTXF 1 20 0\rTXF 1 21 0\rTXF 1 22 0\rTXF 1 23 0\rTXF 1 24 0\rTXF 1 25 0\rTXF 1 26 0\rTXF 1 27 0\rTXF 1 28 0\rTXF 1 29 0\rTXF 1 30 0\rTXF 1 31 0\rTXF 1 32 0\rTXF 1 33 0\rTXF 1 34 0\rTXF 1 35 0\rTXF 1 36 0\rTXF 1 37 0\rTXF 1 38 0\rTXF 1 39 0\rTXF 1 40 0\rTXF 1 41 0\rTXF 1 42 0\rTXF 1 43 0\rTXF 1 44 0\rTXF 1 45 0\rTXF 1 46 0\rTXF 1 47 0\rTXF 1 48 0\rTXF 1 49 0\rTXF 1 50 0\rTXF 1 51 0\rTXF 1 52 0\rTXF 1 53 0\rTXF 1 54 0\rTXF 1 55 0\rTXF 1 56 0\rTXF 1 57 0\rTXF 1 58 0\rTXF 1 59 0\rTXF 1 60 0\rTXF 1 61 0\rTXF 1 62 0\rTXF 1 63 0\rTXF 1 64 0\rRXF 1 0 -1 0\rRXF 1 1 0 0\rRXF 1 2 0 0\rRXF 1 3 0 0\rRXF 1 4 0 0\rRXF 1 5 0 0\rRXF 1 6 0 0\rRXF 1 7 0 0\rRXF 1 8 0 0\rRXF 1 9 0 0\rRXF 1 10 0 0\rRXF 1 11 0 0\rRXF 1 12 0 0\rRXF 1 13 0 0\rRXF 1 14 0 0\rRXF 1 15 0 0\rRXF 1 16 0 0\rRXF 1 17 0 0\rRXF 1 18 0 0\rRXF 1 19 0 0\rRXF 1 20 0 0\rRXF 1 21 0 0\rRXF 1 22 0 0\rRXF 1 23 0 0\rRXF 1 24 0 0\rRXF 1 25 0 0\rRXF 1 26 0 0\rRXF 1 27 0 0\rRXF 1 28 0 0\rRXF 1 29 0 0\rRXF 1 30 0 0\rRXF 1 31 0 0\rRXF 1 32 0 0\rRXF 1 33 0 0\rRXF 1 34 0 0\rRXF 1 35 0 0\rRXF 1 36 0 0\rRXF 1 37 0 0\rRXF 1 38 0 0\rRXF 1 39 0 0\rRXF 1 40 0 0\rRXF 1 41 0 0\rRXF 1 42 0 0\rRXF 1 43 0 0\rRXF 1 44 0 0\rRXF 1 45 0 0\rRXF 1 46 0 0\rRXF 1 47 0 0\rRXF 1 48 0 0\rRXF 1 49 0 0\rRXF 1 50 0 0\rRXF 1 51 0 0\rRXF 1 52 0 0\rRXF 1 53 0 0\rRXF 1 54 0 0\rRXF 1 55 0 0\rRXF 1 56 0 0\rRXF 1 57 0 0\rRXF 1 58 0 0\rRXF 1 59 0 0\rRXF 1 60 0 0\rRXF 1 61 0 0\rRXF 1 62 0 0\rRXF 1 63 0 0\rRXF 1 64 0 0\r
	data.append("TXN 256 1\rRXN 256 1\rSWP 1 256\rDIS 0\rDISS 0\rENA 256\rENAS 1\rGAN 0 160\rGANS 0 160\r");
	QByteArray dataLen = QByteArray::number(l);
	data.append("GAT 0 0 ");
	data.append(dataLen);
	data.append("\r");
	data.append("GATS 0 0 ");
	data.append(dataLen);
	data.append("\r");
	data.append("PAV 1 128 100\rPAW 1 128 80\rFRQS 0 4 1\rAWF 0 1\rAWFS 0 1\rAMP 0 13 0\rAMPS 0 13 0\rBUFF 0\r");
	nc->ignoreNext = true;
	nc->sendData(data);
}

void MainWindow::sendCal()
{
	QByteArray data = QByteArray("CALS 0\r");
	nc->sendData(data);

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	//                 |
	// Clean this mess v
	if(e->key() == Qt::Key_Tab) sendCal();
	if(tfm){
		switch(e->key()){
			case(Qt::Key_Escape):
			case(Qt::Key_Q):
				close();
				break;
			case(Qt::Key_J): 		glWidgetTFM->increaseC(); break;
			case(Qt::Key_K): 		glWidgetTFM->decreaseC(); break;
			case(Qt::Key_G): 		glWidgetTFM->increaseSigma(); break;
			case(Qt::Key_H): 		glWidgetTFM->decreaseSigma(); break;
			case(Qt::Key_E): 		glWidgetTFM->increaseF(); break;
			case(Qt::Key_R): 		glWidgetTFM->decreaseF(); break;
			case(Qt::Key_Space):	glWidgetTFM->toggleAScan(); break;
			case(Qt::Key_W): 		glWidgetTFM->viewUp(); break;
			case(Qt::Key_A): 		glWidgetTFM->viewLeft(); break;
			case(Qt::Key_S): 		glWidgetTFM->viewDown(); break;
			case(Qt::Key_D): 		glWidgetTFM->viewRight(); break;
			case(Qt::Key_Equal):	glWidgetTFM->scaleUp(); break;
			case(Qt::Key_Minus):	glWidgetTFM->scaleDown(); break;
			case(Qt::Key_M):		glWidgetTFM->layerUp(); break;
			case(Qt::Key_N):		glWidgetTFM->layerDown(); break;
			default:
				QWidget::keyPressEvent(e);
				break;
		}
	}
	else{
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
			case(Qt::Key_Space):	glWidget->toggleAScan(); break;
			case(Qt::Key_W): 		glWidget->viewUp(); break;
			case(Qt::Key_A): 		glWidget->viewLeft(); break;
			case(Qt::Key_S): 		glWidget->viewDown(); break;
			case(Qt::Key_D): 		glWidget->viewRight(); break;
			case(Qt::Key_Equal):	glWidget->scaleUp(); break;
			case(Qt::Key_Minus):	glWidget->scaleDown(); break;
			default:
				QWidget::keyPressEvent(e);
				break;
		}
	}
}

//void MainWindow::onAddNew()
//{
//    if (!centralWidget())
//        setCentralWidget(new Window(this));
//    else
//        QMessageBox::information(0, tr("Cannot add new window"), tr("Already occupied. Undock first."));
//}
