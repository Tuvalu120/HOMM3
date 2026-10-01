TEMPLATE = subdirs

SUBDIRS += game tests trainer battle_example

QMAKE_TARGET_OUT = ../out

! isEmpty( BUILD_APPS ) {
    SUBDIRS = $$split(BUILD_APPS, ' ')
}

CONFIG += c++17

QMAKE_CXXFLAGS += -Werror

tests.depends = game
