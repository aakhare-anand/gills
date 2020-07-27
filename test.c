#include <stdio.h>
#include "gills.h"
#include "gills_extern.h"
#include "simplex.h"
#include "testtokens.h"
#include "testdata.h"

// char scan_file_buf[64000];
extern char *prep_buffer;
void *yyscan;
YYSTYPE yylval;

int main ()
{
    int ret;
    gills_context_t *gills;
    simplex_t *simplex;
    gills_fread_t *gf;
    char *datafile = "data";

//    gf = gills_fileropen(datafile, GILLS_FREAD_BUF_DEF_SIZE, GILLS_FREAD_MIN_DEF_SIZE);
    gf = gills_fileropen(datafile, 64, 8);
    if (!gf) {
        printf("file open operation failed ...exiting\n");
        exit(-1);
    }
    simplex = simplex_init(testtokens, SAMPLE_TOKENS_NUM, gf, &(gills->scan_funcs_val[gills->scan_active_idx]));
//    set_prep_buf1();
    if (!simplex) {
        printf("scanner simplex init failed ...exiting\n");
        exit(-1);
    }
    gills = gills_create_def(&yylval, (void *)simplex);
    if (!gills) {
        printf("gills create def failed ...exiting\n");
        exit(-1);
    }
    ret = init_gills_alloc(gills);
    if (ret) {
        printf("init_gills_alloc failed ...exiting\n");
        exit(-1);
    }
    init_gills_scan_func(gills, yysimplex, &yylval, sizeof(YYSTYPE));
    yyset_lval(&yylval, simplex);
//    scan_tokenize("token_file");
//    scan_input_file("data", prep_buffer);
 //   scan_idx = PREP_SCAN_FUNC_IDX;

#if 0
    gills = gills_create_def();
 //   init_gills_parse_param(gills);
    init_gills_alloc(gills);
    init_composer_gills(gills);
#endif

    ret = read_parse(gills, parse_data, parse_tokens_num, NULL, 0, NULL, 0, NULL, 0);
    if (ret) {
        printf("parse data read failure ...exiting\n");
        exit_parse(gills, -1);
    }
 //   gills = init_gills_parse();
    ret = parse(gills, gills->top_token);
    printf("pnodemem_idx %d\npnodeptrs_end %d \n", gills->pnodemem_idx, gills->pnodeptrs_end);
    if (ret) {
        printf("parse failed ...exiting\n");
        exit_parse(gills, -1);
    }
    simplex_cleanup(simplex);
    gills_frclose(gf);
    gills_cleanup(gills);
}

