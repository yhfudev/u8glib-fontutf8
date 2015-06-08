/**
 * @file    fontutf8u8g.h
 * @brief   font api for u8g lib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2015-02-19
 * @copyright GPL
 */
#ifndef FONTUTF8U8G_H
#define FONTUTF8U8G_H

/** Usage:
 * 1. ttf/oft to bdf
 * 2. bdf to u8g:
      cd to your application source directory, which contains files with _U8GT wrapped stings
      run genpages.sh
      rename fontutf8-data-sample.h to fontutf8-data.h
      place all of the files fontutf8-*.h to the same folder as fontutf8u8g.cpp
 * 3. compile your source file
 */

#include <U8glib.h>
#include "rbtree.h"

#define NUM_ARRAY(a) (sizeof(a)/sizeof(a[0]))
#define _U8GT(a) a

#define DEFAULT_FONT u8g_font_9x15

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _u8g_fontinfo_t {
    int page;
    int begin;
    int end;
    int size;
    const u8g_fntpgm_uint8_t *fntdata;

    struct rb_node node;
} u8g_fontinfo_t;

#ifdef __cplusplus
}
#endif

extern int fontinfo_init (u8g_fontinfo_t * fntinfo, int number);

extern void utf8_draw (U8GLIB *pdev, unsigned int x, unsigned int y, const char *msg);

#endif // FONTUTF8U8G_H
