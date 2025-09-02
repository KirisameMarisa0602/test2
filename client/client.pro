QT += core gui widgets multimedia network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = app
TARGET = client

# Include SDK
include(sdk_link.pri)

# Use cloudmeeting UI files
UI_DIR = ui_cloudmeeting

INCLUDEPATH += \
    $$UI_DIR/Headers \
    $$UI_DIR/Headers/comm \
    $$UI_DIR/Sources \
    $$UI_DIR/Sources/comm \
    $$UI_DIR/Forms \
    $$UI_DIR/Resources

# Automatically include all headers and sources from ui_cloudmeeting
HEADERS += $$files($$UI_DIR/Headers/*.h, true)
SOURCES += $$files($$UI_DIR/Sources/*.cpp, true)
FORMS += $$files($$UI_DIR/Forms/*.ui, true)
RESOURCES += $$files($$UI_DIR/Resources/*.qrc, true)

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target