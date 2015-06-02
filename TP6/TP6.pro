# -------------------------------------------------
# Project created by QtCreator 2010-05-29T14:32:09
# -------------------------------------------------
QT -= gui
TARGET = segment
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    point.cpp \
    contraste.cpp
INCLUDEPATH += "/usr/include/opencv"
LIBS += -L/usr/lib \
    -lcv \
    -lhighgui
HEADERS += point.h \
    contraste.h
