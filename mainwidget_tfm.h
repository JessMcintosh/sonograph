#ifndef MAINWIDGET_TFM_H
#define MAINWIDGET_TFM_H

#include "geometryengine.h"
#include "mainwidget.h"

#include <QGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLPixelTransferOptions> 

#include <fftw3.h>
#include <math.h>
#include <limits>

class GeometryEngine;

class MainWidgetTFM : public MainWidget
{
	using MainWidget::MainWidget;

public:
	//using MainWidget::initArrays;
	void initArrays();
};
/*
class MainWidgetTFM : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:
    explicit MainWidgetTFM(QWidget *parent = 0);
	explicit MainWidgetTFM(float s, float f, int l, int n, float e, std::string file, bool tfm, QWidget *parent = 0);
	void increaseSigma() { gauss_sigma+=1.0; qDebug() << "New Sigma: " << gauss_sigma; initTextures(); updateGL(); }
	void decreaseSigma() { gauss_sigma-=1.0; qDebug() << "New Sigma: " << gauss_sigma; initTextures(); updateGL(); }
	void increaseC() { gauss_c+=1.0; qDebug() << "New c: " << gauss_c; initTextures(); updateGL(); }
	void decreaseC() { gauss_c-=1.0; qDebug() << "New c: " << gauss_c; initTextures(); updateGL(); }
	void increaseF() { samplingFrequency+=25000000.0; qDebug() << "New frequency: " << samplingFrequency/1000000.0; updateGL(); }
	void decreaseF() { samplingFrequency-=25000000.0; qDebug() << "New frequency: " << samplingFrequency/1000000.0;updateGL(); }
	void toggleAScan() { a_scan ^= 1; updateGL(); }
	void viewUp() { y_translation += 0.01; updateGL(); }
	void viewDown() { y_translation -= 0.01; updateGL(); }
	void viewLeft() { x_translation -= 0.01; updateGL(); }
	void viewRight() { x_translation += 0.01; updateGL(); }
	void scaleUp(){  viewScale += 0.1; updateGL(); }
	void scaleDown(){  viewScale -= 0.1; updateGL(); }

    ~MainWidget();

	float samplingFrequency;
	int sampleSize;
	float sampleSpeed;
	int numElements;
	float centreFrequency;
	std::string filename;

public slots:
	void setOverExposure(int val);
	void setLogFactor(int val);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);
	void wheelEvent(QWheelEvent *event);

	void initArrays();
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initTextures();

	void filterFFT();
	void createGaussian();

	size_t index( int x, int y ) const { return x + sampleSize * y; }

private:
    QBasicTimer timer;
    QGLShaderProgram program;
    GeometryEngine geometries;

	QOpenGLTexture *texture;
    //GLuint texture_r;
    //GLuint texture_i;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
	float e_sep;
	float overexposure;
	float logfactor;
	float gauss_sigma;
	float gauss_c;
	float viewScale;
	bool a_scan;
	bool tfm;

	float x_translation;
	float y_translation;

	double *m_data;
	float *m_filtered_data_ri;
	fftw_complex *in, *out;
	double *magnitude;
	double *gaussFilter;

};
*/

#endif // MAINWIDGET_TFM_H
