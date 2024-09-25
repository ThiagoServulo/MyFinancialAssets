QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    asset.cpp \
    constants.cpp \
    database.cpp \
    event.cpp \
    eventcontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    newreorganizationwindow.cpp \
    newtransactionwindow.cpp \
    newyieldwindow.cpp \
    transaction.cpp

HEADERS += \
    asset.h \
    constants.h \
    database.h \
    event.h \
    eventcontroller.h \
    mainwindow.h \
    newreorganizationwindow.h \
    newtransactionwindow.h \
    newyieldwindow.h \
    transaction.h

FORMS += \
    mainwindow.ui \
    newreorganizationwindow.ui \
    newtransactionwindow.ui \
    newyieldwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
