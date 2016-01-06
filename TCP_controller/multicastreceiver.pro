QT += network widgets
QT += core

HEADERS       = networkcontroller.h
SOURCES       = networkcontroller.cpp \
                main.cpp

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/network/multicastreceiver
#INSTALLS += target

DESTDIR=bin
OBJECTS_DIR=aux_files
MOC_DIR=aux_files
