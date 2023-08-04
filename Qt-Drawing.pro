
QT       += core gui widgets xml printsupport

TARGET = Qt-Drawing
TEMPLATE = app
DESTDIR = $$PWD/bin


DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11


SOURCES += \
        items/mellipseobject.cpp \
        items/mlineobject.cpp \
        items/mrectobject.cpp \
        items/mtextobject.cpp \
        items/mbaseobject.cpp \
        scenes/mbasescene.cpp \
        views/mbaseview.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        items/mellipseobject.h \
        items/mlineobject.h \
        items/mrectobject.h \
        items/mtextobject.h \
        items/mbaseobject.h \
        scenes/mbasescene.h \
        views/mbaseview.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += $$PWD/views \
               $$PWD/items \
               $$PWD/scenes

RESOURCES += \
    resources.qrc

RC_ICONS = logo.ico
