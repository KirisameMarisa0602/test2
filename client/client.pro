QT += core gui widgets multimedia network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = app
TARGET = client

# Use CloudMeeting UI files
UI_BASE = $$PWD/ui_cloudmeeting

INCLUDEPATH += $$UI_BASE/Headers
INCLUDEPATH += $$UI_BASE/Headers/comm
INCLUDEPATH += $$UI_BASE/Sources
INCLUDEPATH += $$UI_BASE/Sources/comm
INCLUDEPATH += $$UI_BASE/Forms
INCLUDEPATH += $$UI_BASE/Resources

# Automatically include all files from ui_cloudmeeting
HEADERS += $$files($$UI_BASE/Headers/*.h, true)
SOURCES += $$files($$UI_BASE/Sources/*.cpp, true)
FORMS += $$files($$UI_BASE/Forms/*.ui, true)
RESOURCES += $$files($$UI_BASE/Resources/*.qrc, true)

# Link to SDK
include(./sdk_link.pri)

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
