#! /bin/sh
[ -e config.cache ] && rm -f config.cache

echo -n "testing for glibtoolize ... "
if glibtoolize --version >/dev/null 2>&1; then
  LIBTOOLIZE=glibtoolize
  echo "using glibtoolize"
else
  LIBTOOLIZE=libtoolize
  echo "using libtoolize"
fi
$LIBTOOLIZE --automake
aclocal ${OECORE_ACLOCAL_OPTS}
autoconf
autoheader
automake -a
./configure $@
exit
