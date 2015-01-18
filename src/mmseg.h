#ifndef PHP_MMSEG
#define PHP_MMSEG

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define PHP_MMSEG_VERSION "0.0.1"
#define PHP_MMSEG_EXTNAME "mmseg"
#define PHP_MMSEG_LIB_VERSION "3.2.14"

#include <iostream>
#include <php.h>
#include <php_ini.h>
#include <mmseg/Segmenter.h>
#include <mmseg/SegmenterManager.h>
#include <Zend/zend_exceptions.h>
#include "ext/standard/info.h"

PHP_FUNCTION(mmseg_version);
static PHP_MINIT_FUNCTION(mmseg);
static PHP_MINFO_FUNCTION(mmseg);
static PHP_MSHUTDOWN_FUNCTION(mmseg);

PHP_INI_BEGIN()
PHP_INI_ENTRY("mmseg.data_dir", "/usr/local/etc", PHP_INI_ALL, NULL)
PHP_INI_ENTRY("mmseg.stopwords", "stopwords.txt", PHP_INI_ALL, NULL)
PHP_INI_END()

#define phpext_mmseg_ptr &mmseg_module_entry

#endif

