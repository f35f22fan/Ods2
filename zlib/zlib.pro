TEMPLATE = lib

#message($$QMAKESPEC)

linux-g++ {
    #message(Linux)
}

VERSION=1.3.1
CONFIG += dll cmdline release largefile
TARGET=zlib
DESTDIR=$$PWD/../lib
DEFINES += STDC
INCLUDEPATH +=


#include(zlib_defines.pro)

#win32:INCLUDEPATH += "C:/mylibs/extra headers"
#unix:INCLUDEPATH += "/home/user/extra headers"

#win32:LIBS += "C:/mylibs/extra libs/extra.lib"
#unix:LIBS += "-L/home/user/extra libs" -lextra

SOURCES += adler32.c compress.c crc32.c deflate.c gzclose.c gzlib.c \
gzread.c gzwrite.c infback.c inffast.c inflate.c inftrees.c trees.c \
zutil.c

HEADERS += crc32.h deflate.h gzguts.h inffast.h inffixed.h inflate.h inftrees.h \
trees.h zconf.h zlib.h zutil.h
