#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gills.h"
#include "gills_extern.h"

void fin (gills_context_t *gills)
{
    printf("input : %s %s\n", ((GILLSYYSTYPE *)(gills->act_val[1]))->str,
           (gills->act_val[2]) ? 
           ((GILLSYYSTYPE *)(gills->act_val[2]))->str : NULL);
}

void funword (gills_context_t *gills) {
    if (gills->act_val[1] && gills->act_val[2])
    printf("word : %s %s\n", ((GILLSYYSTYPE *)(gills->act_val[1]))->str, ((GILLSYYSTYPE *)(gills->act_val[2]))->str);
}

void f0 (gills_context_t *gills)
{
    printf("input : %s\n", ((GILLSYYSTYPE *)(gills->act_val[1]))->str);
}

void f1 (gills_context_t *gills)
{
    printf("input : %s\n", ((GILLSYYSTYPE *)(gills->act_val[1]))->str);
}

void f (gills_context_t *gills)
{
    printf("%s %s %s\n", ((GILLSYYSTYPE *)(gills->act_val[1]))->str, ((GILLSYYSTYPE *)(gills->act_val[2]))->str, ((GILLSYYSTYPE *)(gills->act_val[3]))->str);
}

/*
void f (gills_context_t *gills)
{
    printf("word %s %s\n", ((GILLSYYSTYPE *)(gills->act_val[1]))->str, ((GILLSYYSTYPE *)(gills->act_val[2]))->str);
}
*/
/*
int parse_tokens_num1 = 7;
token_read_t parse_data1[7] = {
    { "input", 0, 0, 0, { 1, { 1 }, { 0 }, { 0 }, { f0 },
       { "expr", "\0" } } },
    { "expr", 7, 0, 0, { 5, { 1, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { f0, f, f, f, f },
       { { "NUM", "\0" }, { "expr", "PLUS", "NUM", "\0" }, { "expr", "MINUS", "NUM", "\0" },{ "expr", "MULTI", "NUM", "\0" },{ "expr", "DIV", "NUM", "\0" } } } },
    { "NUM", 2, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "PLUS", 3, RIGHT_ASSOC, 2, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MINUS", 4, RIGHT_ASSOC, 1, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MULTI", 5, LEFT_ASSOC, 4, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "DIV", 6, LEFT_ASSOC, 3, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },

};
*/
/*
int parse_tokens_num2 = 7;
token_read_t parse_data2[7] = {
    { "input", 0, 0, 0, { 1, { 1 }, { 0 }, { 0 }, { f0 },
       { "expr", "\0" } } },
    { "expr", 7, 0, 0, { 5, { 1, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { f0, f, f, f, f },
       { { "NUM", "\0" }, { "NUM", "PLUS", "expr", "\0" }, { "NUM", "MINUS", "expr", "\0" },{ "NUM", "MULTI", "expr", "\0" },{ "NUM", "DIV", "expr", "\0" } } } },
    { "NUM", 2, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "PLUS", 3, RIGHT_ASSOC, 2, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MINUS", 4, RIGHT_ASSOC, 1, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MULTI", 5, LEFT_ASSOC, 4, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "DIV", 6, LEFT_ASSOC, 3, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },

};
*/

/*
int parse_tokens_num = 7;
token_read_t parse_data[7] = {
    { "input", 0, 0, 0, { 1, { 1 }, { 0 }, { 0 }, { NULL },
       { "expr", "\0" } } },
    { "expr", 7, 0, 0, { 5, { 1, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { NULL, NULL, NULL, NULL, NULL },
       { { "NUM", "\0" }, { "expr", "PLUS", "expr", "\0" }, { "expr", "MINUS", "expr", "\0" },{ "expr", "MULTI", "expr", "\0" },{ "expr", "DIV", "expr", "\0" } } } },
    { "NUM", 2, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "PLUS", 3, RIGHT_ASSOC, 2, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MINUS", 4, RIGHT_ASSOC, 1, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MULTI", 5, LEFT_ASSOC, 4, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "DIV", 6, LEFT_ASSOC, 3, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },

};
*/

