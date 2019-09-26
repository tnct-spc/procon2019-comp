#-------------------------------------------------
#
# Project created by QtCreator 2019-04-02T22:07:23
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = GameManager
TEMPLATE = lib

DEFINES += GAMEMANAGER_LIBRARY

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        gamesimulator.cpp \
        gamemanager.cpp \
    expandedgamesimulator.cpp

HEADERS += \
        gamesimulator.h \
        gamemanager.h \
    expandedgamesimulator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -lboost_python3
LIBS += -lpython3.7m

LIBS += -L$$OUT_PWD/../Server/ -lServer
INCLUDEPATH += $$PWD/../Server
DEPENDPATH += $$PWD/../Server

LIBS += -L$$OUT_PWD/../Algorithms/ -lAlgorithms
INCLUDEPATH += $$PWD/../Algorithms
DEPENDPATH += $$PWD/../Algorithms

LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities
INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities

LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer

INCLUDEPATH += /usr/include/python3.7m
