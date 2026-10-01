# Qmake file for tests.
# Author: Bartosz Han

TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += ../game/headers \
               ../game/out/ui \
               ../game/ui \
               ../common/headers \
               ../trainer/headers \
               ../trainer/headers/NEAT

DESTDIR = ../out/tests

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/rcc
UI_DIR = $$DESTDIR/ui

TEST_SOURCES = $$files(*.cc, true)

SOURCES = $$TEST_SOURCES \
          $$files(../game/src/*.cc, true) \
          $$files(../common/src/*.cc, true) \
          $$files(../trainer/src/*.cc, true) \

HEADERS += $$files(../game/headers/*.h, true) \
           $$files(../game/ui/*.h, true) \
           $$files(../game/headers/*.hpp, true) \
           $$files(../common/*.hpp, true) \
           $$files(../common/*.h, true) \
           $$files(../trainer/headers*.h, true)

TARGET = test_runner

create_out_dirs.target = create_out_dirs
create_out_dirs.commands = mkdir -p $$DESTDIR $$OBJECTS_DIR $$MOC_DIR $$RCC_DIR $$UI_DIR
QMAKE_EXTRA_TARGETS += create_out_dirs
PRE_TARGETDEPS += create_out_dirs

QMAKE_POST_LINK += cp $$DESTDIR/$$TARGET $$DESTDIR/$$TARGET"_backup"; \
                    $$DESTDIR/$$TARGET

run_tests.target = run_tests_target
run_tests.commands = $$DESTDIR/$$TARGET
QMAKE_EXTRA_TARGETS += run_tests
