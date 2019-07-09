#-------------------------------------------------
#
# Project created by QtCreator 2019-04-02T22:14:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = Algorithms
TEMPLATE = lib

DEFINES += ALGORITHMS_LIBRARY

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
        PolicyPlayer/Evaluator/evaluatorwrapper.cpp \
        PolicyPlayer/Evaluator/randomevaluator.cpp \
        PolicyPlayer/Searcher/searcherwrapper.cpp \
        PolicyPlayer/Searcher/simplesearcher.cpp \
        PolicyPlayer/policyplayer.cpp \
        algofrompython.cpp \
        algorithmwrapper.cpp \
        testalgorithm.cpp \
        randommove.cpp \
        simplebeamsearch.cpp \
        boarddividealgorithm.cpp

HEADERS += \
        PolicyPlayer/Evaluator/evaluatorwrapper.h \
        PolicyPlayer/Evaluator/randomevaluator.h \
        PolicyPlayer/Searcher/searcherwrapper.h \
        PolicyPlayer/Searcher/simplesearcher.h \
        PolicyPlayer/policyplayer.h \
        PolicyPlayer/selectors.h \
        algofrompython.h \
        algorithmwrapper.h \
        testalgorithm.h \
        randommove.h \
        simplebeamsearch.h \
        boarddividealgorithm.h

LIBS += -lboost_python3
LIBS += -lboost_numpy3
LIBS += -lpython3.7m

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities
INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities

INCLUDEPATH += /usr/include/python3.7m
