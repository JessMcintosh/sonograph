QMAKE_MAC_SDK = macosx10.11
QT += network widgets
QT += core

HEADERS       = networkcontroller.h \
				fakemachine.h
SOURCES       = networkcontroller.cpp \
				fakemachine.cpp \
                main.cpp

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/network/multicastreceiver
#INSTALLS += target

DESTDIR=bin
OBJECTS_DIR=aux_files
MOC_DIR=aux_files
