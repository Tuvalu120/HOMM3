# Qmake file for battle_example program.
# Author: Bartosz Han

TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PROJECT_NAME = battle_example

INCLUDEPATH += ../game/headers \
                ../common/headers \
                ../graphics/headers \
                ../graphics/ui \
                ../trainer/headers \
                ../trainer/headers/NEAT

OBJECTS_PATH = ../out/$$PROJECT_NAME
DESTDIR = $$OBJECTS_PATH

TRAINER_SOURCES = $$files(../trainer/src/*.cc, false) \
                  $$files(../trainer/src/NEAT/*.cc, true)

GAME_SOURCES = $$files(../game/src/*.cc, true)

COMMON_SOURCES = $$files(../common/src/*.cc, true)

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/rcc

GRAPHICS_SOURCES = $$files(../graphics/src/*.cc, true)

SOURCES = $$TRAINER_SOURCES \
          $$GRAPHICS_SOURCES \
          $$GAME_SOURCES \
          $$COMMON_SOURCES \
          main.cc

HEADERS += $$files(../trainer/headers/*.h, true) \
           $$files(../trainer/headers/*.hpp, true) \
           $$files(../graphics/headers/*.h, true) \
           $$files(../graphics/headers/*.hpp, true) \
           $$files(../game/headers/*.h, true) \
           $$files(../game/headers/*.hpp, true) \
           $$files(../common/headers*.hpp, true) \
           $$files(../common/headers*.h, true)

TARGET = $$PROJECT_NAME

create_out_dirs.target = create_out_dirs
create_out_dirs.commands = mkdir -p ../out $$DESTDIR $$OBJECTS_DIR $$MOC_DIR $$RCC_DIR
QMAKE_EXTRA_TARGETS += create_out_dirs
PRE_TARGETDEPS += create_out_dirs
