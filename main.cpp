#include <QCommandLineParser> 
//#include <QCoreApplication> 
#include <QApplication>
#include <QLabel>
#include <QDebug>

#ifndef QT_NO_OPENGL
//#include "mainwidget.h"
#include "mainwindow.h"
#endif

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("Sonograph");
	app.setApplicationVersion("0.1");
#ifndef QT_NO_OPENGL
	QCommandLineParser parser;
	parser.setApplicationDescription("Ultrasonograph");
	parser.addHelpOption();
	parser.addOptions({
		// Boolean option for Total Focussing Method
		{{"t", "tfm"},
			QCoreApplication::translate("main", "Total Focussing Method")},
		// Speed of sound in medium
		{{"s", "speed"},
			QCoreApplication::translate("main", "Speed of sound in medium."),
			QCoreApplication::translate("main", "speed")},
		// Seperation between elements
		{{"e", "e_sep"},
			QCoreApplication::translate("main", "Seperation between elements."),
			QCoreApplication::translate("main", "e_sep")},
		// Sampling frequency
		{{"f", "frequency"},
			QCoreApplication::translate("main", "Sampling frequency."),
			QCoreApplication::translate("main", "frequency")},
		// Sample length
		{{"l", "length"},
			QCoreApplication::translate("main", "Number of samples stored / to store."),
			QCoreApplication::translate("main", "samples")},
		{{"n", "elements"},
			QCoreApplication::translate("main", "Number of elements used."),
			QCoreApplication::translate("main", "elements")},
		{"file",
			QCoreApplication::translate("main", "File."),
			QCoreApplication::translate("main", "file")},
	});
	parser.process(app);

	// Default values
	//float s = 1540.0;
	float s = 6300.0;
	float f = 25000000.0;
	int l = 1000;
	int n = 64;
	float e = 0.00063;
	bool tfm = false;
	std::string file = "hole_data.csv";

	if(parser.isSet("speed")) s = parser.value("speed").toFloat();
	if(parser.isSet("frequency")) f = parser.value("frequency").toFloat();
	if(parser.isSet("length")) l = parser.value("length").toInt();
	if(parser.isSet("elements")) n = parser.value("elements").toInt();
	if(parser.isSet("e_sep")) e = parser.value("e_sep").toFloat();
	if(parser.isSet("file")) file = parser.value("file").toStdString();
	tfm = parser.isSet("tfm");
	qDebug() << s << QString(file.c_str());
	qDebug() << QString("TFM: ") << tfm ;

	MainWindow mainWindow(s, f, l, n, e, file, tfm);

	mainWindow.show();
	//MainWidget widget;
	//widget.show();
#else
	QLabel note("OpenGL Support required");
	note.show();
#endif
	return app.exec();
}
