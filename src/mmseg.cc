#include "parser.h"

css::SegmenterManager* psm_segmanager = NULL;
css::Segmenter* ps_seg = NULL;
std::vector<std::string>* pv_stopwords = NULL;
Parser* pp_parser = NULL;

void report_error(std::string s_error) {
	printf("Getting error %s \n", s_error.c_str());
}

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
 *  Function mmseg_synonyms
 *
 *  This function will generate the synonyms dictionary
 *
 *  @version 1.0
 *
 *******************************************************************************/
PHP_FUNCTION(mmseg_synonyms) {
	char* s_inputfile = NULL;
	int inputfile_len = 0;
	char* s_outfile = NULL;
	int outfile_len = 0;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &s_inputfile, &inputfile_len,
				&s_outfile, &outfile_len) == SUCCESS) {
		if(file_exists(s_inputfile)) {
			css::SynonymsDict dict;
			dict.import(s_inputfile);
			dict.save(s_outfile);
			RETURN_TRUE;
		}
	}
	RETURN_FALSE;
}

/*******************************************************************************
 *
 *  Function mmseg_thesaurus
 *
 *  This function will generate the thesaurus dictionary
 *
 *  @version 1.0
 *
 *******************************************************************************/
PHP_FUNCTION(mmseg_thesaurus) {
	char* s_inputfile = NULL;
	int inputfile_len = 0;
	char* s_outfile = NULL;
	int outfile_len = 0;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &s_inputfile, &inputfile_len,
				&s_outfile, &outfile_len) == SUCCESS) {
		if(file_exists(s_inputfile)) {
			css::ThesaurusDict tdict;
			tdict.import(s_inputfile, s_outfile);    
			RETURN_TRUE;
		}
	}
	RETURN_FALSE;
}

/*******************************************************************************
 *
 *  Function mmseg_create_dict
 *
 *  This function will create a new mmseg dict using the file name in the args
 *
 *  @version 1.0
 *
 *******************************************************************************/
PHP_FUNCTION(mmseg_create_dict) {
	char* s_inputfile = NULL;
	int inputfile_len = 0;
	char* s_outfile = NULL;
	int outfile_len = 0;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &s_inputfile, &inputfile_len,
				&s_outfile, &outfile_len) == SUCCESS) {

		if(file_exists(s_inputfile)) {
			css::UnigramCorpusReader ur;
			ur.open(s_inputfile,"plain");

			css::UnigramDict ud;
			int ret = ud.import(ur);
			ud.save(s_outfile);
			RETURN_TRUE;
		}
		else {
			char buf[256];
			sprintf(buf, "The input file %s didn't exists!\n", s_inputfile);
			report_error(buf);
		}
	}
	else {
		report_error("Can't find the input and output file!");
	}
	RETURN_FALSE;
}

/*******************************************************************************
 *
 *  Function mmseg_tokenize
 *
 *  This function will tokenize the input string using libmmseg and return
 *  the tokenized result(as a 2 elements array, first is the type and the second
 *  is the result array
 *
 *  @version 1.0
 *
 *******************************************************************************/
PHP_FUNCTION(mmseg_tokenize) {
	char* s_str = NULL;
	int str_len = 0;
	zval* pzv_out = NULL;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &s_str, &str_len,
			&pzv_out) == SUCCESS) {

		pp_parser->set(s_str, str_len);

		char result[1024];
		TOKEN_TYPE t;
		while((t = pp_parser->next(result))) {
			zval* pzv_item = NULL;
			MAKE_STD_ZVAL(pzv_item);
			array_init_size(pzv_item, 2);

			// Add the type as the first item
			zval* pzv_type = NULL;
			MAKE_STD_ZVAL(pzv_type);
			ZVAL_STRING(pzv_type, t, true);
			add_next_index_zval(pzv_item, pzv_type);
			
			// Add the token as the next item
			zval* pzv_token = NULL;
			MAKE_STD_ZVAL(pzv_token);
			ZVAL_STRING(pzv_token, result, true);
			add_next_index_zval(pzv_item, pzv_token);

			add_next_index_zval(pzv_out, pzv_item);
		}
		RETURN_TRUE;
	}
	RETURN_FALSE;
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

	char* s_datadir = INI_STR("mmseg.data_dir");
	char* s_stopwords = INI_STR("mmseg.stopwords");
	if(file_exists(s_datadir) && file_exists(s_stopwords)) {
		psm_segmanager = new css::SegmenterManager();
		psm_segmanager->init(s_datadir);
		ps_seg = psm_segmanager->getSegmenter();

		// Read stop words
		pv_stopwords = new std::vector<std::string>();
		std::ifstream _file(s_stopwords);
		std::copy(std::istream_iterator<std::string>(_file),
			std::istream_iterator<std::string>(),
			std::back_inserter(*pv_stopwords));
		std::sort(pv_stopwords->begin(),pv_stopwords->end());

		pp_parser = new Parser();
	}
	else {
		report_error("Can't find the configuration files for mmseg!");
		return -1;
	}
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
	if(psm_segmanager) { // Delete the objects if we have really created them
		delete pv_stopwords;
		delete psm_segmanager;
		delete pp_parser;
	}
	return SUCCESS;
}

static zend_function_entry mmseg_functions[] = {
    PHP_FE(mmseg_version, NULL)   
    PHP_FE(mmseg_tokenize, NULL)   
    PHP_FE(mmseg_create_dict, NULL)   
    PHP_FE(mmseg_thesaurus, NULL)   
    PHP_FE(mmseg_synonyms, NULL)   
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
