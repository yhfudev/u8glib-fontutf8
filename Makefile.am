
AM_LDFLAGS=
AM_CPPFLAGS=

DEFS+= \
    `getconf LFS_CFLAGS` \
    `getconf LFS64_CFLAGS` \
    -D_GNU_SOURCE \
    -D_FILE_OFFSET_BITS=64 \
    $(NULL)

AM_CPPFLAGS+= \
    -I$(top_srcdir)/src \
    -I$(top_builddir)/include/ \
    $(NULL)

AM_LDFLAGS += \
    -L$(top_builddir)/src/ \
    `getconf LFS_LDFLAGS` \
    `getconf LFS64_LDFLAGS` \
    $(NULL)

if DEBUG
# use "valgrind --tool=memcheck --leak-check=yes" to check memory leak, MemWatch will drag the program.
#DEFS+=-DMEMWATCH
DEFS+= -DDEBUG=1
AM_CPPFLAGS+=-g -Wall

else
AM_CPPFLAGS+=-O3 -Wall
endif

EXT_FLAGS=
@MK@GITNUMTMP=$(shell cd "$(top_srcdir)"; A=$$(git show | head -n 1 | awk '{print $$2}'); echo $${A:0:7}; cd - > /dev/null )
#@MK@SVNNUMTMP=$(shell cd "$(top_srcdir)"; LC_ALL=C svn info | grep -i Revision | awk '{print $$2}'; cd - > /dev/null )
#@MK@ifeq ($(SVNNUMTMP),)
#EXT_FLAGS+= -DSVN_VERSION='"${GITNUMTMP}"'
#@MK@else
#EXT_FLAGS+= -DSVN_VERSION='"${SVNNUMTMP}"'
#@MK@endif
@MK@ifeq ($(GITNUMTMP),)
@MK@else
EXT_FLAGS+= -DSVN_VERSION='"${GITNUMTMP}"'
@MK@endif
DEFS+=$(EXT_FLAGS)

bin_PROGRAMS=genpages u8gsdl_fonttest

genpages_SOURCES= \
    tools/genpages.c \
    tools/getline.c \
    $(NULL)

u8gsdl_fonttest_SOURCES= \
    fontutf8/fontutf8.cpp \
    u8gapi/fontutf8u8g.cpp \
    u8gapi/rbtree.cpp \
    $(NULL)

BUILT_SOURCES = fontutf8/fontutf8-data.h
CLEANFILES = fontutf8/fontutf8-data.h
nodist_program_SOURCES = fontutf8/fontutf8-data.h
fontutf8/fontutf8-data.h: tools/genpages.sh
	cd fontutf8 && ../tools/genpages.sh

tools/genpages.sh: genpages
fontutf8/fontutf8.cpp: fontutf8/fontutf8-data.h

u8gsdl_fonttest_CFLAGS=-Ifontutf8 -DU8G_16BIT -DU8G_RASPBERRY_PI -DU8G_WITH_PINLIST @U8G_CFLAGS@
u8gsdl_fonttest_CFLAGS=-Ifontutf8 -DU8G_16BIT @U8G_CFLAGS@
u8gsdl_fonttest_CPPFLAGS=$(u8gsdl_fonttest_CFLAGS)
u8gsdl_fonttest_LDFLAGS=$(U8G_LDFLAGS)

# force the autoconf use g++ to link the c program with c++ lib
dummy.cpp:
	touch $@

u8gsdl_fonttest_LDADD = -lu8gbase -lu8gsdl
#u8gsdl_fonttest_LDADD = -lu8gbase -lu8gwpi -lwiringPi
#u8gsdl_fonttest_LDADD = @U8G_PREFIX@/libu8gwpi.la -lwiringPi
#u8gsdl_fonttest_LDADD = @U8G_PREFIX@/libu8gsdl.la
