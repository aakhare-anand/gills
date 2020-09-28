#include <stdio.h>
#include "gills.h"
#include "gills_extern.h"
#include "simplex.h"
#include "testtokens.h"
#include "testdata.h"
#include <time.h>

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
    struct timespec timespecstart, timespecend;
    int secdiff, nsecdiff;

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
    ret = clock_gettime(CLOCK_REALTIME, &timespecstart);
    if (ret) {
        perror("clock_gettime failed: ");
        exit(-1);
    }
    ret = parse(gills, gills->top_token);
 //   printf("pnodemem_idx %d\npnodeptrs_end %d \n", gills->pnodemem_idx, gills->pnodeptrs_end);
    printf("pnodemem_free %d\npnodemem_in_use %d \n", gills->pnodemem->nodes_free_num, gills->pnodemem->nodes_in_use_num);
    if (ret) {
        printf("parse failed ...exiting\n");
        exit_parse(gills, -1);
    }
    ret = clock_gettime(CLOCK_REALTIME, &timespecend);
    if (ret) {
        perror("clock_gettime end failed: ");
        exit(-1);
    }
    printf("\nstart timespec : %d %d \n", timespecstart.tv_sec, timespecstart.tv_nsec);
    printf("end timespec : %d %d \n", timespecend.tv_sec, timespecend.tv_nsec);
    if (timespecend.tv_nsec >= timespecstart.tv_nsec) {
        nsecdiff = timespecend.tv_nsec - timespecstart.tv_nsec;
    } else {
        nsecdiff = 1000000000 + timespecend.tv_nsec - timespecstart.tv_nsec;
        timespecend.tv_sec -= 1;
    }
    secdiff = timespecend.tv_sec - timespecstart.tv_sec;
    printf("diff : %d sec %d nsec\n", secdiff, nsecdiff /* timespecend.tv_sec - timespecstart.tv_sec,
            timespecend.tv_nsec - timespecstart.tv_nsec */ );
    simplex_cleanup(simplex);
    gills_frclose(gf);
    gills_cleanup(gills);
}

