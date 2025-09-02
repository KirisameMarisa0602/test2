QT += core gui widgets network multimedia sql
CONFIG += c++11
TEMPLATE = app
TARGET = cloudmeeting-client

UI_BASE = $$PWD/ui_cloudmeeting

HEADERS += $$files($$UI_BASE/Headers/*.h, true)
SOURCES += $$files($$UI_BASE/Sources/*.cpp, true)
FORMS   += $$files($$UI_BASE/Forms/*.ui, true)
RESOURCES += $$files($$UI_BASE/Resources/*.qrc, true)

include($$PWD/sdk_link.pri)

!exists($$UI_BASE/Sources/*.cpp) {
    SOURCES += $$PWD/main.cpp
}
