#-------------------------------------------------
#
# Project created by QtCreator 2019-04-04T06:09:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Takao
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        main.cpp \
        takao.cpp

HEADERS += \
        takao.h

FORMS += \
        takao.ui

LIBS += -lboost_python3
LIBS += -lboost_numpy3
LIBS += -lpython3.7m

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$OUT_PWD/../Utilities/ -lUtilities
INCLUDEPATH += $$PWD/../Utilities
DEPENDPATH += $$PWD/../Utilities

LIBS += -L$$OUT_PWD/../GameManager/ -lGameManager
INCLUDEPATH += $$PWD/../GameManager
DEPENDPATH += $$PWD/../GameManager

LIBS += -L$$OUT_PWD/../Visualizer/ -lVisualizer
INCLUDEPATH += $$PWD/../Visualizer
DEPENDPATH += $$PWD/../Visualizer

LIBS += -L$$OUT_PWD/../Algorithms/ -lAlgorithms
INCLUDEPATH += $$PWD/../Algorithms
DEPENDPATH += $$PWD/../Algorithms

INCLUDEPATH += /usr/include/python3.7m
