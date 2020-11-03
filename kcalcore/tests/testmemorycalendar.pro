TEMPLATE = app
TARGET = tst_memorycalendar

QT -= gui
QT += testlib
CONFIG += link_pkgconfig
PKGCONFIG += libical uuid

DEPENDPATH += . $$PWD/.. $$PWD/../versit $$PWD/../klibport $$PWD/../kdedate
INCLUDEPATH += . $$PWD/.. $$PWD/../versit $$PWD/../klibport $$PWD/../kdedate /usr/include/libical

DEFINES += "ICALTESTDATADIR=\\\"/opt/tests/kcalcore-qt5/\\\""

QMAKE_LIBDIR += $$PWD/..
LIBS += -lkcalcoren-qt5

HEADERS += testmemorycalendar.h
SOURCES += testmemorycalendar.cpp

target.path = /opt/tests/kcalcore-qt5/
