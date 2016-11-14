#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T21:16:35
#
#-------------------------------------------------


QT       += core gui widgets serialport


TARGET = newbalance
TEMPLATE = app


SOURCES += main.cpp\
        balancingsys.cpp \
    serialthread.cpp \
    calculatefunction.cpp

HEADERS  += balancingsys.h \
    serialthread.h \
    calculatefunction.h

FORMS    += balancingsys.ui

DISTFILES +=

RESOURCES += \
    re.qrc
