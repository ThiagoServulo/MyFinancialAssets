QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    asset.cpp \
    assetapi.cpp \
    assetcontroller.cpp \
    assetwindow.cpp \
    basics.cpp \
    constants.cpp \
    database.cpp \
    event.cpp \
    eventcontroller.cpp \
    fixedincome.cpp \
    main.cpp \
    mainwindow.cpp \
    newfixedincomewindow.cpp \
    newreorganizationwindow.cpp \
    newtransactionwindow.cpp \
    newyieldwindow.cpp \
    reorganization.cpp \
    saleswindow.cpp \
    transaction.cpp \
    yield.cpp

HEADERS += \
    asset.h \
    assetapi.h \
    assetcontroller.h \
    assetwindow.h \
    basics.h \
    constants.h \
    database.h \
    event.h \
    eventcontroller.h \
    fixedincome.h \
    mainwindow.h \
    newfixedincomewindow.h \
    newreorganizationwindow.h \
    newtransactionwindow.h \
    newyieldwindow.h \
    reorganization.h \
    saleswindow.h \
    transaction.h \
    yield.h

FORMS += \
    assetwindow.ui \
    mainwindow.ui \
    newfixedincomewindow.ui \
    newreorganizationwindow.ui \
    newtransactionwindow.ui \
    newyieldwindow.ui \
    saleswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
