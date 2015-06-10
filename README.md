FontUtf8
========

Show UTF-8 human readable strings from source code to your tiny LCD via u8glib

This is a language patch for U8G lib.
The original u8g support partially the char display of multi-language by re-coding the char values due to large size of font data and limited memory/flash of embedded systems.
It's very inconviniant to update the firmware if there's (menu) message changed by recoding all these added message chars.
What if we get all these steps done with the human readable messages in source code and processed by automatical tools while keep the compiled firmware as small as previous releases?
This patch will release the programmer from the bad experiences.

Basically, this tool will scan the source code of the firmware which using u8g lib, pick up the message strings tagged with "_U8GT()",
and then genrate all of font data structures and save it in .h files for u8g.

The firmware developer now can use plain text in their language source code and regenerate the langauge files once they change the message strings tagged with "_U8GT()" before compiling the souce code.
They should use the updated drawStr() function or utf8_draw() to display the strings.


Installation and usages
-----------------------


Prepare BDF font files, you may download a font from WQY, for example: http://wenq.org/daily/wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz

tar -xvf wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz
cp wqy-bitmapfont-gb18030/wenquanyi_12pt.bdf tools/

change the font file name in the bash script tools/genpages.sh

    FN_FONT=${DN_EXEC}/wenquanyi_12pt.bdf

generate the font file fontutf8-data.h

    cd src
    ../tools/genpages.sh

include the file in your source code:
    #include "fontutf8-data.h"


init the font data in setup() function
fontinfo_init(g_fontinfo, NUM_ARRAY(g_fontinfo));


declare all of the string in macro _U8GT(), for example:

     char s[] = _U8GT("黄沙百戰穿金甲，不破樓蘭終不還。");
    // and draw the string:
    utf8_draw (&u8g, 3, 30, s);

