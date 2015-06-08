/**
 * @file    fontutf8u8g.c
 * @brief   font api for u8g lib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2015-02-19
 * @copyright GPL
 */

//#include <Arduino.h>
#include <U8glib.h>

#include "rbtree.h"
#include "fontutf8u8g.h"

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define FALSE 0
#define TRUE  1

typedef struct _u8g_fontinfo_t {
    int page;
    int begin;
    int end;
    int size;
    const u8g_fntpgm_uint8_t *fntdata;

    struct rb_node node;
} u8g_fontinfo_t;

#define assert(a) if (!(a)) {printf("Assert: " # a); exit(1);}

#define g_fontinfo_size NUM_ARRAY(g_fontinfo)
#include "fontutf8-data.h"

struct rb_root g_fontinfo_root = RB_ROOT;
char flag_fontinfo_inited = 0;

wchar_t
get_val_utf82uni (uint8_t *pstart)
{
    size_t cntleft;
    wchar_t retval = 0;

    if (0 == (0x80 & *pstart)) {
        return *pstart;
    }

    if (((*pstart & 0xE0) ^ 0xC0) == 0) {
        cntleft = 1;
        retval = *pstart & ~0xE0;
    } else if (((*pstart & 0xF0) ^ 0xE0) == 0) {
        cntleft = 2;
        retval = *pstart & ~0xF0;
    } else if (((*pstart & 0xF8) ^ 0xF0) == 0) {
        cntleft = 3;
        retval = *pstart & ~0xF8;
    } else if (((*pstart & 0xFC) ^ 0xF8) == 0) {
        cntleft = 4;
        retval = *pstart & ~0xFC;
    } else if (((*pstart & 0xFE) ^ 0xFC) == 0) {
        cntleft = 5;
        retval = *pstart & ~0xFE;
    } else {
        /* encoding error */
        cntleft = 0;
        retval = 0;
    }
    pstart ++;
    for (; cntleft > 0; cntleft --) {
        retval <<= 6;
        retval |= *pstart & 0x3F;
        pstart ++;
    }
    return retval;
}

/**
 * @brief 转换 UTF-8 编码的一个字符为本地的 Unicode 字符(wchar_t)
 *
 * @param pstart : 存储 UTF-8 字符的指针
 * @param pval : 需要返回的 Unicode 字符存放地址指针
 *
 * @return 成功返回下个 UTF-8 字符的位置
 *
 * 转换 UTF-8 编码的一个字符为本地的 Unicode 字符(wchar_t)
 */
uint8_t *
get_utf8_value (uint8_t *pstart, wchar_t *pval)
{
    uint32_t val = 0;
    uint8_t *p = pstart;
    /*size_t maxlen = strlen (pstart);*/

    assert (NULL != pstart);

    if (0 == (0x80 & *p)) {
        val = (size_t)*p;
        p ++;
    } else if (0xC0 == (0xE0 & *p)) {
        val = *p & 0x1F;
        val <<= 6;
        p ++;
        val |= (*p & 0x3F);
        p ++;
        assert ((wchar_t)val == get_val_utf82uni (pstart));
    } else if (0xE0 == (0xF0 & *p)) {
        val = *p & 0x0F;
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        p ++;
        assert ((wchar_t)val == get_val_utf82uni (pstart));
    } else if (0xF0 == (0xF8 & *p)) {
        val = *p & 0x07;
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        p ++;
        assert ((wchar_t)val == get_val_utf82uni (pstart));
    } else if (0xF8 == (0xFC & *p)) {
        val = *p & 0x03;
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        p ++;
        assert ((wchar_t)val == get_val_utf82uni (pstart));
    } else if (0xFC == (0xFE & *p)) {
        val = *p & 0x01;
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        val <<= 6; p ++;
        val |= (*p & 0x3F);
        p ++;
        assert ((wchar_t)val == get_val_utf82uni (pstart));
    } else if (0x80 == (0xC0 & *p)) {
        /* error? */
        for (; 0x80 == (0xC0 & *p); p ++);
    } else {
        /* error */
        for (; ((0xFE & *p) > 0xFC); p ++);
    }
    /*if (val == 0) {
        p = NULL;*/
/*
    } else if (pstart + maxlen < p) {
        p = pstart;
        if (pval) *pval = 0;
    }
*/

    if (pval) *pval = val;

    return p;
}

