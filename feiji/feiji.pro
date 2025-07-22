QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = feiji
TEMPLATE = app

SOURCES += Source Files/main.cpp \
           Source Files/widget.cpp \
           Source Files/before.cpp \
           Source Files/play01.cpp \
           Source Files/play02.cpp \
           Source Files/over.cpp \
           Source Files/plane.cpp

HEADERS  += Header Files/widget.h \
           Header Files/before.h \
           Header Files/play01.h \
           Header Files/play02.h \
           Header Files/over.h \
           Header Files/plane.h \
           Header Files/config.h

FORMS    += Forms/widget.ui

RESOURCES += Resources/resources.qrc

DESTDIR = ./bin
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
RCC_DIR = ./rcc