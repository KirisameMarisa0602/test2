TEMPLATE = lib
CONFIG += staticlib
QT += core network

TARGET = cmsdk
INCLUDEPATH += $$PWD/include

HEADERS += \
    $$PWD/include/cmsdk/authapi.h \
    $$PWD/include/cmsdk/orderapi.h

SOURCES += \
    $$PWD/src/authapi.cpp \
    $$PWD/src/orderapi.cpp
