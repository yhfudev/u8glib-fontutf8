#!/bin/bash
#####################################################################
# genpages.sh for u8g
#
# This script will generate u8g c files for specified fonts
#
# Copyright 2015 Yunhui Fu
# License: GPL/BSD
#####################################################################
my_getpath () {
  PARAM_DN="$1"
  shift
  #readlink -f
  DN="${PARAM_DN}"
  FN=
  if [ ! -d "${DN}" ]; then
    FN=$(basename "${DN}")
    DN=$(dirname "${DN}")
  fi
  cd "${DN}" > /dev/null 2>&1
  DN=$(pwd)
  cd - > /dev/null 2>&1
  echo "${DN}/${FN}"
}
#DN_EXEC=`echo "$0" | ${EXEC_AWK} -F/ '{b=$1; for (i=2; i < NF; i ++) {b=b "/" $(i)}; print b}'`
DN_EXEC=$(dirname $(my_getpath "$0") )
if [ ! "${DN_EXEC}" = "" ]; then
    DN_EXEC="$(my_getpath "${DN_EXEC}")/"
else
    DN_EXEC="${DN_EXEC}/"
fi
#####################################################################

FN_FONT=${DN_EXEC}/unifont.bdf
FN_FONT=${DN_EXEC}/wenquanyi_12pt.bdf

DN_CUR=$(pwd)

(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

rm tmpa tmpb
#rm -f ${DN_EXEC}/fontpage_*.h
rm -f fontpage_*.h

cat << EOF > tmp-proc-page.awk
BEGIN {
    cur_page=0;
    val_begin=0;
    val_pre=0;
}{
    page=\$1;
    val_real=\$2;
    # assert (val_real < 128);
    val=val_real + 128;
    if (cur_page != page) {
        if (cur_page != 0) {
            if (val_begin != 0) {
                print cur_page " " val_begin " " val_pre;
            }
        }
        cur_page=page;
        val_begin=val;
        val_pre=val;
    } else {
        if (val_pre + 1 != val) {
            if (cur_page != 0) {
                print cur_page " " val_begin " " val_pre;
            }
            val_begin=val;
            val_pre=val;
        } else {
            val_pre = val;
        }
    }
} END {
    if (cur_page != 0) {
        print cur_page " " val_begin " " val_pre;
    }
}
EOF

grep -Hrn _U8GT . | grep -v "#define" | grep '"' | \
  sed 's/^.*_U8GT([ \w\t]*"\([^)]*\)"[ \w\t]*).*$/\1/' | \
  ${DN_EXEC}/genpages | \
  sort -k 1n -k 2n | uniq | \
  gawk -v EXEC_PREFIX=${DN_EXEC} -f tmp-proc-page.awk | \
  while read PAGE BEGIN END; do \
    if [ ! -f ${DN_EXEC}/fontpage_${PAGE}_${BEGIN}_${END}.h ]; then \
      ${DN_EXEC}/bdf2u8g -u ${PAGE} -b ${BEGIN} -e ${END} ${FN_FONT} fontpage_${PAGE}_${BEGIN}_${END} ${DN_EXEC}/fontpage_${PAGE}_${BEGIN}_${END}.h > /dev/null 2>&1 ;
      #sed -i 's|#include "u8g.h"|#include "utility/u8g.h"|' ${DN_EXEC}/fontpage_${PAGE}_${BEGIN}_${END}.h ;
    fi ;\
    grep -A 10000000000 u8g_fntpgm_uint8_t ${DN_EXEC}/fontpage_${PAGE}_${BEGIN}_${END}.h >> tmpa ;\
    echo "    FONTDATA_ITEM(${PAGE}, ${BEGIN}, ${END}, fontpage_${PAGE}_${BEGIN}_${END})," >> tmpb ;\
  done

rm -f fontutf8-data.h
echo "#include <u8g.h>" >> fontutf8-data.h
echo "#include \"fontutf8u8g.h\"" >> fontutf8-data.h
echo "" >> fontutf8-data.h
cat tmpa >> fontutf8-data.h
echo "" >> fontutf8-data.h
echo "#define FONTDATA_ITEM(page, begin, end, data) {page, begin, end, NUM_ARRAY(data), data}" >> fontutf8-data.h
echo "u8g_fontinfo_t g_fontinfo[] = {" >> fontutf8-data.h
cat tmpb >> fontutf8-data.h
echo "};" >> fontutf8-data.h
