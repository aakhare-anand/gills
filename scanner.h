#ifndef __SIMPLEX_SCANNER_H__
#define __SIMPLEX_SCANNER_H__

#include <regex.h>

#define SIMPLEX_NO_MATCH -2

typedef void* yyscan_t;

typedef struct simplex_token_ {
    char *regexstr;
    int token_num;
} simplex_token_t;

typedef struct simplex_ {
    regex_t *regex;
    simplex_token_t *tokens;
    int tokens_num;
    char *scan_buf;
    int scan_matchlen;
    void *yylval;
} simplex_t;

void simplex_regexcomp();

#endif /* __SIMPLEX_SCANNER_H__ */
