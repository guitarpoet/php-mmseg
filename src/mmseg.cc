#include "mmseg.h"

/*******************************************************************************
 *
 *  Function mmseg_version
 *
 *  This function will return the version of the libmmseg
 *
 *  @version 1.0
 *
 *******************************************************************************/
PHP_FUNCTION(mmseg_version) {
	RETURN_STRING(PHP_MMSEG_LIB_VERSION, true);
}

/*******************************************************************************
 *
 *  Init Function mmseg
 *
 *  This function will init the mmseg segmenter
 *
 *  @version 1.0
 *
 *******************************************************************************/
static PHP_MINIT_FUNCTION(mmseg) {
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}

/*******************************************************************************
 *
 *  Info Function
 *
 *  This function will show the plugins information
 *
 *  @version 1.0
 *
 *******************************************************************************/
static PHP_MINFO_FUNCTION(mmseg) {
    php_info_print_table_start();
    php_info_print_table_row(2, "Revision", PHP_MMSEG_VERSION);
    php_info_print_table_end();
    DISPLAY_INI_ENTRIES();    
}

static PHP_MSHUTDOWN_FUNCTION(mmseg) {
    UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

static zend_function_entry mmseg_functions[] = {
    PHP_FE(mmseg_version, NULL)   
	PHP_FE_END
};
  
zend_module_entry mmseg_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,    
#endif
    PHP_MMSEG_EXTNAME,         
    mmseg_functions,           
    PHP_MINIT(mmseg),
	PHP_MSHUTDOWN(mmseg),	
    NULL,
    NULL,
	PHP_MINFO(mmseg),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_MMSEG_VERSION,         
#endif
    STANDARD_MODULE_PROPERTIES 
};
  
#ifdef COMPILE_DL_MMSEG
ZEND_GET_MODULE(mmseg)
#endif
