#include "mainwidget_tfm.h"

#include <QMouseEvent>

#include <math.h>
#include <locale.h>
#include <iostream>
#include <QPixmap>
#include <sstream>
#include <vector>
#include <fstream>
#include <iterator>
#include <QDebug>


void MainWidgetTFM::initializeGL()
{
	layer = 0;
	initArrays();

    initializeGLFunctions();
    qglClearColor(Qt::black);
    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    geometries.init();

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

void MainWidgetTFM::initArrays()
{
	magnitude = new double[sampleSize];	
	gaussFilter = new double[sampleSize];
	m_data = new double[numElements * numElements * sampleSize];
	in = new fftw_complex[sampleSize];
	out = new fftw_complex[sampleSize];
	m_filtered_data_ri = new float[sampleSize * numElements * numElements * 2];
}

void MainWidgetTFM::initShaders()
{
    // Override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader_tfm.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();

    // Restore system locale
    setlocale(LC_ALL, "");
}

void MainWidgetTFM::filterFFT(){

	for (int pulsing = 0; pulsing < numElements; pulsing++) {
		if(pulsing % 10 == 0)
		qDebug() << "filtering: " << ((float)pulsing) * 100.0/((float)numElements) << "%";
	for (int element = 0; element < numElements; element++) {

		// copy the array to fftw_complex structure, could be a faster way e.g. memcpy
		for (int i = 0; i < sampleSize; i++) {
			in[i][0] = m_data[index3d(i,element,pulsing)];
			in[i][1] = 0;
		}
		//for (int i = 0; i < 100; i++) std::cout << i << ": " << in[i][0] << std::endl;

		/* FFT */

		fftw_plan my_plan = fftw_plan_dft_1d(sampleSize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
		fftw_execute(my_plan);

		for (int i = 0; i < sampleSize; i++) {
			out[i][0] /= (float)sampleSize;
			out[i][1] /= (float)sampleSize;
			//std::cout << i << ": " << out[i][0] << " , " << out[i][1] << "i" << std::endl;
		}
		for (int i = 0; i < sampleSize; i++) {
			//magnitude[i] = sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1]);
			//std::cout << i << ": " << magnitude[i] << std::endl;
			//outfile << to_string(magnitude[i]) << endl;
		}

		//int peak_frequency = 0;
		//double peak_val = 0.0;

		//for(int i = 0; i < 500; i++){
		//	//std::cout << i << ": " << magnitude[i] << std::endl;

		//	if(magnitude[i] > peak_val){
		//		std::cout << "new peak : " << i << std::endl;
		//		peak_val = magnitude[i];
		//		peak_frequency = i;
		//	}
		//}

		//std::cout << "peak_frequency(index) : " << peak_frequency << std::endl;
		//float time_period = 1.0 / peak_frequency;
		

		fftw_destroy_plan(my_plan);


		/* FILTERING */

		// Create gaussian filter vector
		// f(x; sig, c) = e ^ ( -(x-c)^2 / 2*sig^2 )
		//float sigma, c;
		//createGaussian(70, 200);
		createGaussian();
		double weight = 2.0;
		
		for (int i = 0; i < sampleSize; i++) {
			out[i][0] = out[i][0] * gaussFilter[i] * weight;
			out[i][1] = out[i][1] * gaussFilter[i] * weight;
		}
		for (int i = 500; i < sampleSize; i++) {
			//out[i][0] = 0.0;
			//out[i][1] = 0.0;
		}
		
		/* INVERSE FFT */

		// To check that there's no existing data
		for (int i = 0; i < sampleSize; i++) {
			in[i][0] = 0;
			in[i][1] = 0;
		}
		
		my_plan = fftw_plan_dft_1d(sampleSize, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);
		fftw_execute(my_plan);
		//for (int i = 0; i < 100; i++) std::cout << i << ": " << in[i][0] << std::endl;
		
		//for (int i = 0; i < sampleSize; i++) std::cout << i << ": " << in[i][0] << " , " << in[i][1] << "i" << std::endl;


		fftw_destroy_plan(my_plan);
		
		// multiply output by 2 (why?)
		for (int i = 0; i < sampleSize; i++) {
			in[i][0] *= 2.0;
			in[i][1] *= 2.0;
			//qDebug() << in[i][0];
			//qDebug() << in[i][1];
		}


		// Calc absolute value and put back into array
		for (int i = 0; i < sampleSize; i++) {
			//m_filtered_data_ri[element][i][0] = (in[i][0]);//(float)
			//m_filtered_data_ri[element][i][1] = (in[i][1]);//(float)
			m_filtered_data_ri[index3d(i,element,pulsing)*2] 		= (in[i][0]);//(float)
			m_filtered_data_ri[index3d(i,element,pulsing)*2+1]	= (in[i][1]);//(float)
		}

	}
	}

	// calculate in seconds, using sampling rate
	//time_period = time_period / samplingRate;
	//std::cout << "test" << std::endl;
	//float frequency = 1 / time_period;
	//std::cout << "peak frequency : " << frequency << std::endl;
}

void MainWidgetTFM::createGaussian()
{
	/** HOW IT USED TO WORK **/

	// Create gaussian filter vector
	// f(x; sig, c) = e ^ ( -(x-c)^2 / 2*sig^2 )
	
	// Calculate sigma and c
	//float sigma = gauss_sigma;
	//float c = gauss_c;

	//for (int x = 0; x < sampleSize; x++) {
	//	gaussFilter[x] = exp( ( (-((x-c)*(x-c))) / (2*sigma*sigma)) );
	//	//std::cout << "filter [" << x << "] = " << gaussFilter[x] << std::endl;
	//}

	//return;
	
	// Really shit and needs cleaning up

	float db_down = 40.0;
	float fract = pow(10.0,(-db_down / 20.0));
	float peak_pos_fract = centreFrequency / samplingFrequency;
	float half_width_fract = peak_pos_fract;
	float r1 = half_width_fract / (pow(-log(fract),0.5));

	double step = 1.0 / (sampleSize-1);
	double count = 0.0;

	for (int x = 0; x < sampleSize; x++) {
		gaussFilter[x] = count - peak_pos_fract;
		count += step;
	}

	for (int x = 0; x < sampleSize; x++) {
		gaussFilter[x] = exp( -pow((gaussFilter[x]/r1),2.0) );
		//std::cout << "filter [" << x << "] = " << gaussFilter[x] << std::endl;
	}
}

void MainWidgetTFM::initTextures()
{
	
    // Read CSV file
    std::ifstream ifile(filename);
	std::string line;

	// x is current receiving element
	// t is current time sample
	// p is current pulsing element
	int x = 0, t = 0, p = 0;
	if (ifile.is_open()){
		double tmp;
		while(ifile >> tmp){
			if(t % 100 == 0)
				qDebug() << "Reading CSV: " << (float)t * 100.0 / sampleSize << "%";
			//std::cout << "temp: " << tmp << std::endl;
			m_data[index3d(t,x,p)] = tmp; // (fabs(tmp));
			//std::cout << "m_data: " << m_data[x][t] << std::endl;
			x++;
			for (int j = 1; j < numElements; j++) {
				ifile >> tmp;
				m_data[index3d(t,x,p)] = tmp; // fabs(tmp);
				//m_data[x][t] = (uchar)255;
				x++;
			}
			p++;
			x = 0;
			for(int i = 1; i < numElements; i++){
				for (int j = 0; j < numElements; j++) {
					ifile >> tmp;
					m_data[index3d(t,x,p)] = tmp; // fabs(tmp);
					//m_data[x][t] = (uchar)255;
					x++;
				}
				x = 0;
				p++;
			}
			x = 0;
			p = 0;
			t++;
		}
		ifile.close();
	}
	else{
		std::cout << "Could not open file: " << std::endl;
		exit(1);
	}

	filterFFT();

	for (int i = 0; i < numElements; i++) {
		for(int x = 0; x < numElements; x++){
			//std::cout << x << std::endl;
			for (int t = 0; t < 1000; t++) {
				//std::cout << m_data[index3d(t,x,i)] << " ";
			}
			//std::cout << std::endl;
		}
	}

    //glEnable(GL_TEXTURE_2D);

	//QPixmap pmap(QSize(64,64));
	//pmap.fill();
	//QImage image(m_data[0], 64, 1500, QImage::Format_Grayscale8);
	//QImage image(m_data[0], 1000, 64, QImage::Format_Grayscale8);

	
	//QOpenGLPixelTransferOptions *options = new QOpenGLPixelTransferOptions;
	//options->setImageHeight(64);
	//options->setRowLength(1000);

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < sampleSize; j++) {
			//std::cout << "[" << i << "][" << j << "]   " << m_filtered_data_ri[i][j][0] << " + " << m_filtered_data_ri[i][j][0] << "i" << std::endl;
			//m_filtered_data_ri[i][j][0] = (float)i/64.0;
			//m_filtered_data_ri[i][j][1] = (float)j/1000.0;
			//m_filtered_data_ri[i][j][0] = (float)i/64.0;
			//m_filtered_data_ri[i][j][1] = (float)j/1000.0;
		}
	}

	//uchar tmpdata[64*3];
	//for (int i = 0; i < 64 * 3; i++) {
	//	tmpdata[i] = (uchar)i*2;
	//}

	//texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	//texture->create();

	////texture = new QOpenGLTexture(image);
	////texture = new QOpenGLTexture(QImage(":/cube.png"));
	//texture->setSize(sampleSize,numElements);
	//texture->setFormat(QOpenGLTexture::RG32F);
	//texture->allocateStorage();
	////texture->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::Float32);
	//texture->setData(QOpenGLTexture::RG, QOpenGLTexture::Float32, m_filtered_data_ri);
	////texture->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, m_filtered_data_ri[0][0], options);
	//std::cout << "allocated? : " << texture->isStorageAllocated() << std::endl;
	//std::cout << "w : " << texture->width() << std::endl;
	//std::cout << "h : " << texture->height() << std::endl;

    //texture->setMinificationFilter(QOpenGLTexture::Nearest);
    //texture->setMagnificationFilter(QOpenGLTexture::Linear);
    //texture->setWrapMode(QOpenGLTexture::Repeat);

	//texture->bind();	



	texture = new QOpenGLTexture(QOpenGLTexture::Target3D);
	texture->create();

	//texture = new QOpenGLTexture(image);
	//texture = new QOpenGLTexture(QImage(":/cube.png"));
	texture->setSize(sampleSize,numElements,numElements);
	texture->setFormat(QOpenGLTexture::RG32F);
	texture->allocateStorage();
	//texture->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::Float32);
	texture->setData(QOpenGLTexture::RG, QOpenGLTexture::Float32, m_filtered_data_ri);
	//texture->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, m_filtered_data_ri[0][0], options);
	std::cout << "allocated? : " << texture->isStorageAllocated() << std::endl;
	std::cout << "w : " << texture->width() << std::endl;
	std::cout << "h : " << texture->height() << std::endl;
	std::cout << "d : " << texture->depth() << std::endl;

    texture->setMinificationFilter(QOpenGLTexture::Linear);
    //texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

	texture->bind();	
}

