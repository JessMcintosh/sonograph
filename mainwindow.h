#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class MainWidget;
class MainWidgetTFM;

//class MainWindow : public QMainWindow
class MainWindow : public QWidget
{
    Q_OBJECT

public:
	//MainWindow(QWidget *parent = 0);
    explicit MainWindow(float s, float f, int l, int n, float e, std::string file, bool tfm, QWidget *parent = 0);
private:
	QSlider *expSlider;
	QSlider *logFactorSlider;
	MainWidget *glWidget;
	MainWidgetTFM *glWidgetTFM;
    QSlider *createSlider();
protected:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
};

#endif
