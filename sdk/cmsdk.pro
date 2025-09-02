QT += core network
QT -= gui

TEMPLATE = lib
CONFIG += staticlib

TARGET = cmsdk

INCLUDEPATH += include

HEADERS += \
    include/cmsdk/authapi.h \
    include/cmsdk/orderapi.h

SOURCES += \
    src/authapi.cpp \
    src/orderapi.cpp

# Installation
target.path = $$PWD/../build
INSTALLS += target