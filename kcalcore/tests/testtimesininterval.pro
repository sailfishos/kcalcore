TEMPLATE = app
TARGET = tst_timesininterval

QT += testlib
CONFIG += link_pkgconfig
PKGCONFIG += libical uuid

DEPENDPATH += . $$PWD/.. $$PWD/../versit $$PWD/../klibport $$PWD/../kdedate
INCLUDEPATH += . $$PWD/.. $$PWD/../versit $$PWD/../klibport $$PWD/../kdedate /usr/include/libical

QMAKE_LIBDIR += $$PWD/..
LIBS += -lkcalcoren-qt5

HEADERS += testtimesininterval.h
SOURCES += testtimesininterval.cpp

target.path = /opt/tests/kcalcore-qt5/

INSTALLS += target
