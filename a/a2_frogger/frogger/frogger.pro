TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    console.c

HEADERS += \
    console.h


unix|win32: LIBS += -lncurses
