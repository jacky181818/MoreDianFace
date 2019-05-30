#-------------------------------------------------
#
# Project created by QtCreator 2019-05-08T15:06:21
#
#-------------------------------------------------

QT       += core gui
QT       += jsonserializer network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MoreDianFace
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    HTTP\chttprequest.cpp \
    HTTP\httprequest.cpp \
    HTTP\CMoreDianHttpMethod.cpp \
    HTTP\swmhttp.cpp \
    MoreDianData.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    HTTP\chttprequest.h \
    HTTP\httprequest.h \
    HTTP\CMoreDianHttpMethod.h \
    HTTP\swmhttp.h \
    MoreDianData.h \
    dialog.h
    ../VDPServer/Log4/mylogger.h

FORMS    += mainwindow.ui \
    dialog.ui

INCLUDEPATH += ../VDPServer/Log4 \
    $$PWD/HTTP

CONFIG(debug, debug|release) {
    LIBS +=  $$PWD/../VDPServer/lib/Debug/log4cplusSD.lib
    BUILD = Debug
    BUILD_SUFFIX = D
} else {
    LIBS +=  $$PWD/../VDPServer/lib/Release/log4cplusS.lib
    BUILD = Release
}

win32 {
LIBS += $$PWD/../VDPServer/lib/Release/DbgHelp.Lib \
        $$PWD/../VDPServer/lib/Release/WtsApi32.Lib \
        $$PWD/../VDPServer/lib/Release/UserEnv.Lib \
        $$PWD/../VDPServer/lib/Release/libssl.lib \
        $$PWD/../VDPServer/lib/Release/libcrypto.lib \
        "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x86/AdvAPI32.Lib"
}

DISTFILES += \
    readme.md \
    readme.txt
