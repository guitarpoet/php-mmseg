#ifndef PHP_MMSEG_PARSER
#define PHP_MMSEG_PARSER

#include "mmseg.h"

#define TOKEN_TYPE_TOKEN "TOKEN"
#define TOKEN_TYPE_LINE_BREAK "LINE_BREAK"
#define TOKEN_TYPE_STOP_WORD "STOP_WORD"

typedef const char* TOKEN_TYPE;

bool str_eql(const char* str1, const char* str2);

struct TokenPeek {
    char* tok;
    u2 len;
    u2 symlen;
    TOKEN_TYPE type;
};

typedef struct TokenPeek token_peek;

class Parser {
private: 
	bool isStopWord(std::string item) {
		if(pv_stopwords)
			return std::binary_search(pv_stopwords->begin(), pv_stopwords->end(), item);
		return false;
	}
	
	bool filterToken(const char* token, u2 len, u2 symlen);

public:
	void set(const char* buffer, int length) {
		if(ps_seg) {
			ps_seg->setBuffer((u1*)buffer, length);
		}
	}

	bool peek(token_peek &peek);

	TOKEN_TYPE next(char* result);
};

#endif