/* return v1 - v2 */
int
fontinfo_compare (u8g_fontinfo_t * v1, u8g_fontinfo_t * v2)
{
    assert (NULL != v1);
    assert (NULL != v2);
    if (v1->page < v2->page) {
        return -1;
    } else if (v1->page > v2->page) {
        return 1;
    }
    if (v1->end < v2->begin) {
        return -1;
    } else if (v1->begin > v2->end) {
        return 1;
    }
    return 0;
}

static int
fontinfo_insert(struct rb_root *root, u8g_fontinfo_t *data)
{
    struct rb_node **new1 = &(root->rb_node), *parent = NULL;

    // Figure out where to put new node
    while (*new1) {
        u8g_fontinfo_t *this1 = container_of(*new1, u8g_fontinfo_t, node);

        int result = fontinfo_compare (data, this1);

        parent = *new1;
        if (result < 0) {
            new1 = &((*new1)->rb_left);
        } else if (result > 0) {
            new1 = &((*new1)->rb_right);
        } else {
            return FALSE;
        }
    }

    // Add new node and rebalance tree.
    rb_link_node(&data->node, parent, new1);
    rb_insert_color(&data->node, root);

    return TRUE;
}

int
fontinfo_init_internal (u8g_fontinfo_t * fntinfo, int number)
{
    struct rb_root *root = &g_fontinfo_root;
    int i;

    for (i = 0; i < number; i ++) {
        fontinfo_insert (root, &fntinfo[i]);
    }
    return 0;
}

int
fontinfo_init (void)
{
    int ret = 0;
    if (! flag_fontinfo_inited) {
        ret = fontinfo_init_internal(g_fontinfo, NUM_ARRAY(g_fontinfo));
    }
    if (ret >= 0) {
        flag_fontinfo_inited = 1;
    }
    return ret;
}

const u8g_fntpgm_uint8_t *
fontinfo_find (wchar_t val)
{
    struct rb_root *root = &g_fontinfo_root;
    struct rb_node *node = root->rb_node;
    int i;
    // calculate the page
    u8g_fontinfo_t vcmp = {val / 128, val % 128 + 128, val % 128 + 128, 0, 0};

    if (val < 128) {
        return DEFAULT_FONT; //u8g_font_gdr25;
    }
    if (NULL == g_fontinfo) {
        return NULL;
    }
    fontinfo_init();

    while (node) {
        int result;
        u8g_fontinfo_t *data = container_of(node, u8g_fontinfo_t, node);

        result = fontinfo_compare (&vcmp, data);

        if (result < 0) {
            node = node->rb_left;
        } else if (result > 0) {
            node = node->rb_right;
        } else {
            return data->fntdata;
        }
    }
    return NULL;
}

void
utf8_draw (U8GLIB *pdev, unsigned int x, unsigned int y, const char *msg)
{
    int len;
    uint8_t *pend = NULL;
    uint8_t *p;
    wchar_t val;
    uint8_t buf[2] = {0, 0};
    u8g_fntpgm_uint8_t * fntpqm = NULL;

    //pdev->drawStr(x, y, msg);
    len = strlen(msg);
    pend = (uint8_t *)msg + len;
    for (p = (uint8_t *)msg; p < pend; ) {
        val = 0;
        p = get_utf8_value(p, &val);
        if (NULL == p) {
            break;
        }
        buf[0] = (uint8_t)(val & 0x7F);
        buf[0] |= 0x80; // use upper page to avoid 0x00 error in C. you may want to generate the font data by bdf2u8g -b 128 -e 255 -u ${PAGE} ....
        fntpqm = (u8g_fntpgm_uint8_t *)fontinfo_find (val);
        if (NULL == fntpqm) {
            //continue;
            buf[0] = '?';
            pdev->setFont (DEFAULT_FONT);
        } else {
            pdev->setFont (fntpqm);
        }

        pdev->drawStr(x, y, (char *) buf);
        x += pdev->getStrWidth((char *)buf);
    }
}

