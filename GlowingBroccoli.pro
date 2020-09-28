
TEMPLATE = app

CONFIG += qt c++14 warn_on strict_c++
QT     += core gui
 greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GlowingBroccoli
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += src

HEADERS += src/mainwindow.h \
    src/snake.h
FORMS   += src/mainwindow.ui
SOURCES += src/main.cpp src/mainwindow.cpp \
    src/snake.cpp
