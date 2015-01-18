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
PHP_INI_ENTRY("mmseg.stopwords", "/usr/local/etc/stopwords.txt", PHP_INI_ALL, NULL)
PHP_INI_END()

extern css::SegmenterManager* psm_segmanager;
extern css::Segmenter* ps_seg; 
extern std::vector<std::string>* stopwords;

inline bool file_exists (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
} 

#define phpext_mmseg_ptr &mmseg_module_entry

#endif

