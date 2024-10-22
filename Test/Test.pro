TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

QT += core gui widgets

SOURCES += \
    com.cpp \
    generatecnpyboarddata.cpp \
        main.cpp \
    generaterandomsimulateddata.cpp \
    randomplayoutforpolicyplayer.cpp

HEADERS += \
    com.h \
    generatecnpyboarddata.h \
    generaterandomsimulateddata.h \
    randomplayoutforpolicyplayer.h

LIBS += -lboost_python3
LIBS += -lboost_numpy3
LIBS += -lpython3.7m
LIBS += -lcnpy
LIBS += -lz

LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities
INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities

LIBS += -L$$OUT_PWD/../Algorithms/ -lAlgorithms
INCLUDEPATH += $$PWD/../Algorithms
DEPENDPATH += $$PWD/../Algorithms

LIBS += -L$$OUT_PWD/../GameManager/ -lGameManager
INCLUDEPATH += $$PWD/../GameManager
DEPENDPATH += $$PWD/../GameManager

LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer

LIBS += -L$$OUT_PWD/../Communication/ -lCommunication
INCLUDEPATH += $$PWD/../Communication
DEPENDPATH += $$PWD/../Communication

LIBS += -L$$OUT_PWD/../Server/ -lServer
INCLUDEPATH += $$PWD/../Server
DEPENDPATH += $$PWD/../Server

INCLUDEPATH += /usr/include/python3.7m
