# 首发于Qt君公众号
QT += gui

CONFIG += c++11 console
CONFIG -= app_bundle debug_and_release

HEADERS += Object2Json.h

SOURCES += \
        main.cpp

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
