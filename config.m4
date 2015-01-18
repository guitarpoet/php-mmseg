PHP_ARG_ENABLE(mmseg, whether to enable mmseg tokenizer support, [ --enable-mmseg Enable mmseg support], yes, yes)

if test "$PHP_MMSEG" != "no"; then 
	if test "$PHP_MMSEG" != "yes"; then
		LIBMMSEG_SEARCH_DIRS=$PHP_MMSEG
	else
		LIBMMSEG_SEARCH_DIRS="/usr/local/include /usr/include /opt/local/include /usr/local/src /opt/local/src"
	fi

	for i in $LIBMMSEG_SEARCH_DIRS; do
		if test -f $i/mmseg/Segmenter.h; then
			LIBMMSEG_INCDIR=$i
		fi
	done

	LIBMMSEG_LIB_SEARCH_DIRS="/usr/local/lib /usr/lib /opt/local/lib"
	for i in $LIBMMSEG_LIB_SEARCH_DIRS; do
		if test -f $i/libmmseg.a; then
			LIBMMSEG_LD_DIR=$i
		fi
	done

	if test -z "$LIBMMSEG_INCDIR"; then
		AC_MSG_ERROR(Cannot find libmmseg at /usr/local or /opt/local !)
	fi

	if test -z "$LIBMMSEG_LD_DIR"; then
		AC_MSG_ERROR(Cannot find libmmseg.a at /usr/lib, /usr/local/lib or /opt/local/lib !)
	fi

	PHP_REQUIRE_CXX()
	PHP_ADD_LIBRARY(stdc++, 1, PHP5CPP_SHARED_LIBADD)
	PHP_ADD_LIBRARY_WITH_PATH(mmseg, $LIBMMSEG_LD_DIR, LIBMMSEG_STATIC_LIBADD)
	LDFLAGS="-lmmseg -lstdc++"
	PHP_ADD_INCLUDE($LIBMMSEG_INCDIR)
    PHP_NEW_EXTENSION(mmseg, src/mmseg.cc src/parser.cc, $ext_shared,,"-Wno-c++11-compat-deprecated-writable-strings",yes)
fi
