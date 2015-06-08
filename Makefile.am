
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

bin_PROGRAMS=fonttest genpages

genpages_SOURCES= \
    tools/genpages.c \
    tools/getline.c \
    $(NULL)

fonttest_SOURCES= \
    fontutf8/fontutf8.cpp \
    u8gapi/fontutf8u8g.cpp \
    u8gapi/rbtree.cpp \
    $(NULL)

tools/genpages.sh: genpages
fontutf8/fontutf8-data.h: tools/genpages.sh
	cd fontutf8 && ../tools/genpages.sh
fontutf8/fontutf8.cpp: fontutf8/fontutf8-data.h

fonttest_CFLAGS=-Ifontutf8 -DU8G_RASPBERRY_PI @U8G_CFLAGS@
fonttest_CPPFLAGS=-Ifontutf8 -DU8G_RASPBERRY_PI $(U8G_CFLAGS)
fonttest_LDFLAGS=$(U8G_LDFLAGS)

# force the autoconf use g++ to link the c program with c++ lib
dummy.cpp:
	touch $@

#fonttest_LDADD = $(top_builddir)/src/libseederdict.la -ldl
fonttest_LDADD = -lU8glib -lwiringPi
