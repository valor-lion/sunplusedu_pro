#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T09:30:07
#
#-------------------------------------------------

QT       += core gui

TARGET = FMQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    mypushbutton.cpp \
    showwidget.cpp \
    pyvolume.cpp \
    listwidget.cpp \
    mylist.cpp \
    device.cpp \
    stationparam.cpp

HEADERS  += mainwidget.h \
    mypushbutton.h \
    pyvolume.h \
    showwidget.h \
    listwidget.h \
    mylist.h \
    device.h \
    stationparam.h \
    si4702.h

FORMS    += mainwidget.ui \
    listwidget.ui

RESOURCES += \
    image.qrc
