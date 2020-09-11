#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include "gills_app.h"

#define UNEXPECTED_EOF          -2
#define UNEXPECTED_TOKEN        -3
#define TOKEN_OUT_OF_PARSE_TREE -4
#define AMBIGUITY               -5
#define UNKNOWN_TOKEN           -6
#define MEM_EXHAUSTED           -7
#define INVALID_ASSOC_PREC      -8
#define CREC_AMBIGUOUS          -9
#define UPPNODE_AMBIGUOUS       -10
#define GILLS_SCANNER_ERROR     -11

#define TOP_TREE_REACHED         1

#define PNODE_DEF_NUM            10000
#define LIST_NODES_DEF_NUM       10000
#define PSTACK_MAX_DEF_NUM       10000
#define TKSTACK_MAX_DEF_NUM      1000
#define PSTACK_IDX_MAX_DEF_NUM   1000

#define TOKEN_NAME_LEN           256

#define PREP_SCAN_FUNC_IDX 0
#define MAKE_SCAN_FUNC_IDX 1
#define SCAN_FUNCS_MAX_DEF_NUM 2

#define REDUCE_ACTION_INLINE 0
#define TOKEN_MEM_INTERNAL   1

#define LAST_TOKEN_DEF_NUM        EOF
#define EMPTY_TOKEN_DEF_NUM       1
#define EMPTY_TOKEN_DEF_NAME      "%empty"

struct gills_context_;
typedef void * yyscan_t;
typedef int (*scan_func_t)(void *, yyscan_t);

typedef enum assoc_ {
     NO_ASSOC = 0,
     RIGHT_ASSOC,
     LEFT_ASSOC
} assoc_e;

typedef union GILLSYYSTYPE_ {
    char str[8];
} GILLSYYSTYPE;

typedef struct list_ {
    void *data;
    struct list_ *next;
    struct list_ *prev;
} list_t;

typedef void (*rule_action_t)(struct gills_context_ *);

typedef struct token_rule_ {
    int rules_num;
    int tokens_num[10];
    int rules_assoc[10];
    int rules_prec[10];
    rule_action_t f[10];
    char rule_tokens[10][20][128];
//    int rule_tokens_assoc[10][20];
//    int rule_tokens_prec[10][20];
} token_rule_t;

typedef struct token_read_ {
    char uptoken_name[128];
    int uptoken_num;
    int token_assoc;
    int token_prec;
//    char *token_datatype;
    token_rule_t rules;
//    int rules_num;
} token_read_t;

struct token_node_;
struct rule_;

typedef struct rule_node_ {
    struct token_node_ *token;
    struct rule_ *rule;
    list_t *lnode;
    struct rule_node_ *next;
    struct rule_node_ *prev;
    struct token_node_ *up;
    int num;
    int rule_num;
    int recur;
    int crecur;
    int right_recur;
    int int_recur;
//  int mutual_recur;
    int assoc;
    int prec;
} rule_node_t;

typedef struct rule_ {
    struct token_node_ *token;
    rule_node_t *rule_nodes; //rule_nodes[]
    int num;
    int rule_nodes_num;
    int assoc;
    int prec;
    rule_action_t action;
    int crecur;
    int right_recur;
    int int_recur;
} rule_t;

typedef struct token_node_ {
    int token_num;
    int nonterm;
    struct token_node_ *up;
    char token_name[TOKEN_NAME_LEN];
    char *token_datatype;
    int recur;
    int crecur;
    int right_recur;
    int int_recur;
    int assoc;
    int prec;
//    int token_num;
    int rules_num;
    rule_t *rules;
    struct token_node_ *token_next;
    struct token_node_ *token_prev;
} token_node_t;

typedef struct parse_reclist_ {
    list_t *parserecdn_lnode;
    list_t **parse_reclist;
} parse_reclist_t;

typedef struct parse_node_ {
    token_node_t *token;
    rule_node_t *rule_node;
    rule_t *rule;
    int nonterm;
    list_t *tkstack_lnode;
    int tkstackidx;
    int recur;
    int crecur;
    int right_recur;
    int int_recur;
    int right_recur_end;
    int recurup;
    int uprecur;
//    struct parse_node_ *recur_pnode;
    list_t *recur_list;
    struct parse_node_ *rl_next;
    struct parse_node_ *rl_prev;
    list_t *dn;
    list_t *updn_lnode;
    struct parse_node_ *up;
    struct parse_node_ *uprecur_pnode;
    int rnum;
    int uprnum;
    int dnrnum;
    int res;
    int recres;
    int uprecres;
    int topres;
    int topres_top_or_lasttk;
    struct parse_node_ *recres_pnode;
    int res_rule_num;
    int seq_num;
    struct parse_node_ *dnres_pnode;
    list_t *dnres_lnode;
    struct parse_node_ *recup_pnode;
    struct parse_node_ *recdn_pnode;
    int recdn_prec;
    int recdn_assoc;
    struct parse_node_ *recurup_pnode;
    struct parse_node_ *crecurup_pnode;
    struct parse_node_t *parserec_pnode;
    list_t *upparse_rec_list;
    list_t *upreclist_lnode;
    int rec_seq_num;
    list_t *parse_reclist;
//    parse_reclist_t *parse_reclist;
    list_t *parserec_llnode;
    list_t *parserec_lnode;
    struct parse_node_ *gen_recur_pnode;
    list_t *up_recur_list;
    struct parse_node_ *up_precur_pnode;
    list_t *recur_up_list;
    int recur_up_top;
    int recur_free;
    list_t *rec_prevup_list;
    list_t *recprevup_lnode;
    int assoc;
    int prec;
    int act;
    int recact;
    void *val;
    list_t *toplist_lnode;
    list_t *top_lasttk_pnode_list;
    struct parse_node_ *lasttk_toppnode;
    struct parse_node_ *toptolast_tkpnode;
    struct parse_node_ *lasttktotoprule_pnode;
    int lasttk_top_floating;
    struct parse_node_ *recur_fl_lasttk_pnode;
} parse_node_t;

typedef struct gills_context_ {
    int topreach;
    int top_free;
    parse_node_t *toppnode;
    token_node_t top_last_exp_token;
    parse_node_t top_last_exp_pnode;
    int last_tk_num;
    int empty_tk_num;
    parse_node_t **pstack;
    int pstack_idx;
    int pstack_max_num;
//    int *pstackidx_stack;
//    int pstackidx_top;
    int pstackidx_max_num;
    list_t **tkstack;
    list_t *prevop_tkstack_lnode;
    int prevop_tkstack_update;
    int tkstack_idx;
    int tkstack_max_num;
    list_t *listnodearr;
    int listnodearr_idx;
    int listnodearr_max_num;
    int scan_funcs_max_num;
    int scan_funcs_max_idx;
    int scan_active_idx;
    scan_func_t *scan_funcs;
    void **scan_funcs_val;
    int *scan_val_size;
    list_t *token_list;
    list_t *term_token_list;
    token_node_t *top_token;
    rule_node_t top_rule_node;
    int tokens_num;
    int retfree;
    yyscan_t *yyscan;
    void **act_val;
    int action_params_max_num;
    parse_node_t *pnodemem;
    int pnodemem_idx;
    parse_node_t **pnodeptrs;
    int pnode_max_num;
    int pnodeptrs_start;
    int pnodeptrs_end;
 //   void *yyscan;
    void* yylval;
} gills_context_t;

typedef void* yystype;

#endif
