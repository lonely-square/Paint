#-------------------------------------------------
#
# Project created by QtCreator 2018-09-29T20:04:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Paint
TEMPLATE = app

INCLUDEPATH += D:\eigen-3.4.0\eigen-3.4.0\Eigen

SOURCES += main.cpp\
    edge.cpp \
        paintwindow.cpp \
    paintarea.cpp \
    pencil.cpp \
    polygonInner.cpp \
    shape.cpp

HEADERS  += paintwindow.h \
    edge.h \
    paintarea.h \
    pencil.h \
    polygonInner.h \
    shape.h

FORMS    +=

RESOURCES += \
    paint.qrc
