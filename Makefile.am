
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

dist_bin_SCRIPTS=tools/genpages.sh
bin_PROGRAMS=genpages
bin_PROGRAMS+=u8gsdl_fontutf8test
bin_PROGRAMS+=u8gwpi_fontutf8test

genpages_SOURCES= \
    tools/genpages.c \
    tools/getline.h \
    tools/getline.c \
    $(NULL)

u8gsdl_fontutf8test_SOURCES= \
    fontutf8test/fontutf8-data.h \
    fontutf8test/fontutf8test.cpp \
    src/fontutf8u8g.h \
    src/fontutf8u8g.cpp \
    src/rbtree.h \
    src/rbtree_augmented.h \
    src/rbtree.cpp \
    $(NULL)

u8gwpi_fontutf8test_SOURCES=$(u8gsdl_fontutf8test_SOURCES)

BUILT_SOURCES = fontutf8test/fontutf8-data.h
CLEANFILES = fontutf8test/fontutf8-data.h
nodist_program_SOURCES = fontutf8test/fontutf8-data.h
fontutf8test/fontutf8-data.h: fontutf8test/fontutf8test.cpp tools/genpages.sh
	cd fontutf8test && ../tools/genpages.sh
tools/genpages.sh: genpages tools/wenquanyi_12pt.bdf

tools/wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz:
	cd tools && wget http://wenq.org/daily/wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz

tools/wenquanyi_12pt.bdf: tools/wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz
	cd tools && tar -xvf wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz && cp wqy-bitmapfont-gb18030/wenquanyi_12pt.bdf .

# force the autoconf use g++ to link the c program with c++ lib
dummy.cpp:
	touch $@

u8gsdl_fontutf8test_CFLAGS=-Ifontutf8test -Isrc -DU8G_16BIT @U8G_CFLAGS@
u8gsdl_fontutf8test_CPPFLAGS=$(u8gsdl_fontutf8test_CFLAGS)
u8gsdl_fontutf8test_LDFLAGS=$(U8G_LDFLAGS)
u8gsdl_fontutf8test_LDADD = -lu8gbase -lu8gsdl
#u8gsdl_fontutf8test_LDADD = @U8G_PREFIX@/libu8gsdl.la

u8gwpi_fontutf8test_CFLAGS=-Ifontutf8test -Isrc -DU8G_16BIT -DU8G_RASPBERRY_PI -DU8G_WITH_PINLIST @U8G_CFLAGS@
u8gwpi_fontutf8test_CPPFLAGS=$(u8gwpi_fontutf8test_CFLAGS)
u8gwpi_fontutf8test_LDFLAGS=$(U8G_LDFLAGS)
u8gwpi_fontutf8test_LDADD = -lu8gbase -lu8gwpi -lwiringPi
#u8gwpi_fontutf8test_LDADD = -lu8gwpi -lwiringPi
#u8gwpi_fontutf8test_LDADD = @U8G_PREFIX@/libu8gwpi.la -lwiringPi

