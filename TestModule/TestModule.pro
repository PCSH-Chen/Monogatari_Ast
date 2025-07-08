QT += core gui widgets

CONFIG += plugin

TEMPLATE = lib
TARGET = TestModule

HEADERS += TestModule.h
SOURCES += TestModule.cpp

# Include path to ModuleTemplate.h
INCLUDEPATH += ..

# Output to Modules directory
DESTDIR = ../Modules

# Platform-specific library settings
unix:!macx {
    QMAKE_LFLAGS += -Wl,--no-undefined
}

# Debug information
CONFIG += debug_and_release