void MainWidgetTFM::resizeGL(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    // Calculate aspect ratio
    //qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    //const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    //projection.perspective(fov, aspect, zNear, zFar);
	projection.ortho(-1.0f,1.0f,-1.0f,1.0f,0.0f,7.0f);
}

void MainWidgetTFM::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 viewMatrix;
    //viewMatrix.translate(0.0, 0.0, -4.0);
	viewMatrix.scale(viewScale);
    viewMatrix.translate(x_translation, y_translation, 0.0);
    //viewMatrix.rotate(rotation);

	QMatrix4x4 realMatrix;
	//realMatrix.scale(1.0f); // imaging m
	realMatrix.scale(0.001f); // imaging mm 
	//realMatrix.scale(0.01f); // imaging cm 

	program.setUniformValue("sampleSize", (float)sampleSize);
	program.setUniformValue("sampleFrequency", samplingFrequency);
	program.setUniformValue("sampleSpeed", sampleSpeed);
	program.setUniformValue("numElements", numElements);
	program.setUniformValue("e_sep", e_sep);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * viewMatrix);
    program.setUniformValue("m_matrix", realMatrix);

    program.setUniformValue("logfactor", logfactor);
    program.setUniformValue("overexposure", overexposure);

    program.setUniformValue("a_scan", a_scan);

	program.setUniformValue("texture", 0);

	program.setUniformValue("layer", layer);

    // Draw geometry
    geometries.drawGeometry(&program);
}

void MainWidgetTFM::setOverExposure(int val)
{
	overexposure = (float) val / 100.0;
	updateGL();
}

void MainWidgetTFM::setLogFactor(int val)
{
	logfactor = (float) val / 100.0;
	updateGL();
}

void MainWidgetTFM::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

	e_sep += (float)numSteps*0.00001; 

	std::cout << "e_sep: " << e_sep << std::endl;
	update();

    event->accept();
}

void MainWidgetTFM::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidgetTFM::mouseReleaseEvent(QMouseEvent *e)
{
    //// Mouse release position - mouse press position
    //QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    //// Rotation axis is perpendicular to the mouse position difference
    //// vector
    //QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    //// Accelerate angular speed relative to the length of the mouse sweep
    //qreal acc = diff.length() / 100.0;

    //// Calculate new rotation axis as weighted sum
    //rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    //// Increase angular speed
    //angularSpeed += acc;
}

void MainWidgetTFM::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Update scene
        updateGL();
    }
}
