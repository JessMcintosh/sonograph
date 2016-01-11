QT       += core gui widgets 
CONFIG += debug c++11

TARGET = program.exe
TEMPLATE = app

DESTDIR = bin
OBJECTS_DIR = bin/.obj
MOC_DIR = bin/.moc
RCC_DIR = bin/.rcc
UI_DIR = bin/.ui

SOURCES += main.cpp

LIBS''= -lfftw3

qtHaveModule(opengl) {
    QT += opengl

    SOURCES += mainwidget.cpp \
	   mainwidget_tfm.cpp \
       geometryengine.cpp \
	   mainwindow.cpp \
	   #window.cpp

    HEADERS += \
        mainwidget.h \
		mainwidget_tfm.h \
        geometryengine.h \
		mainwindow.h \
		#window.h

    RESOURCES += \
        shaders.qrc \
		textures.qrc
}

