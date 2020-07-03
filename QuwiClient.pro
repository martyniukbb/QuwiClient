#-------------------------------------------------
#
# Project created by QtCreator 2020-06-25T10:54:36
#
#-------------------------------------------------

QT += core gui network
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuwiClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    projects.cpp \
    quwiclient.cpp \
    projectsdelegate.cpp \
    itemdatabase.cpp \
    quwiprojectlistitem.cpp \
    editproject.cpp

HEADERS  += mainwindow.h \
    projects.h \
    quwiclient.h \
    projectsdelegate.h \
    itemdatabase.h \
    quwiprojectlistitem.h \
    editproject.h

FORMS    += mainwindow.ui \
    projects.ui \
    editproject.ui

RESOURCES += \
    images.qrc
