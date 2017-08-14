TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


INCLUDEPATH += /home/pasquda/c++_libs/boost_1_64_0
LIBS += -L/home/pasquda/c++_libs/boost_1_64_0 -lboost_system -lboost_chrono -lboost_thread -lboost_timer -lpthread -lboost_serialization

HEADERS += \
    stock.h \
    connection.h \
    client.h
