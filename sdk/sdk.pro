QT = core network
QT -= gui

CONFIG += c++11 staticlib
TEMPLATE = lib
TARGET = cmsdk

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += include

HEADERS += \
    include/cmsdk/auth_api.h \
    include/cmsdk/order_api.h

SOURCES += \
    src/auth_api.cpp \
    src/order_api.cpp

target.path = /usr/lib
!isEmpty(target.path): INSTALLS += target