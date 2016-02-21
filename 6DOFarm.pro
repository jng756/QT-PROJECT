#-------------------------------------------------
#
# Project created by QtCreator 2015-03-29T19:54:42
#
#-------------------------------------------------

QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 6DOFarm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    images.qrc

DISTFILES +=
