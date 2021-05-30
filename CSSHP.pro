QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupports

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += _ELPP_QT_LOGGING \
           _ELPP_STL_LOGGING \
           _ELPP_STRICT_ROLLOUT \
           _START_EASYLOGGINGPP

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    common/modbus_utils.cpp \
    transducer/transducer_manager.cpp \
    transducer/transducer_thread.cpp \
    qcustomplot.cpp

HEADERS += \
    mainwindow.h \
    common/modbus_utils.h \
    transducer/transducer_manager.h \
    transducer/transducer_thread.h \
    qcustomplot.h \
    easylogging++.h

win32:INCLUDEPATH += $$PWD/thirdparty/modbus/modbus-x86/include
win32:LIBS += -L$$PWD/thirdparty/modbus/modbus-x86/lib -lmodbus
unix:INCLUDEPATH += $$PWD/thirdparty/modbus/modbus-arm/include
unix:LIBS += -L$$PWD/thirdparty/modbus/modbus-arm/lib -lmodbus

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    CSSHP_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    CSSHP.qrc
