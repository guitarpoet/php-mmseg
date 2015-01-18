#include "parser.h"

bool str_eql(const char* str1, const char* str2) {
    return strncmp(str1, str2, sizeof(str2) - 1) == 0;
}

bool Parser::filterToken(const char* token, u2 len, u2 symlen) {
     // 锟斤拷
    char txtHead[3] = {(char)239,(char)187,(char)191};

    // Check if the token is the UTF8's magic word
    if(len == 3 && memcmp(token,txtHead,sizeof(char)*3) == 0){
        //check is 0xFEFF
        return true;
    }

    char result[256];
    sprintf(result, "%*.*s", symlen, symlen, token);
    if(str_eql(result, "") || str_eql(result, " ") || str_eql(result, "\t")) {
        return true;
    }

    if(isStopWord(result)) {
        return true;
    }

    return false;
}

bool Parser::peek(token_peek &peek) {
	if(!ps_seg)
		return false;

    u2 len = 0, symlen = 0;
   
    // Getting the token
    char* tok = (char*)ps_seg->peekToken(len, symlen);
    ps_seg->popToken(len);

    if(!tok || !*tok || !len) // We have reach the end.
        return false;

    // If we can still get the token
    peek.tok = tok;
    peek.len = len;
    peek.symlen = symlen;
    if(*tok == '\r' || *tok == '\n') { // The line break should be ignored
        peek.type = TOKEN_TYPE_LINE_BREAK;
    }
    else {
        if(!filterToken(tok, len, symlen)) {
            peek.type = TOKEN_TYPE_TOKEN;
        }
        else {
            peek.type = TOKEN_TYPE_STOP_WORD;
        }
    }
    return true;
}

TOKEN_TYPE Parser::next(char* result) {
    token_peek t = {
        NULL, 0, 0, NULL
    };
    if(peek(t)) {
        sprintf(result, "%*.*s", t.symlen, t.symlen, t.tok);
        return t.type;
    }
    return NULL;
}
