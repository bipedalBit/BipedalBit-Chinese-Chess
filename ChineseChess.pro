#-------------------------------------------------
#
# Project created by QtCreator 2015-11-01T20:40:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChineseChess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Evaluator.cpp \
    MTD_f.cpp \
    ParallelGenerator.cpp \
    SerialGenerator.cpp \
    State.cpp \
    AlphaBeta.cpp \
    clickablelabel.cpp

HEADERS  += mainwindow.h \
    Evaluator.h \
    Move.h \
    MoveGenerator.h \
    MTD_f.h \
    ParallelGenerator.h \
    Position.h \
    SearchCore.h \
    SerialGenerator.h \
    State.h \
    AlphaBeta.h \
    clickablelabel.h

FORMS    += mainwindow.ui

CONFIG += c++11

RESOURCES += \
    rc.qrc

DISTFILES +=
