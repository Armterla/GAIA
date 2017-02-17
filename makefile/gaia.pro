TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= c++11
CONFIG += warn_off

mac{
	LIBS += -liconv
}

INCLUDEPATH += ../include/
HEADERS += ../include/*.h
win32{
    SOURCES += ..\src\*.c
    SOURCES += ..\src\*.cpp
}
!win32{
    SOURCES += ../src/*.c
    SOURCES += ../src/*.cpp
}


INCLUDEPATH += ../test/
HEADERS += ../test/*.h
win32{
    SOURCES += ..\test\*.cpp
}
!win32{
    SOURCES += ../test/*.cpp
}

SOURCES += main.cpp

#Pre-Header
windows{
	PRECOMPILED_HEADER = ..\test\preheader.h
}
