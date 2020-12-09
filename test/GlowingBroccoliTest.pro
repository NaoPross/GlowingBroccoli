include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread

CONFIG += qt c++14 warn_on strict_c++
QT     += core gui
 greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
        ../src/snake.h \
        tst_gameplay.h

SOURCES += \
        ../src/snake.cpp \
        main.cpp
