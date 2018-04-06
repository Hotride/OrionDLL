#-------------------------------------------------
#
# Project created by QtCreator 2016-10-04T12:53:13
#
#-------------------------------------------------
VERSION = 1.0.2.0

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += NOMINMAX

LIBS += -lgdi32 -lMsimg32 -lWinmm -lUser32 -lShell32

TARGET = Orion
TARGET_EXT = .dll
TEMPLATE = lib
CONFIG += dll

CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/Orion/

QMAKE_LFLAGS +=-static-libgcc -static-libstdc++

SOURCES += \
		Orion/dllmain.cpp \
	Orion/DataStream.cpp \
	Orion/Global.cpp \
	Orion/Crypt/GameCrypt.cpp \
	Orion/Crypt/LoginCrypt.cpp \
	Orion/Crypt/md5.cpp \
	Orion/Crypt/twofish.cpp

unix {
        target.path = /usr/lib
        INSTALLS += target
}

HEADERS += \
	Orion/DataStream.h \
	Orion/Global.h \
	Orion/OrionDef.h \
	Orion/resource.h \
	Orion/Crypt/AES.H \
	Orion/Crypt/GameCrypt.h \
	Orion/Crypt/LoginCrypt.h \
	Orion/Crypt/md5.h \
	Orion/Crypt/platform.H \
	Orion/Crypt/TABLE.H
