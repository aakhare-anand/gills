#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "gills.h"
#include "simplex.h"
#include "gillsfileop.h"

simplex_t* simplex_init (simplex_token_t *tokens, int tokens_num,
                         gills_fread_t *fread, void *yylval)
{
    simplex_t *simplex;
    int i, j;
    int ret;

    simplex = (simplex_t *)calloc(1, sizeof(simplex_t));
    if (!simplex) {
        printf("mem alloc failed for simplex ...exiting\n");
        return NULL;
    }

    simplex->tokens = tokens;
    simplex->tokens_num = tokens_num;
    simplex->fread = fread;
    simplex->yylval = yylval;

    simplex->scan_buf = (char *)calloc(fread->min_bufread_len, sizeof(char));
    if (!simplex->scan_buf) {
        printf("mem alloc failed for simplex scan buffer ...exiting\n");
        return NULL;
    }
/*
    simplex->tokens_regex = (regex_t **)calloc(simplex->tokens_num, sizeof(regex_t *));
*/

    simplex->tokens_regex = (regex_t *)calloc(simplex->tokens_num, sizeof(regex_t));
    if (!simplex->tokens_regex) {
        printf("simplex regex mem alloc failed ...exiting\n");
        return NULL;
    }

/*
    simplex->tokens = tokens;
    simplex->tokens_num = tokens_num;
    simplex->fread = fread;
    simplex->yylval = yylval;
*/
    for (i = 0; i < simplex->tokens_num; i++) {

 //        simplex->tokens_regex[i] = (regex_t *)calloc(1, sizeof(regex_t));

         ret = regcomp(simplex->tokens_regex + i, simplex->tokens[i].regexstr,
                       REG_EXTENDED);
         if (ret) {
             printf("regcomp failed for %d token %d %s ret %d ....exiting\n",
                    i, simplex->tokens[i].token_num, simplex->tokens[i].regexstr, ret);
             free(simplex->scan_buf);
/*
             for (j = 0; j < simplex->tokens_num; j++) {
                 free(simplex->tokens_regex[j]);
             }
*/
             free(simplex->tokens_regex);
             free(simplex);
             return NULL;
         }
    }
    return simplex;
}

void simplex_cleanup (simplex_t *simplex)
{
    int i;

    free(simplex->scan_buf);
/*
    for (i = 0; i < simplex->tokens_num; i++) {
         free(simplex->tokens_regex[i]);
    }
*/
    free(simplex->tokens_regex);
    free(simplex);
}

int yysimplex (void *yylval, yyscan_t yyscan)
{
    int  i = 0, j, k = 0, l = 0, m = 0, odd = 0;
    int  ret, len;
    regmatch_t pmatch;
    gills_fread_t* fread;
    int buf_index;
    simplex_t *simplex = (simplex_t *)yyscan;
    char *bufptr;

    fread = simplex->fread;
    ret = gills_fread(fread);
    if (ret) {
         if (ret == GILLS_FEOF_BUFEMPTY) {
             return EOF;
//            return SIMPLEX_END_OF_SCAN;
         }
         if (ret == GILLS_FERROR) {
             return GILLS_SCANNER_ERROR;
//            return SIMPLEX_FILE_READ_ERROR;
         }
    }
    bufptr = fread->readbuf;
    buf_index = fread->head_idx;

      j = 0;
      while (1) {
        ret = regexec(simplex->tokens_regex + j, bufptr + buf_index, 1, &pmatch, 0);
        if (ret != 0) {
           j++;
           if (j == simplex->tokens_num) break;
           continue; 
        }

        len = pmatch.rm_eo - pmatch.rm_so;
        printf("%d %d\n", pmatch.rm_so, pmatch.rm_eo);
        if (pmatch.rm_so) {
            j++; if (j == simplex->tokens_num) { return SIMPLEX_NO_MATCH; } 
            continue;
        }

        for (k = 0; k < len; k++) {
             simplex->scan_buf[k] = bufptr[buf_index + k];
        }
        simplex->scan_buf[k] = '\0';
        simplex->scan_matchlen = len;
//        fread->head_idx += len;
        gills_fread_buf_consume(fread, len);
        printf("pattern %d  %s\nyylval %s\ntoken %d \n\n", j, simplex->tokens[j].regexstr, simplex->scan_buf, simplex->tokens[j].token_num);

#define GETLVAL_INLINE 1
#ifdef GETLVAL_INLINE
        yyget_lval(simplex);
#endif
        return simplex->tokens[j].token_num;
    }
    simplex->fread->head_idx = simplex->fread->end_idx = 0;
    return SIMPLEX_NO_MATCH;
}

int yylex (YYSTYPE *yylval, yyscan_t *yyscan)
{
    return yysimplex((void *)yylval, yyscan);
}

YYSTYPE * yyget_lval  (yyscan_t yyscanner)
{
    simplex_t *simplex = (simplex_t *)yyscanner;

    ((YYSTYPE *)(simplex->yylval))->str = simplex->scan_buf;
    return (YYSTYPE *)(simplex->yylval);
}

void yyset_lval (YYSTYPE * yylval_param, yyscan_t yyscanner)
{
    simplex_t *simplex = (simplex_t *)yyscanner;

    simplex->yylval = yylval_param;
}



