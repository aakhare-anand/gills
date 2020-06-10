#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "parser.h"
#include "scanner.h"
#include "gillsfileop.h"

simplex_t* simplex_init (simplex_token_t *tokens, int tokens_num,
                         gills_fread_t *fread, void *yylval)
{
    simplex_t *simplex;
    int i, ret;

    simplex = (simplex_t *)calloc(1, sizeof(simplex));
    if (!simplex) {
        printf("mem alloc failed for simplex ...exiting\n");
        return NULL;
    }

    simplex->tokens = tokens;
    simplex->tokens_num = tokens_num;
    simplex->fread = fread;
    simplex->yylval = yylval;

    simplex->scan_buf = (char *)calloc(fread.min_fread_len, 1);
    if (!simplex->scan_buf) {
        printf("mem alloc failed for simplex scan buffer ...exiting\n");
        return -1;
    }

    simplex->tokens_regex = (regex_t *)calloc(simplex->tokens_num, sizeof(regex_t));
    if (!simplex->tokens_regex) {
        printf("simplex regex mem alloc failed ...exiting\n");
        return -1;
    }

    for (i = 0; i < simplex->tokens_num; i++) {
         ret = regcomp(&simplex->tokens_regex[i], simplex->tokens[i].regexstr,
                       REG_EXTENDED);
         if (ret) {
             printf("regcomp failed for %d token %d %s ret %d ....exiting\n",
                    i, gillsgentokens[i].token_num, gillsgentokens[i].regexstr, ret);
             exit(-1);
         }
    }
    return simplex;
}

void simplex_cleanup (simplex_t *simplex)
{
    free(simplex->scan_buf);
    free(simplex->tokens_reg);
    free(simplex);
}

int yylex (void *yylval, yyscan_t *yyscan)
{
    int  i = 0, j, k = 0, l = 0, m = 0, odd = 0;
    int  ret, len;
    regmatch_t pmatch;
    gills_fread_t* fread;
    int buf_index = *buf_idx, offset = 0;
    simplex_t *simplex = (simplex_t *)yyscan;

    fread = simplex->fread;
    ret = gills_fread(fread);
    if (ret && (ret != GILLS_MIN_FREAD_BUF_AVAIL)) {
        if (fread->infile_eof)
            return EOF;
        printf("error in file operation ...exiting\n");
        exit(-1);
    }
    bufptr = fread->readbuf;
    buf_index = fread->head_idx;

      j = 0;
      while (1) {
        ret = regexec(&simplex->tokens_regex[j], buffer + buf_index, 1, &pmatch, 0);
        if (ret != 0) {
           j++;
           if (j == simplex->tokens_num) break;
           continue; 
        }

        offset = buf_index;
        len = pmatch.rm_eo - pmatch.rm_so;
        printf("%d %d\n", pmatch.rm_so, pmatch.rm_eo);
        if (pmatch.rm_so) {
            j++; if (j == simplex->tokens_num) { return SIMPLEX_NO_MATCH; } 
            continue;
        }

        for (k = 0; k < len; k++) {
             simplex->scan_buf[k] = buffer[offset + k];
        }
        simplex->scan_buf[k] = '\0';
//        per token driver function
//        yylval->str = simplex->scan_buf;
        simplex->scan_matchlen = len;
        fread->head_idx += len;
        printf("pattern %d  %s\nyylval %s\ntoken %d \n\n", j, gillsgentokens[j].regexstr, simplex->scan_buf, gillsgentokens[j].token_num);
        return simplex->tokens[j].token_num;
    }
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
