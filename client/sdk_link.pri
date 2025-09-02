# SDK linking configuration
SDK_DIR = $$PWD/../sdk

INCLUDEPATH += $$SDK_DIR/include
LIBS += -L$$SDK_DIR -lcmsdk

# Ensure SDK is built first
PRE_TARGETDEPS += $$SDK_DIR/libcmsdk.a