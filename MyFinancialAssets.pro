QT       += core gui sql network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    asset.cpp \
    assetapi.cpp \
    assetfutureyield.cpp \
    assetwindow.cpp \
    basics.cpp \
    closedfixedincomeswindow.cpp \
    constants.cpp \
    database.cpp \
    databasecreation.cpp \
    editvaluewindow.cpp \
    event.cpp \
    eventcontroller.cpp \
    financialinstitution.cpp \
    financialinstitutionmonth.cpp \
    financialinstitutionwindow.cpp \
    fixedincome.cpp \
    investmentcontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    monthwindow.cpp \
    newfixedincomewindow.cpp \
    newreorganizationwindow.cpp \
    newtransactionwindow.cpp \
    newyieldwindow.cpp \
    reorganization.cpp \
    saleswindow.cpp \
    transaction.cpp \
    updatefixedincomewindow.cpp \
    variableincomeperformancewindow.cpp \
    yield.cpp

HEADERS += \
    asset.h \
    assetapi.h \
    assetfutureyield.h \
    assetwindow.h \
    basics.h \
    closedfixedincomeswindow.h \
    constants.h \
    database.h \
    databasecreation.h \
    editvaluewindow.h \
    event.h \
    eventcontroller.h \
    financialinstitution.h \
    financialinstitutionmonth.h \
    financialinstitutionwindow.h \
    fixedincome.h \
    investmentcontroller.h \
    mainwindow.h \
    monthwindow.h \
    newfixedincomewindow.h \
    newreorganizationwindow.h \
    newtransactionwindow.h \
    newyieldwindow.h \
    reorganization.h \
    saleswindow.h \
    transaction.h \
    updatefixedincomewindow.h \
    variableincomeperformancewindow.h \
    yield.h

FORMS += \
    assetwindow.ui \
    closedfixedincomeswindow.ui \
    editvaluewindow.ui \
    financialinstitutionwindow.ui \
    mainwindow.ui \
    monthwindow.ui \
    newfixedincomewindow.ui \
    newreorganizationwindow.ui \
    newtransactionwindow.ui \
    newyieldwindow.ui \
    saleswindow.ui \
    updatefixedincomewindow.ui \
    variableincomeperformancewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources.qrc
