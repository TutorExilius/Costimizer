#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T19:14:28
#
#-------------------------------------------------

QT       += core gui widgets sql printsupport

TARGET = Costimizer
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mylist.cpp \
    shopitem.cpp \
    discounter.cpp \
    discountershopitem.cpp \
    config.cpp \
    configdialog.cpp \
    db_dataprovider.cpp \
    discounterwindow.cpp

HEADERS += \
        mainwindow.h \
    mylist.h \
    shopitem.h \
    discounter.h \
    discountershopitem.h \
    config.h \
    configdialog.h \
    db_dataprovider.h \
    discounterwindow.h

FORMS += \
        mainwindow.ui \
    configdialog.ui \
    discounterwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
