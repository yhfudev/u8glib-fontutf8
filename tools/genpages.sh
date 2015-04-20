#!/bin/bash
#####################################################################
# genpages.sh for u8g
#
# This script will generate u8g c files for specified fonts
#
# Copyright 2015 Yunhui Fu
# License: GPL v3.0 or later
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

DN_CUR=$(pwd)

(cd ${DN_EXEC}; gcc -o genpages genpages.c getline.c)

rm tmpa tmpb
grep -Hrn _U8GT . | grep -v "#define" | sed 's/^.*_U8GT([ \w\t]*"\(.*\)"[ \w\t]*).*$/\1/' | ${DN_EXEC}/genpages  | sort | uniq | \
  while read PAGE ; do \
    ${DN_EXEC}/bdf2u8g -b 0 -e 127 -l ${PAGE} ${DN_EXEC}/unifont.bdf fontpage_${PAGE} fontpage_${PAGE}.h ;\
    sed -i 's|#include "u8g.h"|#include "utility/u8g.h"|' fontpage_${PAGE}.h
    echo "#include \"fontpage_${PAGE}.h\"" >> tmpa ;\
    echo "FONTDATA_ITEM(${PAGE}, fontpage_${PAGE})," >> tmpb ;\
  done

echo "#include \"fontutf8u8g.h\"" > fontutf8-data-sample.h
cat tmpa >> fontutf8-data-sample.h
echo "#define FONTDATA_ITEM(page, data) {page, NUM_ARRAY(data), data}" >> fontutf8-data-sample.h
echo "const fontdata_t g_fontdata[] = {" >> fontutf8-data-sample.h
cat tmpb >> fontutf8-data-sample.h
echo "};" >> fontutf8-data-sample.h
