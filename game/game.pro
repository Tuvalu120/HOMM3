# Qmake file for game.
# Author: Bartosz Han

TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = ../out/game
GRAPHICS_DIR = ../graphics
QMAKE_TARGET_OUT = $$DESTDIR

TARGET = heroes3

INCLUDEPATH += headers \
               $$GRAPHICS_DIR/headers \
               ../common/headers

VPATH += src \
         $$GRAPHICS_DIR/src

SOURCES += main.cc \
           $$files(src/*.cc, true) \
           $$files($$GRAPHICS_DIR/src/*.cc, true) \
           $$files(../common/src/*.cc, true)


HEADERS += $$files(headers/*.h, true) \
           #$$files(headers/*.hpp, true) \
           $$files($$GRAPHICS_DIR/headers/*.h, true) \
           $$files($$GRAPHICS_DIR/headers/*.hpp, true) \
           $$files(../common/*.hpp, true) \
           $$files(../common/*.h, true)

FORMS += $$files($$GRAPHICS_DIR/windows/*.ui, true)

OBJECTS_DIR = $$QMAKE_TARGET_OUT/obj
MOC_DIR = $$QMAKE_TARGET_OUT/moc
RCC_DIR = $$QMAKE_TARGET_OUT/rcc
UI_DIR = $$GRAPHICS_DIR/ui


create_out_dirs.target = create_out_dirs
create_out_dirs.commands = mkdir -p ../out $$QMAKE_TARGET_OUT $$DESTDIR $$OBJECTS_DIR $$MOC_DIR $$RCC_DIR
QMAKE_EXTRA_TARGETS += create_out_dirs
PRE_TARGETDEPS += create_out_dirs
