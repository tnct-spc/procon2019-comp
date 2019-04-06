TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

QT += core gui widgets

SOURCES += \
        main.cpp \
    generaterandomsimulateddata.cpp

HEADERS += \
    generaterandomsimulateddata.h


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
