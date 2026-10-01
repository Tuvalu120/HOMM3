# Qmake file for trainer.
# Author: Bartosz Han

TEMPLATE = app

INCLUDEPATH += ../game/headers \
                headers \
                headers/NEAT \
                ../common/headers

OBJECTS_PATH = ../out/trainer
DESTDIR = $$OBJECTS_PATH

OBJECTS_DIR = $$OBJECTS_PATH/obj

TRAINER_SOURCES = $$files(src/*.cc, false) \
                  $$files(src/NEAT/*.cc, true) \
                  main.cc

COMMON_SOURCES = $$files(../common/src/*.cc, true)

SOURCES = $$TRAINER_SOURCES \
          $$COMMON_SOURCES \
          $$files(../game/src/*.cc, true)

HEADERS += $$files(headers/*.h, true) \
           $$files(headers/*.hpp, true) \
           $$files(../game/headers/*.h, true) \
           $$files(../game/headers/*.hpp, true) \
           $$files(../common/*.hpp, true) \
           $$files(../common/*.h, true)

TARGET = trainer

create_out_dirs.target = create_out_dirs
create_out_dirs.commands = mkdir -p ../out $$DESTDIR $$OBJECTS_DIR
QMAKE_EXTRA_TARGETS += create_out_dirs
PRE_TARGETDEPS += create_out_dirs
