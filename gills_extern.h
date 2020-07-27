#ifndef __GILLS_EXTERN_H__
#define __GILLS_EXTERN_H__

gills_context_t* gills_create_def(void *yylval, void *yyscan);
void init_gills_params(gills_context_t *gills,
                       int init_pstack_max_num,
                       int init_pstackidx_max_num,
                       int init_tkstack_max_num,
                       int init_listnodearr_max_num,
                       int init_scan_funcs_max_num,
                       int init_pnode_max_num,
                       int init_action_params_max_num,
                       int init_last_tk_num);
int init_gills_alloc(gills_context_t *gills);
void init_gills_scan_func(gills_context_t *gills,
                     scan_func_t scan_func, void *scan_val,
                     int val_size);
int read_parse(gills_context_t *gills,
                token_read_t *tkread, int tokens_num,
                char **tkleft_assoc, int tkleftassoc_num,
                char **tkright_assoc, int tkrtassoc_num,
                char **tkprec, int tkprec_num);
int parse(gills_context_t *gills, token_node_t *starttoken);
void gills_cleanup(gills_context_t *gills);
void exit_parse(gills_context_t *gills, int exit_code);

#endif    /* __GILLS_EXTERN_H__ */
