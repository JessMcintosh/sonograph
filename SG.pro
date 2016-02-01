QMAKE_MAC_SDK = macosx10.11

QT       += core gui widgets network
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
	   TCP_controller/networkcontroller.cpp


    HEADERS += \
        mainwidget.h \
		mainwidget_tfm.h \
        geometryengine.h \
		mainwindow.h \
	    TCP_controller/networkcontroller.h

    RESOURCES += \
        shaders.qrc \
		textures.qrc
}

