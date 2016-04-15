TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += precompile_header
PRECOMPILED_HEADER = stdafx.h
TARGET = bin/rpn_expressions

# LIBS += -lboost_unit_test_framework

SOURCES += main.cpp \
    Operation.cpp \
    RPNExpressionTokenizer.cpp \
    RPNExpressionTokenizerTests.cpp \
    InfixExpressionCalculator.cpp \
    InfixExpressionCalculator_tests.cpp

HEADERS += \
    Operation.h \
    RPNExpressionTokenizer.h \
    stdafx.h \
    InfixExpressionCalculator.h