int parse_tokens_num = 7;
token_read_t parse_data[7] = {
    { "input", 0, 0, 0, { 1, { 1 }, { 0 }, { 0 }, { f0 },
       { "expr", "\0" } } },
    { "expr", 7, 0, 0, { 5, { 1, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { f0, f, f, f, f },
       { { "NUM", "\0" }, { "expr", "PLUS", "expr", "\0" }, { "expr", "MINUS", "expr", "\0" },{ "expr", "MULTI", "expr", "\0" },{ "expr", "DIV", "expr", "\0" } } } },
    { "NUM", 2, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "PLUS", 3, RIGHT_ASSOC, 2, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MINUS", 4, RIGHT_ASSOC, 1, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MULTI", 5, LEFT_ASSOC, 4, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "DIV", 6, LEFT_ASSOC, 3, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },

};

/*
int parse_tokens_num4 = 9;
token_read_t parse_data4[9] = {
    { "input", 0, 0, 0, { 1, { 1 }, { 0 }, { 0 }, { f0 },
       { "expr", "\0" } } },
    { "expr", 7, 0, 0, { 6, { 1, 1, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0 }, { f0, f1, f, f, f, f },
       { { "NUM", "\0" }, { "WORD0", "\0" }, { "expr", "PLUS", "expr", "\0" }, { "expr", "MINUS", "expr", "\0" },{ "expr", "MULTI", "expr", "\0" },{ "expr", "DIV", "expr", "\0" } } } },
    { "NUM", 2, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "WORD0", 9, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "PLUS", 3, RIGHT_ASSOC, 2, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MINUS", 4, RIGHT_ASSOC, 1, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MULTI", 5, LEFT_ASSOC, 4, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "DIV", 6, LEFT_ASSOC, 3, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },

};
*/
/*
int parse_tokens_num = 11;
token_read_t parse_data[11] = {
    { "input", 0, 0, 0, { 1, { 2 }, { 0 }, { 0 }, { fin },
       { "expr", "word", "\0" } } },
    { "expr", 7, 0, 0, { 5, { 1, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { f0, f, f, f, f },
       { { "NUM", "\0" }, { "expr", "PLUS", "expr", "\0" }, { "expr", "MINUS", "expr", "\0" },{ "expr", "MULTI", "expr", "\0" },{ "expr", "DIV", "expr", "\0" } } } },
    { "NUM", 2, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "PLUS", 3, RIGHT_ASSOC, 2, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MINUS", 4, RIGHT_ASSOC, 1, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MULTI", 5, LEFT_ASSOC, 4, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "DIV", 6, LEFT_ASSOC, 3, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "word", 8, 0, 0, { 1, { 2 }, { 0 }, { 0 }, { funword }, { { "WORD0", "WORD1", "\0" } } } },
    { "WORD0", 9, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "WORD1", 10, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
};
*/
/*
int parse_tokens_num = 12;
token_read_t parse_data[12] = {
    { "input", 0, 0, 0, { 1, { 2 }, { 0 }, { 0 }, { fin },
       { "expr", "word", "\0" } } },
    { "expr", 7, 0, 0, { 5, { 1, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { f0, f, f, f, f },
       { { "NUM", "\0" }, { "expr", "PLUS", "expr", "\0" }, { "expr", "MINUS", "expr", "\0" },{ "expr", "MULTI", "expr", "\0" },{ "expr", "DIV", "expr", "\0" } } } },
    { "NUM", 2, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "PLUS", 3, RIGHT_ASSOC, 2, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MINUS", 4, RIGHT_ASSOC, 1, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "MULTI", 5, LEFT_ASSOC, 4, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "DIV", 6, LEFT_ASSOC, 3, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "word", 8, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { funword, funword }, { { EMPTY_TOKEN_DEF_NAME, "\0" }, { "WORD0", "WORD1", "\0" } } } },
    { EMPTY_TOKEN_DEF_NAME, 11, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "WORD0", 9, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
    { "WORD1", 10, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } },
};
*/
/*
int parse_tokens_num6 = 4;
token_read_t parse_data6[4] = {
    { "input", 0, 0, 0, { 1, { 1 }, { 0 }, { 0 }, { f0 },
       { "word", "\0" } } },
    { "word", 1, 0, 0, { 2, { 1, 3 }, { 0, 0 }, { 0, 0 }, { f0, f },
       { { "WORD", "\0" }, { "word", "NUM", "word", "\0" } } } },
    { "WORD", 2, 0, 0, { 0,  { 0 }, { 0 }, {0}, { NULL }, {"\0" } } },
    { "NUM", 3, 0, 0, { 0, { 0 }, { 0 }, { 0 }, { NULL }, { "\0" } } }
};
*/
/*
int parse_tokens_num7 = 3;
token_read_t parse_data7[3] = {
    { "input", 0, 0, 0, { 1, { 1 }, { 0 }, { 0 }, { f0 },
       { "word", "\0" } } },
    { "word", 1, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { f0, f },
       { { "WORD", "\0" }, { "WORD", "word", "\0" } } } },
    { "WORD", 2, 0, 0, { 0,  { 0 }, { 0 }, {0}, { NULL }, {"\0" } } },
};
*/
