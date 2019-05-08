#-------------------------------------------------
#
# Project created by QtCreator 2019-04-01T16:10:55
#
#-------------------------------------------------

QT       -= gui

TARGET = Python
TEMPLATE = lib

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
        communication.cpp

HEADERS += \
        communication.h

LIBS += -lboost_python
LIBS += -lpython3.7m

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities
INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities

LIBS += -L$$OUT_PWD/../Algorithms/ -lAlgorithms
INCLUDEPATH += $$PWD/../Algorithms
DEPENDPATH += $$PWD/../Algorithms

LIBS += -L$$OUT_PWD/../GameManager/ -lGameManager
INCLUDEPATH += $$PWD/../GameManager
DEPENDPATH += $$PWD/../GameManager

INCLUDEPATH += /usr/include/python3.7m
