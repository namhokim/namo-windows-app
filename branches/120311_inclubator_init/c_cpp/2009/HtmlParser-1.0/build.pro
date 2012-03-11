win32:TEMPLATE	= vclib
unix:TEMPLATE = lib
macx:TEMPLATE = lib
LANGUAGE	= C++

CONFIG	+= warn_on 

unix:CONFIG += staticlib
win32:CONFIG += staticlib

QT      -= gui

include(src/__build__.pri)

ROOT_DIR = ..
TARGET = HtmlParser

INCLUDEPATH     += $$ROOT_DIR/IOStreams-1.0/src


DESTDIR = dist
OBJECTS_DIR = obj


unix:LIBS += -lGStreams -L$$DESTDIR
win32:LIBS += $$DESTDIR/JStreams.lib

CONFIG(release) {
 DEFINES += QT_NO_DEBUG_OUTPUT
}


