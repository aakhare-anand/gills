#ifndef __SIMPLEX_SCANNER_H__
#define __SIMPLEX_SCANNER_H__

#include <regex.h>
#include "gillsfileop.h"
#include "gills_app.h"

#define SIMPLEX_END_OF_SCAN       -1
#define SIMPLEX_NO_MATCH          -2
#define SIMPLEX_FILE_READ_ERROR   -3

typedef void* yyscan_t;

typedef struct simplex_token_ {
    char *regexstr;
    int token_num;
} simplex_token_t;

typedef struct simplex_ {
    regex_t *tokens_regex;
    simplex_token_t *tokens;
    int tokens_num;
//    char scan_buf[4096];
    char *scan_buf;
    int scan_matchlen;
    void *yylval;
    gills_fread_t *fread;
} simplex_t;

simplex_t* simplex_init(simplex_token_t *tokens, int tokens_num,
                         gills_fread_t *fread, void *yylval);
void simplex_cleanup(simplex_t *simplex);
int yysimplex(void *yylval, yyscan_t yyscan);
int yylex(YYSTYPE *yylval, yyscan_t *yyscan);
YYSTYPE * yyget_lval(yyscan_t yyscanner);
void yyset_lval(YYSTYPE * yylval_param, yyscan_t yyscanner);

#endif /* __SIMPLEX_SCANNER_H__ */
