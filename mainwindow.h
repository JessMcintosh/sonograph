#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
class QLineEdit;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class MainWidget;
class MainWidgetTFM;
class NetworkController;

//class MainWindow : public QMainWindow
class MainWindow : public QWidget
{
    Q_OBJECT

public:
	//MainWindow(QWidget *parent = 0);
    explicit MainWindow(float s, float f, int l, int n, float e, std::string file, bool tfm, QWidget *parent = 0);

public slots:
	void finishedRendering();
private:
	QSlider *expSlider;
	QSlider *logFactorSlider;
	MainWidget *glWidget;
	MainWidgetTFM *glWidgetTFM;
    QSlider *createSlider();
	void *widgetPointer;
	NetworkController *nc;
	QLineEdit *speedBox;
	QLabel *speedLabel;
	QPushButton *e_sepButton;

	void sendCal();
	void sendSetup(float f, int l, int n);
	bool tfm;
protected:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
	void newData();
	void changeSpeed();
	void toggle_e_sep();
};

#endif
