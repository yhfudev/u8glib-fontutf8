/**
 * @file    fontutf8u8g.h
 * @brief   font api for u8g lib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2015-02-19
 * @copyright GPL/BSD
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

#include <u8g.h>

#define NUM_ARRAY(a) (sizeof(a)/sizeof(a[0]))
#define _U8GT(a) a

#define DEFAULT_FONT u8g_font_9x15

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _uxg_fontinfo_t {
    uint16_t page;
    uint8_t begin;
    uint8_t end;
    uint16_t size;
    const u8g_fntpgm_uint8_t *fntdata;

} uxg_fontinfo_t;

extern int fontinfo_init1 (const uxg_fontinfo_t * fntinfo, int number);
extern char fontinfo_isinited1(void);
//void u8g_SetUtf8Fonts (uxg_fontinfo_t * fntinfo, int number);
//char u8g_Utf8FontIsInited(void);
#define u8g_SetUtf8Fonts1        fontinfo_init1
#define u8g_Utf8FontIsInited1    fontinfo_isinited1

void u8g_DrawUtf8Str1 (u8g_t *pu8g, unsigned int x, unsigned int y, const char *utf8_msg);

#ifdef __cplusplus
}
#endif

#endif // FONTUTF8U8G_H
