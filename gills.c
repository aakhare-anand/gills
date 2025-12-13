#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gills.h"
#include "gills_internal.h"
#include "gills_extern.h"
#include "gills_app.h"
#include <time.h>

extern token_read_t token_reads[];

int assign_assoc_prec(gills_context_t *gills,
                       char **right, int right_num,
                       char **left, int left_num,
                       char **prec, int prec_num);
void parse_set_rule_assoc_prec_last_term(gills_context_t *gills,
                                          parse_node_t *pnode);
void parse_set_rule_assoc_prec(gills_context_t *gills,
                                parse_node_t *pnode);
parse_node_t* get_pnode(gills_context_t *gills);
void free_pnode(gills_context_t *gills, parse_node_t *pnode);
void reset_pnode_mem(gills_context_t *gills);
int generate_parse(gills_context_t *gills,
                    parse_node_t *start_pnode,
                    token_node_t *starttoken,
                    rule_node_t *rule_node,
                    int floating,
                    parse_node_t *gen_recur_pnode,
                    parse_node_t **retpnode,
                    parse_node_t **lastres_pnode);
int get_token(gills_context_t *gills, int scan_idx);
void action_ex(gills_context_t *gills,
                parse_node_t *parsenode, parse_node_t *rlpnode);
void change_parsereclist_recup_pnode(list_t *parse_reclist,
                                      parse_node_t *recup_pnode,
                                      parse_node_t *up_pnode);
void action_ex_inline(gills_context_t *gills,
                       parse_node_t *tkpnode,
                       parse_node_t *crecur_begin_pnode);
void free_parsereclist_lnode(gills_context_t *gills, parse_node_t *pnode);
void free_parse_rule(gills_context_t *gills, parse_node_t *tkpnode);
void free_recur_list(gills_context_t *gills, parse_node_t *pnode);
void free_parse_recpnode(gills_context_t *gills,
                          parse_node_t *prevpnode,
                          int *recpnode_npass,
                          int *recpnode_pass, int updir);
void free_parse_nodes(gills_context_t *gills,
                       parse_node_t *tkpnode);
int reduce_parse(gills_context_t *gills, parse_node_t *tkpnode);

int assign_assoc_prec (gills_context_t *gills,
                       char **right, int right_num,
                       char **left, int left_num,
                       char **prec, int prec_num)
{
    int i, precedence;
    list_t *list;
    token_node_t *tknode;

    precedence = 0;
    if (!gills->term_token_list && (right_num || left_num || prec_num)) {
        printf("invalid input for no terminal tokens\n");
        return INVALID_ASSOC_PREC;
    }
    if (right) {
        for (i = 0; i < right_num; i++) {
             list = gills->term_token_list;
             while (1) {
                 tknode = (token_node_t *)list->data;
                 if (!strcmp(tknode->token_name, right[i])) {
                     precedence += 1;
                     tknode->assoc = RIGHT_ASSOC;
                     tknode->prec = precedence;
                     break;
                 }
                 list = list->next;
                 if (list == gills->term_token_list) {
                     printf("unknown token %s in right assoc list\n",
                             right[i]);
                     return UNKNOWN_TOKEN;
                 }
             }
        }
    }

    precedence = 0; 
    if (left) {
        for (i = 0; i < left_num; i++) {
            list = gills->term_token_list;
            while (1) {
                tknode = (token_node_t *)list->data;
                if (!strcmp(tknode->token_name, left[i])) {
                    precedence += 1;
                    tknode->assoc = LEFT_ASSOC;
                    tknode->prec = precedence;
                    break;
                }
                list = list->next;
                if (list == gills->term_token_list) {
                    printf("unknown token %s in left assoc list\n",
                           left[i]);
                    return UNKNOWN_TOKEN;
                }
            }
        }
    }

    precedence = 0;
    if (prec) {
        for (i = 0; i < prec_num; i++) {
            list = gills->term_token_list;
            while (1) {
                tknode = (token_node_t *)list->data;
                if (!strcmp(tknode->token_name, prec[i])) {
                    precedence += 1;
                    tknode->prec = precedence;
                    break;
                }
                list = list->next;
                if (list == gills->term_token_list) {
                    printf("unknown token %s in precedence list\n",
                            prec[i]);
                    return UNKNOWN_TOKEN;
                }
            }
        }
    }
}

void parse_set_rule_assoc_prec_last_term (gills_context_t *gills,
                                          parse_node_t *pnode)
{
    parse_node_t *fpnode = pnode;
    parse_node_t *recpnode = NULL;

    while (1) {
        if (fpnode->rule_start)
            return;
        if (fpnode->rule_node->right_recur ||
            fpnode->rule_node->int_recur) {
            if (recpnode) {
                printf("invalid recursvie syntax at rule ");
//               print_rule(fpnode->rule);
                printf("   ...exiting\n");
                exit(-1);
            }
           recpnode = fpnode;
        } else if (!fpnode->nonterm) {
            if (recpnode) {
                recpnode->assoc = fpnode->assoc;
                recpnode->prec = fpnode->prec;
            }
            recpnode = NULL;
        }
       fpnode = fpnode->rl_prev;
    }
}

void parse_set_rule_assoc_prec (gills_context_t *gills,
                                parse_node_t *pnode)
{
    parse_node_t *fpnode = pnode;
    parse_node_t *recpnode;
    int dnres_pass = 0;

    if (fpnode->rl_next /* ||
        !fpnode->rl_next->right_recur */)
        return;
    while (1) {
        if (!dnres_pass || fpnode->right_recur) {
            fpnode = fpnode->rl_prev;
            dnres_pass = 0;
        }
        if (!fpnode)
            return;
        if (fpnode->int_recur || fpnode->right_recur) {
            recpnode = fpnode;
            continue;
        }
        if (fpnode->recur) {
            continue;
        }
        if (!fpnode->nonterm) {
            pnode->assoc = fpnode->assoc;
            pnode->prec = fpnode->prec;
        }
        fpnode = fpnode->dnres_pnode;
        while (fpnode->rl_next)
               fpnode = fpnode->rl_next;
        dnres_pass = 1;
    }
}        

int read_parse (gills_context_t *gills,
                token_read_t *tkread, int tokens_num,
                char **tkleft_assoc, int tkleftassoc_num,
                char **tkright_assoc, int tkrtassoc_num,
                char **tkprec, int tkprec_num)
{
    list_t *list, *lnode;
    int i, j, k, m, found, token_num_max = 0, rlnum, rlnodenum = 0;
    token_node_t *token, *uptknode, *tknode;
    char *token_name;
    rule_t *rules, *rule;
    rule_node_t *rlnode, *rule_node;

    for (i = 0; i < tokens_num; i++) {
        tknode = (token_node_t *)calloc(1, sizeof(token_node_t));
        strcpy(tknode->token_name, tkread[i].uptoken_name);
        if (!strcmp(tkread[i].uptoken_name, EMPTY_TOKEN_DEF_NAME)) {
            gills->empty_tk_num = tkread[i].uptoken_num;
        }
        tknode->token_num = tkread[i].uptoken_num;
        if (token_num_max < tknode->token_num)
            token_num_max = tknode->token_num;
        tknode->rules_num = tkread[i].rules.rules_num;
        tknode->assoc = tkread[i].token_assoc;
        tknode->prec = tkread[i].token_prec;
        gills->token_list = list_add(gills, gills->token_list, tknode, NULL);
    }
    for (i = 0; i < tokens_num; i++) {
        list = gills->token_list;
        found = 0;
        while (1) {
            if (!list)
                break;
            list = list->next;
            if (list == gills->token_list)
                break;
            tknode = (token_node_t *)list->data;
            if (!strcmp(tknode->token_name, tkread[i].uptoken_name)) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("token %s not found ...exiting\n",
                    tknode->token_name);
            return UNKNOWN_TOKEN;
        } else {
            uptknode = tknode;
        }
        uptknode->rules = (rule_t *)calloc(tkread[i].rules.rules_num,
                           sizeof(rule_t));
        uptknode->rules_num = tkread[i].rules.rules_num;
        if (uptknode->rules_num)
            uptknode->nonterm = 1;
        for (m = 0; m < tkread[i].rules.rules_num; m++) {
            uptknode->rules[m].token = uptknode;
            uptknode->rules[m].assoc = tkread[i].rules.rules_assoc[m];
            uptknode->rules[m].prec = tkread[i].rules.rules_prec[m];
            uptknode->rules[m].num = m;
            uptknode->rules[m].rule_nodes_num = tkread[i].rules.tokens_num[m];
            uptknode->rules[m].action = tkread[i].rules.f[m];
            rlnum = 0;
            rlnodenum = 0;
            uptknode->rules[m].rule_nodes = (rule_node_t *)
                calloc(tkread[i].rules.tokens_num[m],
                       sizeof(rule_node_t));
            for (k = 0; k < tkread[i].rules.tokens_num[m]; k++) {
                 rlnode = &(uptknode->rules[m].
                          rule_nodes[k]);
                 token_name = tkread[i].rules.rule_tokens[m][k];
                 list = gills->token_list;
                 while (1) {
                     if (!list)
                         break;
                     list = list->next;
                     if (list == gills->token_list)
                         break;
                     tknode = (token_node_t *)list->data;
                     if (!strcmp(token_name, tknode->token_name))
                         break;
                 }
                 if (list == gills->token_list) {
                     tknode = (token_node_t *)calloc(1, sizeof(token_node_t));
                     strcpy(tknode->token_name, token_name);
                     token_num_max += 1;
                     tknode->rules_num = 0;
                     gills->token_list = list_add(gills, gills->token_list, tknode, NULL);
                     gills->term_token_list = list_add(gills, gills->term_token_list, tknode, NULL);
                 }
/*
                      printf("unknown token %s ...exiting\n", token_name);
                      return UNKNOWN_TOKEN;
*/
                  rlnode->token = tknode;
                  rlnode->num = rlnodenum;
                  rlnode->rule_num = m;
                  rlnode->rule = &(uptknode->rules[m]);
                  rlnode->up = uptknode;
                  if (rlnode->token == uptknode) {
                      uptknode->recur = 1;
                      if (rlnode->num == 0) {
                          rlnode->crecur = 1;
                          uptknode->crecur = 1;
                          uptknode->rules[m].crecur = 1;
                      } else if (rlnode->num == 
                          (uptknode->rules[m].rule_nodes_num - 1)) {
                          rlnode->right_recur = 1;
                          uptknode->right_recur = 1;
                          uptknode->rules[m].right_recur = 1;
                      } else {
                          rlnode->int_recur = 1;
                          uptknode->int_recur = 1;
                          rules[m].int_recur = 1;
                      }
                  }
/*
                  rlnode->assoc = tkread[i].rules.rule_token_assoc[m][rlnodenum];
                  rlnode->prec = tkread[i].rules.rule_token_prec[m][rlnodenum];
*/
                  
                  rlnodenum += 1;
                  rlnode->up = uptknode;
            }
        }
    }
        list = gills->token_list;
    while (1) {
        list = list->next;
        if (list == gills->token_list)
            break;
        token = (token_node_t *)(list->data);
        for (i = 0; i < token->rules_num; i++) {
            rule = &(token->rules[i]);
            for (j = 0; j < rule->rule_nodes_num; j++) {
                rule_node = &(rule->rule_nodes[j]);
                if ((rule_node->token->token_num !=
                     token->token_num) &&
                     (rule_node->token->crecur ||
                      rule_node->token->int_recur ||
                      rule_node->token->right_recur)) {
                    rule_node->recur = 1;
                }
            }
        }
    }
/*
      assign_assoc_prec(gills, tkleft_assoc, tkleftassoc_num,
                        tkright_assoc, tkrtassoc_num,
                        tkprec, tkprec_num);
*/
//      assign_rule_assoc_prec();
    list = gills->token_list;
    gills->top_token = NULL;
    while (list) {
        list = list->next;
        if (list == gills->token_list) {
            printf("no top token input defined ...exiting\n");
            exit_parse(gills, -1);
        }
        tknode = (token_node_t *)list->data;
        if (!strcmp(tknode->token_name, "input")) {
            tknode->token_num = 0;
            gills->top_token = tknode;
            break;
        }
    }

    gills->tokens_num = tokens_num;
    return 0;
}

parse_node_t* get_pnode (gills_context_t *gills)
{
    return mem_pool_get_node(gills, gills->pnodemem);
}


void free_pnode (gills_context_t *gills, parse_node_t *pnode)
{
#ifndef GILLS_FAST
    list_t *upparsereclist, *uppreclist_lnode, *recuplist;
    parse_node_t *recuruppnode, *uprecpnode;

    if (pnode->crecur) {
        if (pnode->parse_reclist) {
            printf("parse node free with rec branches ...returning/*exiting*/\n");
//            exit_parse(gills, -1);
        }
        recuplist = pnode->recur_up_list;
        if (pnode->recup_pnode && 
            pnode->parserec_lnode) {
//            pnode->recup_pnode->parse_reclist) {
            free_parsereclist_lnode(gills, pnode);
        }
        pnode->recur_up_list = NULL;
    }

    if (!pnode->int_recur &&
        !pnode->right_recur &&
        !pnode->crecur &&
         pnode->recur &&
         pnode->recur_list)
         free_recur_list(gills, pnode);
    if (pnode->tkstack_lnode) {
        if (gills->prevop_tkstack_lnode) {
            if (gills->prevop_tkstack_lnode == pnode->tkstack_lnode) {
                gills->prevop_tkstack_lnode = pnode->tkstack_lnode->next;
                    if (gills->prevop_tkstack_lnode->next->next ==
                        gills->prevop_tkstack_lnode) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
                        gills->tkstack[pnode->tkstackidx] = NULL;
#else
                        gills->prevop_tkstack_listmesh_list = NULL;
#endif
                }
                gills->prevop_tkstack_update = 1;
            }
            if (gills->prevact_tkstack_list) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
                if (gills->prevact_tkstack_list == 
                    gills->tkstack[pnode->tkstackidx]) {
                    if (pnode->tkstack_lnode ==
                        pnode->tkstack_lnode->next->next) {
                        while (1) {
                            gills->prevact_tkstack_idx += 1;
                            if ((gills->action &&
                                 gills->tkstack_idx < 
                                 gills->prevact_tkstack_idx) ||
                                 (!gills->action &&
                                   gills->tkstack_idx ==
                                   gills->prevact_tkstack_idx)) {
                                   gills->prevact_tkstack_list = NULL;
                                   gills->prevact_tkstack_idx = -1;
                                   break;
                            }
                            if (gills->tkstack[gills->prevact_tkstack_idx]) {
                                gills->prevact_tkstack_list =
                                gills->tkstack[gills->prevact_tkstack_idx];
                                break;
                            }
                        }
                    }
                }
#else
                if (gills->prevact_tkstack_list == pnode->tkstack_llnode) {
                    if (pnode->tkstack_llnode ==
                        pnode->tkstack_llnode->next->next) {
                        if (!gills->prevact_tkstack_llist ||
                             (gills->prevact_tkstack_llist ==
                                gills->prevact_tkstack_llist->next->next)) {
                                gills->prevact_tkstack_list = NULL;
                                gills->prevact_tkstack_llist = NULL;
                        } else {
                            gills->prevact_tkstack_llist = gills->prevact_tkstack_llist->next;
                            gills->prevact_tkstack_list = (list_t *)(gills->prevact_tkstack_llist->data);
                        }
                    }
                }
#endif
            }
        }
#ifndef GILLS_MEM_TKSTACK_UNLIM
        gills->tkstack[pnode->tkstackidx] = list_del_node(gills, gills->tkstack[pnode->tkstackidx], pnode->tkstack_lnode);
#else
        list_mesh_del_node(gills, &gills->tkstack_listmesh, pnode->tkstack_llmeshnode, &pnode->tkstack_llnode,
                           pnode->tkstack_lnode);
#endif
#ifdef TOKEN_MEM_INTERNAL
//        free(pnode->val);
#endif
    }
    mem_pool_node_free(gills, gills->pnodemem, (void *)pnode);
#endif
}

void reset_pnode_mem (gills_context_t *gills)
{
 //   gills->pnodemem_idx = 0;
 //   gills->pnodeptrs_start = gills->pnodeptrs_end = 0;
}

list_t* list_create (gills_context_t *gills)
{
    list_t *listhead;

    if (gills->listnodearr_idx > gills->listnodearr_max_num) {
        printf("link list memory %d exhausted ...exiting\n",
                gills->listnodearr_max_num);
        exit(-1);
    }
    listhead = &(gills->listnodearr[gills->listnodearr_idx]);
    gills->listnodearr_idx += 1;
    listhead->next = listhead->prev = listhead;
    return listhead;
}

list_t* list_add (gills_context_t *gills,
                  list_t *list, void *data, list_t **lnode)
{
    list_t *nlist; //  = calloc(1, sizeof(list_t));
    list_t *prev, *dlist, *listhead = list;

    if (gills->listnodearr_idx > gills->listnodearr_max_num) {
        printf("link list memory %d exhausted ...exiting\n",
                gills->listnodearr_max_num);
        exit(-1);
    }
    if (!listhead) {
        listhead = &(gills->listnodearr[gills->listnodearr_idx]);
        gills->listnodearr_idx += 1;
        listhead->next = listhead->prev = listhead;
    }
    nlist = &(gills->listnodearr[gills->listnodearr_idx]);
    gills->listnodearr_idx += 1;
    if (lnode)
        *lnode = nlist;
    prev = listhead->prev;
    listhead->prev = nlist;
    nlist->prev = prev;
    nlist->next = listhead;
    prev->next = nlist;
    nlist->data = data;
    return listhead;
}

list_t* list_del_node (gills_context_t *gills,
                       list_t *list, list_t *node)
{
    list_t *retnode;

    if (!list || !node)
        return list;
        node->prev->next = node->next;
        node->next->prev = node->prev;
//        free(node);
#ifndef GILLS_FAST
        memset(node, 0, sizeof(list_t));
#endif
        if (list == list->next) {
//            free(list);
#ifndef GILLS_FAST
            memset(list, 0, sizeof(list_t));
#endif
            return NULL;
        }
        return list;
}
 

list_t* list_del_head (gills_context_t *gills, list_t *list)
{
    list_t *listnode;
    if (!list)
        return NULL;
    if (list == list->next) {
 //       free(list);
        return NULL;
    }
    listnode = list->next;
    listnode->prev = list->prev;
    list->prev->next = listnode;
//    free(list);
    return listnode;
}

void free_list (list_t *list)
{
    list_t *nxtlist = NULL, *listptr = list;

    if (!list)
        return;
    while (1) {
        if (listptr->next == listptr) {
            memset(listptr, 0, sizeof(list_t));
            return;
        }
        nxtlist = listptr->next;
//        free(listptr);
        listptr->next->prev = listptr->prev;
        listptr->prev->next = listptr->next;
        memset(listptr, 0, sizeof(list_t));
        listptr = nxtlist;
    }
}

void list_mesh_add (gills_context_t *gills,
                    list_t **llist, list_t **list,
                    void *data, list_t **llnode, list_t **lnode)
{
    list_t *listptr = *list, *llistptr = *llist;

    listptr = list_add(gills, listptr, data, lnode);
    if (!*list) {
        llistptr = list_add(gills, llistptr, listptr, llnode);
        *llist = llistptr;
    }
    *list = listptr;
}

void list_mesh_del_node (gills_context_t *gills,
                         list_t **llist, list_t *llist_node, list_t **list,
                         list_t *list_node)
{
    list_t *listptr = *list, *llistptr = *llist;

    if ((listptr->next == list_node) &&
        (listptr->prev == list_node)) {
        llistptr = list_del_node(gills, llistptr, llist_node);
        if (!llistptr)
            *llist = NULL;
    }
    listptr = list_del_node(gills, listptr, list_node);
    if (!listptr)
        *list = NULL;
}

gills_context_t* gills_create_def (void *yylval, void *yyscan)
{
    gills_context_t *gills;

    gills = (gills_context_t *)calloc(1, sizeof(gills_context_t));
    gills->yylval = yylval;
    gills->yyscan = yyscan;

    gills->last_tk_num = LAST_TOKEN_DEF_NUM;
    gills->empty_tk_num = EMPTY_TOKEN_DEF_NUM;
    gills->pstack_max_num = PSTACK_MAX_DEF_NUM;
//    gills->pstackidx_max_num = PSTACK_IDX_MAX_DEF_NUM;
    gills->tkstack_max_num = TKSTACK_MAX_DEF_NUM;
    gills->listnodearr_max_num = LIST_NODES_DEF_NUM;
    gills->scan_funcs_max_num = SCAN_FUNCS_MAX_DEF_NUM;
    gills->pnodemem_nodes_max_num = PNODEMEM_NODES_MAX_DEF_NUM;
    gills->pnodemem_segs_max_num = PNODEMEM_SEGS_MAX_DEF_NUM;
}

void init_gills_params (gills_context_t *gills,
                       int init_pstack_max_num,
//                       int init_pstackidx_max_num,
                       int init_tkstack_max_num,
                       int init_listnodearr_max_num,
                       int init_scan_funcs_max_num,
                       int init_pnodemem_nodes_max_num,
                       int init_pnodemem_segs_max_num,
                       int init_action_params_max_num,
                       int init_last_tk_num)
{
    gills->pstack_max_num = init_pstack_max_num;
//    gills->pstackidx_max_num = init_pstackidx_max_num;
    gills->tkstack_max_num = init_tkstack_max_num;
    gills->listnodearr_max_num = init_listnodearr_max_num;
    gills->scan_funcs_max_num = init_scan_funcs_max_num;
    gills->pnodemem_nodes_max_num = init_pnodemem_nodes_max_num;
    gills->pnodemem_segs_max_num = init_pnodemem_segs_max_num;
    gills->action_params_max_num = init_action_params_max_num;
    gills->last_tk_num = init_last_tk_num;
}

int init_gills_alloc (gills_context_t *gills)
{

    gills->retfree = 0;
    gills->topreach = 0;
    gills->top_free = 0;
    gills->toppnode = NULL;
    gills->pstack_idx = -1;
    gills->tkstack_idx = 0;
//    gills->pstackidx_top = -1;
    gills->listnodearr_idx = 0;
    gills->scan_funcs_max_idx = -1;
    gills->scan_active_idx = 0;
 //   gills->pnodemem_idx = 0;
 //   gills->pnodeptrs_start = 0;
 //   gills->pnodeptrs_end = 0;
    gills->listnodearr = (list_t *)calloc(gills->listnodearr_max_num, sizeof(list_t));
    gills->pstack = (parse_node_t **)calloc(gills->pstack_max_num, sizeof(parse_node_t *));
//    gills->pnodemem = (parse_node_t *)calloc(gills->pnode_max_num, sizeof(parse_node_t));
//    gills->listnodearr = (list_t *)calloc(gills->listnodearr_max_num, sizeof(list_t));
 //   gills->pnodeptrs = (parse_node_t **)calloc(gills->pnode_max_num, sizeof(parse_node_t *));
//    gills->pstack = (parse_node_t **)calloc(gills->pstack_max_num, sizeof(parse_node_t *));
//    gills->pstackidx_stack = (int *)calloc(gills->pnode_max_num, sizeof(int));
#ifndef GILLS_MEM_TKSTACK_UNLIM
    gills->tkstack = (list_t **)calloc(gills->tkstack_max_num, sizeof(list_t));
#endif
 //   memset(&(gills->top_rule_node), 0, sizeof(rule_node_t));
 //   memset(&(gills->top_last_exp_pnode), 0, sizeof(parse_node_t));
 //   memset(&(gills->top_last_exp_token), 0, sizeof(token_node_t));
    gills->top_last_exp_pnode.token = &gills->top_last_exp_token;
    gills->top_last_exp_token.token_num = gills->last_tk_num;
 //   gills->scan_funcs_max_num = SCAN_FUNCS_MAX_DEF_NUM;
 //   gills->scan_funcs_max_idx = -1;
    gills->act_val = (void **)calloc(gills->action_params_max_num,
                                     sizeof(void *));
    gills->scan_funcs = (scan_func_t *)calloc(gills->scan_funcs_max_num,
                                       sizeof(scan_func_t));
    gills->scan_funcs_val = (void **)calloc(gills->scan_funcs_max_num,
                                       sizeof(void *));
    gills->scan_val_size = (int *)calloc(gills->scan_funcs_max_num,
                                       sizeof(int));
 //   gills->pnodemem = (parse_node_t *)calloc(gills->pnode_max_num, sizeof(parse_node_t));

    gills->pnodemem = mem_pool_alloc(gills,
                                     sizeof(parse_node_t),
                                     gills->pnodemem_nodes_max_num,
                                     gills->pnodemem_segs_max_num);
   if (!gills->pnodemem) {
       printf("mem_pool_alloc failed\n");
       return -1;
   }

    return 0;
}

void init_gills_scan_func (gills_context_t *gills,
                     scan_func_t scan_func, void *scan_val,
                     int val_size)
{
    gills->scan_funcs_max_idx += 1;
    gills->scan_funcs[gills->scan_funcs_max_idx] = *scan_func;
    gills->scan_funcs_val[gills->scan_funcs_max_idx] = scan_val;
    gills->scan_val_size[gills->scan_funcs_max_idx] = val_size;
}

void gills_cleanup (gills_context_t *gills)
{
//    free(gills->pnodemem);
//    free(gills->listnodearr);
 //   free(gills->pnodeptrs);
    free(gills->pstack);
//    free(gills->pstackidx_stack);
#ifndef GILLS_MEM_TKSTACK_UNLIM
 //   free(gills->tkstack);
#endif
 //   free(gills->scan_funcs);
    free(gills->scan_funcs_val);
    free(gills->scan_val_size);
 //   free(gills->pnodemem);
    mem_pool_free(gills, gills->pnodemem);
    free(gills->listnodearr);
    free(gills);
}



void exit_parse (gills_context_t *gills, int exit_ret)
{
    gills_cleanup(gills);
    exit(exit_ret);
}

int generate_parse (gills_context_t *gills,
                    parse_node_t *start_pnode,
                    token_node_t *starttoken,
                    rule_node_t *rule_node,
                    int floating,
                    parse_node_t *gen_recur_pnode,
                    parse_node_t **retpnode,
                    parse_node_t **lastres_pnode)
{
    token_node_t *tnode = starttoken, *rnode;
    parse_node_t *pnode, *uppnode, *lretpnode, *rpnode, *grpnode, *recpnode, *crecpnode, *recuruppnode, *crecuruppnode, *fpnode, *fpnode1, *recnxtpnode, *uprecpnode, *recuppnode, *toplastexptk, *recupfl_pnode;
    int rnum = 0, upflag = 0, updir = 0, toprecur, pstackidx_start = gills->pstack_idx;
    rule_t *rules, *rule;
    rule_node_t *rlnode, *nxtrlnode = NULL;
    list_t *lnode, *up_recur_list, *up_precur_list, *reclist, *reclist1, *parselistnode, *listnode;
    int i, recur, ret = 0, top_reached = 0, fl = 1, recur_up_top;

    fpnode = NULL;
    pnode = start_pnode;
    if (start_pnode) { 
        fpnode = pnode;
        nxtrlnode = NULL;
        toprecur = 0;
        while (1) {
            fpnode = pnode;
            if (fpnode->token->token_num == 0) {
                fpnode->res = 1;
                if (retpnode)
                    *retpnode = fpnode;
                gills->topreach = 1;
                if (updir && (pnode->recur || pnode->int_recur ||
                    pnode->right_recur)) {
                    updir = 0;
                    toprecur = 1;
                    break;
                }
                if (toprecur) {
                    toprecur = 0;
                    break;
                }
                return 0;
            }
            rlnode = pnode->rule_node;
            if (rlnode->num != rlnode->rule->rule_nodes_num - 1) {

                nxtrlnode = &(rlnode->rule->rule_nodes[rlnode->num + 1]);
                tnode = nxtrlnode->token;
                break;
            } else {
                fpnode1 = fpnode;
                fpnode = fpnode1->rl_next;
                rule = fpnode->rule_node->rule;
                if (fpnode == fpnode1) {
                    fpnode->rule_start = fpnode->rule_end = 1;
                }

            
                if (fpnode->rule->token->crecur &&
                    (!fpnode->crecur && fpnode1->right_recur)) {
                    updir = 0;
                    pnode = fpnode1;
                    goto crecurgenlabel;
                }

                if (fpnode->rule->token->crecur &&
                    fpnode->crecur) {
                    updir = 0;
                    fpnode->res = 1;
                    if (fpnode->recup_pnode->recres) {
                        printf("ambiguous syntax token %s %d"
                               "with token %s %d and %s %d ...exiting\n",
                               fpnode->crecurup_pnode->token->token_name,
                               fpnode->crecurup_pnode->token->token_num,
                               fpnode->crecurup_pnode->recdn_pnode->token->token_name,
                               fpnode->crecurup_pnode->recdn_pnode->token->token_num,
                               fpnode->token->token_name, fpnode->token->token_num);
                        return CREC_AMBIGUOUS;
//                    exit_parse(gills, -1);
                    }
                    free_parsereclist_lnode(gills, fpnode);
                    fpnode->recup_pnode->recdn_pnode = fpnode;
                    if (!fpnode->recup_pnode->recur &&
                        !fpnode->recup_pnode->int_recur &&
                        !fpnode->recup_pnode->right_recur)
                        fpnode->recup_pnode->recres = 1;
                    pnode = fpnode1->right_recur ? fpnode1 : fpnode;
                    goto crecurgenlabel;
                }

                if (rule->crecur &&
                    !rule->right_recur) {
                    pnode = fpnode;
                    goto crecurreducelabel;
                } else if (!rule->crecur) {
                    pnode = fpnode;
                    goto upreducelabel;
                } else if (rule->crecur && rule->right_recur) { 
                    pnode = fpnode;
                    goto crecurreducelabel;
                }

                if (fpnode->crecur) {
crecurreducelabel:
                    fpnode->res = 1;
                    if (fpnode->recup_pnode->recres) {
                        printf("ambiguous syntax token %s %d"
                                "with token %s %d and %s %d ...exiting\n",
                                fpnode->crecurup_pnode->token->token_name,
                                fpnode->crecurup_pnode->token->token_num,
                                fpnode->crecurup_pnode->recdn_pnode->token->token_name,
                                fpnode->crecurup_pnode->recdn_pnode->token->token_num,
                                fpnode->token->token_name, fpnode->token->token_num);
                        return CREC_AMBIGUOUS;
//                    exit_parse(gills, -1);
                    }
                    free_parsereclist_lnode(gills, fpnode);
                    fpnode->recup_pnode->recdn_pnode = fpnode;
                    if (!fpnode->recup_pnode->recur &&
                        !fpnode->recup_pnode->int_recur &&
                        !fpnode->recup_pnode->right_recur)
                        fpnode->recup_pnode->recres = 1;
                }
upreducelabel:
                uppnode = fpnode->up;
                if (!uppnode->up) {
                    gills->topreach = 1;
                    uppnode->res = 1;
                    uppnode->dnres_pnode = pnode;
                    uppnode->dn = list_del_node(gills, uppnode->dn,
                                                pnode->updn_lnode);
                    toplastexptk = get_pnode(gills);
                    toplastexptk->rl_next = toplastexptk->rl_prev =
                    toplastexptk;
                    memcpy(toplastexptk, &gills->top_last_exp_pnode,
                           sizeof(parse_node_t));
                    toplastexptk->lasttk_toppnode = uppnode;
#ifndef GILLS_MEM_TKSTACK_UNLIM
                    gills->tkstack[gills->tkstack_idx] =
                         list_add(gills, gills->tkstack[gills->tkstack_idx],
                                  toplastexptk, &lnode); 
                    toplastexptk->tkstackidx = gills->tkstack_idx;
                    toplastexptk->tkstack_lnode = lnode;
#else
                    list_mesh_add(gills, &gills->tkstack_listmesh,
                                  &gills->nextop_tkstack_listmesh_list,
                                  (void *)toplastexptk,
                                   &gills->nextop_tkstack_listmesh_llist,
                                   &toplastexptk->tkstack_lnode);
                    toplastexptk->tkstack_llnode =
                        gills->nextop_tkstack_listmesh_list;
                    toplastexptk->tkstack_llmeshnode =
                        gills->nextop_tkstack_listmesh_llist;
#endif
                    toplastexptk->lasttk_toppnode = uppnode;
                    toplastexptk->lasttktotoprule_pnode = fpnode;
                    uppnode->top_lasttk_pnode_list =
                    list_add(gills, uppnode->top_lasttk_pnode_list,
                             toplastexptk, &lnode);
                    toplastexptk->toplist_lnode = lnode;
                    if (floating)
                        toplastexptk->lasttk_top_floating = 1;
                    if (retpnode)
                        *retpnode = toplastexptk;
                    if (lastres_pnode)
                        *lastres_pnode = toplastexptk;
                    return 0;
                }
                uppnode->res = 1;
                uppnode->dnres_pnode = pnode;
#ifndef GILLS_FAST
                uppnode->dn = list_del_node(gills, uppnode->dn,
                                            pnode->updn_lnode);
#endif
                pnode = uppnode;
                updir = 1;
                if (updir &&
                    pnode &&
                    pnode->token->crecur &&
                    (pnode->recur || pnode->int_recur)) {
                    updir = 0;
                    goto crecurgenlabel;
                }
            }
        }
    }

    fpnode = pnode;
    if (lastres_pnode)
        *lastres_pnode = fpnode ? fpnode : start_pnode;
    pnode = get_pnode(gills);
    if (!pnode)
        return MEM_EXHAUSTED;

    pnode->rl_next = pnode->rl_prev = pnode;

    if (!fpnode) {
        pnode->rule_start = pnode->rule_end = 1;
    }

    pnode->up = fpnode ? fpnode->up : NULL;
    if (nxtrlnode && nxtrlnode->num == nxtrlnode->rule->rule_nodes_num - 1) {
        pnode->rule_end = 1;
    }
    if (fpnode) {
        fpnode->rl_next->rl_prev = pnode;
        pnode->rl_next = fpnode->rl_next;
        fpnode->rl_next = pnode;
        pnode->rl_prev = fpnode;
    }
    pnode->token = tnode;
    pnode->rule_node = nxtrlnode;
    pnode->rule = nxtrlnode ? nxtrlnode->rule : NULL;
    pnode->nonterm = tnode->nonterm;
    if (retpnode) *retpnode = pnode;
    if (pnode && nxtrlnode && nxtrlnode->right_recur) {
#ifdef OPPREC_ASSOC_BOTTOM
        parse_set_rule_assoc_prec(gills, pnode);
#endif
#define OPPREC_ASSOC_LAST_TERM 1
#ifdef OPPREC_ASSOC_LAST_TERM
        parse_set_rule_assoc_prec_last_term(gills, pnode);
#endif        
    }


    if (pnode->token->rules_num) {
        gills->pstack_idx += 1;
        gills->pstack[gills->pstack_idx] = pnode;
    } else {
        pnode->prec = pnode->token->prec;
        pnode->assoc = pnode->token->assoc;
        pnode->rule = pnode->rule_node->rule;
        if (pnode != start_pnode) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
            gills->tkstack[gills->tkstack_idx] = 
                list_add(gills, gills->tkstack[gills->tkstack_idx],
                         pnode, &lnode);
            pnode->tkstackidx = gills->tkstack_idx;
            pnode->tkstack_lnode = lnode;
#else
            list_mesh_add(gills, &gills->tkstack_listmesh,
                          &gills->nextop_tkstack_listmesh_list,
                          (void *)pnode,
                           &gills->nextop_tkstack_listmesh_llist,
                           &pnode->tkstack_lnode);
            pnode->tkstack_llnode = gills->nextop_tkstack_listmesh_list;
            pnode->tkstack_llmeshnode = gills->nextop_tkstack_listmesh_llist;
#endif
        }
        if (retpnode)
            *retpnode = pnode;
        return 0;
    }
    if (!start_pnode && !rule_node) { 
        pnode->rule_node = &gills->top_rule_node;
        gills->toppnode = pnode;
    }

    while (1) {
        pnode = gills->pstack[gills->pstack_idx];
        gills->pstack_idx--;
        rules = pnode->token->rules;
        
        pnode->recur = pnode->rule_node->recur;
        pnode->crecur = pnode->rule_node->crecur;
        pnode->right_recur = pnode->rule_node->right_recur;
        pnode->int_recur = pnode->rule_node->int_recur;
        pnode->nonterm = pnode->token->nonterm;
        if ((!floating && 
             pnode->res && !pnode->recres) &&
            (pnode->recur ||
             pnode->right_recur ||
             pnode->crecur))
            goto crecurgenlabel;
        for (i = 0; i < pnode->token->rules_num; i++) {
            rlnode = rules[i].rule_nodes;
            rnum = 0;
            if (rlnode->crecur && !pnode->res &&
                (pnode->int_recur ||
                 pnode->right_recur)) {
                pnode->recur_list = pnode->up->recur_list;
                continue;
            } 
            rpnode = get_pnode(gills);
            if (!rpnode) {
                return MEM_EXHAUSTED;
            }
            rpnode->rule_start = 1;
            rpnode->rl_prev = rpnode->rl_next = rpnode;
            rpnode->token = rlnode->token;
            rpnode->nonterm = rpnode->token->nonterm;
            rpnode->rule_node = rlnode;
            rpnode->rule = rlnode ? rlnode->rule : NULL;
            rpnode->recur = rlnode->recur;
            rpnode->right_recur = rlnode->right_recur;
            rpnode->crecur = rlnode->crecur;
            rpnode->int_recur = rlnode->int_recur;
            rpnode->uprecur_pnode = pnode->recur ? pnode :
                                    pnode->uprecur_pnode;
            recur = 0;
            if (!pnode->res &&
                pnode->recur && rpnode->crecur) {
                pnode->recur_list = list_add(gills, pnode->recur_list, rpnode, NULL);
            }
            if (!rpnode->crecur) {
                pnode->dn = list_add(gills, pnode->dn, rpnode, &lnode);
                rpnode->updn_lnode = lnode;
                rpnode->up = pnode;
                if (rpnode->token->rules_num) {
                    gills->pstack_idx++;
                    gills->pstack[gills->pstack_idx] = rpnode;
                    rpnode->uprnum = rnum;
            rnum += 1;
                }
                if (!rpnode->token->rules_num) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
                    gills->tkstack[gills->tkstack_idx] =
                        list_add(gills, gills->tkstack[gills->tkstack_idx],
                                 rpnode, &lnode);
                    rpnode->tkstackidx = gills->tkstack_idx;
                    rpnode->tkstack_lnode = lnode;
#else
                    list_mesh_add(gills, &gills->tkstack_listmesh,
                                  &gills->nextop_tkstack_listmesh_list,
                                  (void *)rpnode,
                                  &gills->nextop_tkstack_listmesh_llist,
                                  &rpnode->tkstack_lnode);
                    rpnode->tkstack_llnode =
                        gills->nextop_tkstack_listmesh_list;
                    rpnode->tkstack_llmeshnode =
                        gills->nextop_tkstack_listmesh_llist;
#endif
                    if (rpnode->token->token_num == gills->empty_tk_num) {
                        rpnode->res = 1;
                        ret = reduce_parse(gills, rpnode);
                        if (ret) {
                            printf("error ...exiting\n");
                            exit_parse(gills, -1);
                        }
                    }
                }
            }
        }
        if (gills->pstack_idx == pstackidx_start)
            break;
    }
crecurgenlabel:
    if ((pnode->res && !pnode->recres &&
         pnode->token->crecur &&
         (pnode->recur || pnode->right_recur ||
          pnode->int_recur)) ||
         (!pnode->recur &&
          !pnode->right_recur && pnode->crecur &&
           pnode->res && !pnode->recres)) {
        reclist = pnode->crecur ?
                      pnode->up->recur_list :
                      pnode->recur_list;
        reclist1 = reclist;
        parselistnode = list_create(gills);
        pnode->parse_reclist = list_add(gills, pnode->parse_reclist,
                                        parselistnode, &listnode);

        while (reclist1) {
            reclist = reclist->next;
            if (reclist == reclist1)
                break;
            recpnode = (parse_node_t *)reclist->data;
            crecpnode = get_pnode(gills);
            crecpnode->rule_start = 1;
            crecpnode->rl_next = crecpnode->rl_prev = crecpnode;
            crecpnode->token = recpnode->token;
            crecpnode->rule_node = recpnode->rule_node;
            crecpnode->rule = recpnode->rule_node ?
                              recpnode->rule_node->rule :
                              NULL;
            crecpnode->recup_pnode = pnode;
            parselistnode = list_add(gills, parselistnode,
                                     crecpnode, &lnode);
            crecpnode->crecur = 1;
            crecpnode->parserec_llnode = listnode;
            crecpnode->parserec_lnode = lnode;
            crecpnode->recurup_pnode = crecpnode->up = pnode->recur ? pnode : pnode->up;
            crecpnode->crecurup_pnode = pnode;
            crecpnode->uprecur_pnode = pnode->uprecur_pnode;
            crecpnode->assoc = crecpnode->token->assoc;
            crecpnode->prec = crecpnode->token->prec;
                 ret = generate_parse(gills, crecpnode,
                       crecpnode->token, 
                       crecpnode->rule_node, 0,
                       crecpnode, NULL,
                       NULL);
                 crecpnode->res = 1;
        }

        if ((pnode->res && !pnode->recres &&
             pnode->token->crecur &&
             (pnode->recur || pnode->int_recur ||
              pnode->right_recur)) ||
             (pnode->res && !pnode->recres && pnode->crecur)) {
            fl = 1;
            fpnode = pnode;
            while (1) {
                if (fpnode->crecur) {
                    fpnode = fpnode->up;
                } else if (fpnode->right_recur) {
                    fpnode = fpnode->rl_next;
                } else if (fpnode->recur || fpnode->int_recur) {
                    break;
                }
            }
                         
            lretpnode = recnxtpnode = NULL;
            ret = generate_parse(gills, fpnode, NULL,
                                 fpnode->rule_node, fl, NULL,
                                 &lretpnode, &recnxtpnode);
            if (ret) {
                printf("generate_parse %d failed ...exiting\n", ret);
                return ret;
//                         exit_parse(gills, -1);
            }
            recnxtpnode->floating = 1;
        }
    }
    return 0;
}

int get_token (gills_context_t *gills, int scan_idx)
{
    int tk;
    tk = gills->scan_funcs[scan_idx](gills->scan_funcs_val[scan_idx], gills->yyscan);

    return tk;
}

void action_ex (gills_context_t *gills,
                parse_node_t *parsenode, parse_node_t *rlpnode)
{
    parse_node_t *pnode = parsenode, *lrlpnode = rlpnode;
    int i = 1;

    if (!lrlpnode->rule->action) {
        pnode->val = rlpnode->val;
        pnode->act = 1;
        return;
    }
    gills->act_val[0] = NULL;
    while (1) {
        gills->act_val[i] = rlpnode->val;
        if (rlpnode->rule_end)
            break;
        i++;
        rlpnode = rlpnode->rl_next;
    }
    if (lrlpnode->rule->action)
        (lrlpnode->rule->action)(gills);
    if (!gills->act_val[0])
        pnode->val = gills->act_val[1];
    pnode->act = 1;
}

void change_parsereclist_recup_pnode (list_t *parse_reclist,
                                      parse_node_t *recup_pnode,
                                      parse_node_t *up_pnode)
{
    list_t *parsereclist = parse_reclist, *parselistnode, *listnode;
    parse_node_t *fpnode;

    if (!parse_reclist)
        return;
    while (1) {
        parsereclist = parsereclist->next;
        if (parsereclist == parse_reclist)
            break;
        parselistnode = (list_t *)(parsereclist->data);
        listnode = parselistnode;
        if (!listnode)
            continue;
        while (1) {
            listnode = listnode->next;
            if (listnode == parselistnode)
                break;
            fpnode = (parse_node_t *)(listnode->data);
            fpnode->recup_pnode = recup_pnode;
            fpnode->up = up_pnode;
        }
    }
}

void action_ex_inline (gills_context_t *gills,
                       parse_node_t *tkpnode,
                       parse_node_t *crecur_begin_pnode)
{
    parse_node_t *pnode = tkpnode, *uppnode, *uprecpnode2, *rlpnode, *rlpnode1, *rlnxtpnode, *fpnode, *dnrecpnode, *dnrecpnode1, *rtdnrecpnode, *rtdnrecpnode1, *uprecpnode, *uprecpnode1, *toplasttkpnode;
    int first = 1, rlnextflag, dndir = 0;
    int crecur, right_recur, int_recur, recur, assoc, prec, dncrecur, dnright_recur, dnassoc, dnprec, upcrecur, upright_recur, upassoc, upprec;
    int rule_prec, prev_rule_prec, rule_assoc, prev_rule_assoc, rtdncrecur, rtdnright_recur, rtdnassoc, rtdnprec;
    int uppnodereached = 0, lasttk_pass;
    list_t *toplasttklist;

    if (tkpnode->token->token_num == gills->last_tk_num &&
        gills->topreach) {

        toplasttklist = tkpnode->lasttk_toppnode->top_lasttk_pnode_list;
        tkpnode->lasttk_toppnode->top_lasttk_pnode_list =
            list_del_node(gills, toplasttklist, tkpnode->toplist_lnode);
        pnode = tkpnode->lasttktotoprule_pnode;
        if ((!pnode || !pnode->token) &&
            tkpnode->lasttk_toppnode->topres &&
            !tkpnode->lasttk_toppnode->dn &&
            !tkpnode->lasttk_toppnode->dnres_pnode) {
            free_pnode(gills, gills->toppnode);
            free_pnode(gills, tkpnode);
            return;
        }
        lasttk_pass = 1;
    }
    rlpnode = pnode;
    if (lasttk_pass ||
        (crecur_begin_pnode &&
         tkpnode != crecur_begin_pnode))  {
        while (1) {
            if (rlpnode->nonterm && rlpnode->dnres_pnode) {
                rlpnode = rlpnode->dnres_pnode;
            } else {
                pnode = rlpnode;
                break;
            }
        }
    }
    while (1) {
action_loop_start:
        uppnodereached = 0;
        rlpnode = pnode;
        if (!rlpnode->res)
            return;
        if (!rlpnode->nonterm)
            pnode->act = 1;
        if (rlpnode->recur || rlpnode->int_recur || rlpnode->right_recur) {
            if (rlpnode->act &&
                !rlpnode->recact) {
                if (rlpnode->recdn_pnode) {
                    rlpnode = rlpnode->recdn_pnode;
                    pnode = rlpnode;
                    dndir = 1;
                    goto action_crecur_loop_start;
                } else if (!rlpnode->parse_reclist) {
                    pnode->recact = 1;
                } else if (!rlpnode->right_recur) {
                    return;
                } 
            }
        }
        if (!rlpnode->crecur && (((!rlpnode->recur && 
            !rlpnode->int_recur &&
            !rlpnode->right_recur && rlpnode->act)) ||
            ((rlpnode->recur || 
              rlpnode->int_recur ||
              rlpnode->right_recur) &&
              rlpnode->act &&
              rlpnode->recact))) {
            fpnode = rlpnode;
            while (1) {
                if (!rlpnode->rule_end) {
                    if(rlpnode->nonterm) {
                       rlpnode = rlpnode->rl_next;
                       if (!rlpnode->res)
                           return;
                       rlnextflag = 1;
                       break;
                    } else {
                        rlpnode->act = 1;
                        rlpnode = rlpnode->rl_next;
                        if (!rlpnode->res)
                            return;
                    }
                } else {
                    if (!rlpnode->nonterm) {
                        rlpnode->act = 1;
                        rlnextflag = 0;
                        break;
                    } 
                    if (!rlpnode->act) 
                        rlnextflag = 1;
                    else
                        rlnextflag = 0;
                    break;
                }
            }
            if (rlnextflag) {
                pnode = rlpnode;
                if (pnode->nonterm) {
                    rlpnode = pnode;
                    while (1) {
#ifndef GILLS_FAST
                        if (!rlpnode->dnres_pnode ||
                            rlpnode->dn)
#else
                        if (!rlpnode->dnres_pnode)
#endif
                            return;

                        rlpnode = rlpnode->dnres_pnode;
                        if (!rlpnode->nonterm) {
                            pnode = rlpnode;
                            goto action_loop_start;
                        }
                    }
                }
                goto action_loop_start;
            } else {
                rlpnode1 = rlpnode;
                rlpnode = rlpnode->rl_next;

                if (!crecur_begin_pnode && rlpnode->crecur) {
                    rlpnode->act = 1;
                    goto action_crecur_loop_start;
                }
                if (crecur_begin_pnode) {
                    if (!rlpnode->right_recur &&
                        crecur_begin_pnode == rlpnode)
                        return;

                    if (rlpnode->up &&
                        rlpnode->up->right_recur) {
                        fpnode = rlpnode->up;
                        fpnode = fpnode->rl_next;
                        if (fpnode == crecur_begin_pnode) {
                            action_ex(gills, rlpnode->up, rlpnode);
                            rlpnode->up->act = 1;
                            rlpnode->up->dnres_pnode = NULL;
                            free_parse_rule(gills, rlpnode);
                            return;
                        }
                    }
                }
                action_ex(gills, rlpnode->up, rlpnode);
                pnode = rlpnode->up;
                free_parse_rule(gills, rlpnode);
                pnode->dnres_pnode = NULL;
                if (pnode == gills->toppnode) {
                    if (pnode->topres) {
                        toplasttklist = pnode->top_lasttk_pnode_list;
                        while (toplasttklist) {
                            toplasttklist = toplasttklist->next;
                            toplasttkpnode = (parse_node_t *)
                                             (toplasttklist->data);
                            free_pnode(gills, toplasttkpnode);
                            toplasttklist->data = NULL;
                            if (toplasttklist->next ==
                                pnode->top_lasttk_pnode_list)
                            break;
                        }
                        free_list(pnode->top_lasttk_pnode_list); 
                        free_pnode(gills, gills->toppnode);
                        gills->toppnode = NULL;
                        return;
                    } else {
                        return;
                    }
                }
                goto action_loop_start;
            }
        }
        if (rlpnode->rule_end) {
            rlpnode1 = rlpnode;
            rlpnode = rlpnode1->rl_next;
        }
action_crecur_loop_start:
        uppnodereached = 0;
        rlpnode1 = rlpnode->rl_prev;
        crecur = rlpnode->crecur;
        right_recur = rlpnode1->right_recur;
        if (right_recur) {
            assoc = rlpnode1->assoc;
            prec = rlpnode1->prec;
        } else if (crecur) {
            assoc = LEFT_ASSOC;
            prec = 0;
        } else {
            assoc = 0;
            prec = 0;
        }
        uppnode = rlpnode->up;
        if (rlpnode->crecur && rlpnode->recdn_pnode) {
            dnrecpnode = rlpnode->recdn_pnode;
            dncrecur = dnrecpnode->crecur;
            dnrecpnode1 = dnrecpnode->rl_prev;
            if (!dnrecpnode->res)
                return;
            dnright_recur = dnrecpnode1->right_recur;
            if (dnright_recur) {
                dnassoc = dnrecpnode1->assoc;
                dnprec = dnrecpnode1->prec;
            } else if (dncrecur) {
                dnassoc = LEFT_ASSOC;
                dnprec = 0;
            } else {
                dnassoc = 0;
                dnprec = 0;
            }
        } else {
            dnrecpnode = dnrecpnode1 = NULL;
            dncrecur = 0;
            dnright_recur = 0;
            dnassoc = dnprec = 0;
        }
        uprecpnode = rlpnode->recup_pnode;
        if (uprecpnode) {
            if (uprecpnode->crecur) {
                uprecpnode1 = uprecpnode->rl_prev;
                upcrecur = uprecpnode->crecur;
                uppnodereached = 0;
                if (!uprecpnode->res || !uprecpnode1->res)
                    return;
                upright_recur = 0;
                upassoc = LEFT_ASSOC;
                upprec = 0;
                uppnodereached = 0;
            } else if (uprecpnode->right_recur) {
                uprecpnode1 = uprecpnode;
                uprecpnode = uprecpnode1->rl_next;
                if (!uprecpnode->res || !uprecpnode1->res)
                    return;
                upassoc = uprecpnode1->assoc;
                upprec = uprecpnode1->prec;
                upright_recur = uprecpnode1->right_recur;
                upcrecur = uprecpnode->crecur;
                uppnodereached = 1;
            } else if (uprecpnode->recur &&
                       uprecpnode == rlpnode->up) {
                uppnode = rlpnode->up;
                uprecpnode = uprecpnode1 = NULL;
                upcrecur = upright_recur = 0;
                upassoc = upprec = 0;
                uppnodereached = 1;
            }
        }
        if (rlpnode1->right_recur &&
            rlpnode1->recdn_pnode) {
            if (rlpnode1->recdn_pnode) {
                rtdnrecpnode = rlpnode1->recdn_pnode;
                rtdnrecpnode1 = rtdnrecpnode->rl_prev;
                rtdncrecur = rtdnrecpnode->crecur;
                if (!rtdnrecpnode->res)
                    return;
                rtdnright_recur = rtdnrecpnode1->right_recur;
                if (rtdnright_recur) {
                    rtdnassoc = rtdnrecpnode1->assoc;
                    rtdnprec = rtdnrecpnode1->prec;
                } else if (rtdncrecur) {
                    rtdnassoc = LEFT_ASSOC;
                    rtdnprec = 0;
                } else {
                    rtdnassoc = 0;
                    rtdnprec = 0;
                }
            }
        } else {
            rtdnrecpnode = rtdnrecpnode1 = NULL;
            rtdncrecur = rtdnright_recur = 0;
            rtdnassoc = rtdnprec = 0;
        }

        if (!crecur && !right_recur) {
            action_ex(gills, rlpnode->up, rlpnode);
            pnode = rlpnode->up;
            rlpnode->up->dnres_pnode = NULL;
            free_parse_rule(gills, rlpnode);
            continue; //break;
        }
        if ((crecur && !right_recur && !dnrecpnode) ||
            (crecur && right_recur &&
             !dnrecpnode && !rtdnrecpnode)) {
#ifndef GILLS_FAST
            if (rlpnode->recup_pnode->parse_reclist)
                return;
            if (uppnodereached && !upright_recur) {
                if ((!((!assoc || assoc == LEFT_ASSOC) && !prec)) &&
                    ((crecur && !right_recur && rlpnode->parse_reclist) ||
                     (crecur && right_recur &&
                      rlpnode1->parse_reclist))) {
                    return;
                }
#endif
                if (!rlpnode->act)
                    action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                rlpnode->val = rlpnode->up->val;
                action_ex(gills, rlpnode, rlpnode);
                rlpnode->up->val = rlpnode->val;
                pnode = rlpnode->up;
#ifndef GILLS_FAST
                if (!rlpnode->parse_reclist &&
                    !rlpnode1->parse_reclist)
#endif
                    pnode->recact = 1;
                if (crecur && !right_recur) {
                    rlpnode->up->parse_reclist = rlpnode->parse_reclist;
                    change_parsereclist_recup_pnode(rlpnode->parse_reclist,
                                                rlpnode->up, rlpnode->up);
                    rlpnode->parse_reclist = NULL;
                } else if (crecur && right_recur) {
                    rlpnode->up->parse_reclist = rlpnode1->parse_reclist;
                    if (rlpnode->up->parse_reclist)
                        rlpnode->up->recres = 0;
                    change_parsereclist_recup_pnode(rlpnode1->parse_reclist,
                                                rlpnode->up, rlpnode->up);
                    rlpnode1->parse_reclist = NULL;
                }
                free_parse_rule(gills, rlpnode);
                pnode->recdn_pnode = NULL;
                if (pnode->parse_reclist)
                    return;
                dndir = 0;
                goto action_loop_start;
            } else {
#ifndef GILLS_FAST
                if ((crecur && !right_recur && rlpnode->parse_reclist) ||
                    (crecur && right_recur && rlpnode1->parse_reclist)) {
#else
                if ((crecur && !right_recur) ||
                    (crecur && right_recur)) {
#endif
                    if (dndir)
                       return;
                    pnode = rlpnode = uprecpnode;
                    rlpnode = pnode;
                    dndir = 0;
                    goto action_crecur_loop_start;
                }
                if ((!prec && !upprec && assoc >= upassoc) ||
                    (prec >= upprec)) {
                    if (upright_recur && uprecpnode1) {
                        uprecpnode2 = uprecpnode1;
                    } else {
                        uprecpnode2 = uprecpnode;
                    }
                    if (!rlpnode->act)
                        action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                    rlpnode->val = uprecpnode2->val;
                    action_ex(gills, rlpnode, rlpnode);
                    uprecpnode2->val = rlpnode->val;
                    uprecpnode2->recdn_pnode = NULL;
                    if (crecur && !right_recur) {
                        uprecpnode2->parse_reclist =
                                     rlpnode->parse_reclist;
                        rlpnode->parse_reclist = NULL;
                        change_parsereclist_recup_pnode(uprecpnode2->parse_reclist, uprecpnode2, uprecpnode2);
                    } else if (crecur && right_recur){
                        uprecpnode2->parse_reclist = rlpnode1->parse_reclist;
                        rlpnode1->parse_reclist = NULL;
                        change_parsereclist_recup_pnode(uprecpnode2->
                            parse_reclist, uprecpnode2, uprecpnode2);
                    }
                    if (uprecpnode2->parse_reclist)
                        uprecpnode2->recres = 0;
                    free_parse_rule(gills, rlpnode);
                    pnode = uprecpnode;
                    if (uprecpnode2 == uprecpnode1) {
                        if (!rlpnode->parse_reclist &&
                            !rlpnode1->parse_reclist)
                            uprecpnode1->recact = 1;
                    }
                        rlpnode = pnode;
                        dndir = 0;
                        goto action_crecur_loop_start;
                } else {
                        if (uprecpnode1 == rlpnode->recup_pnode) {
                            uprecpnode2 = uprecpnode1;
                        } else {
                            uprecpnode2 = uprecpnode;
                        }
                        pnode = uprecpnode;
                        rlpnode = pnode;
                        dndir = 0;
                        goto action_crecur_loop_start;
                }
            }
        }
        if (crecur && !right_recur &&
            dnrecpnode && !rtdnrecpnode) {
            if ((((!prec && !dnprec && assoc >= dnassoc) ||
                (prec >= dnprec)) &&
               ((uppnodereached &&
                 !upright_recur) ||
                (((!prec && !upprec && assoc > upassoc) ||
                ((prec >= upprec))))))) {
                if (uppnodereached && !upright_recur) {
                    rlpnode->val = rlpnode->up->val;
                } else {
                    if (upright_recur && uprecpnode1) {
                        rlpnode->val = uprecpnode1->val;
                    } else {
                        rlpnode->val = uprecpnode->val;
                    }
                }
                if (!rlpnode->act)
                    action_ex_inline(gills, rlpnode->rl_next,
                                     rlpnode);
                action_ex(gills, rlpnode, rlpnode);
                if (uppnodereached &&
                    !upright_recur) {
                    rlpnode->up->val = rlpnode->val;
                    rlpnode->up->recdn_pnode = dnrecpnode;
                    rlpnode->up->parse_reclist =
                                 rlpnode->parse_reclist;
                    change_parsereclist_recup_pnode(
                    rlpnode->parse_reclist,
                    rlpnode->up, rlpnode->up);
                    dnrecpnode->recup_pnode = rlpnode->up;
                    dnrecpnode->up = rlpnode->up;
                    rlpnode->parse_reclist = NULL;
                    free_parse_rule(gills, rlpnode);
                    pnode = dnrecpnode;
                    rlpnode = pnode;
                    goto action_crecur_loop_start;
                } else {
                    if (upright_recur) {
                        uprecpnode2 = uprecpnode1;
                    } else {
                        uprecpnode2 = uprecpnode;
                    }
                    uprecpnode2->val = rlpnode->val;
                    uprecpnode2->recdn_pnode = dnrecpnode;
                    dnrecpnode->recup_pnode = uprecpnode2;
                    dnrecpnode->up = upright_recur ?
                                     uprecpnode1 : uprecpnode2->up;
                    change_parsereclist_recup_pnode(
                                rlpnode->parse_reclist,
                                uprecpnode2, dnrecpnode->up);
                    uprecpnode2->parse_reclist =
                                 rlpnode->parse_reclist;
                    rlpnode->parse_reclist = NULL;
                    free_parse_rule(gills, rlpnode);
                    if (upassoc > dnassoc ||
                        (upassoc == dnassoc &&
                         upprec >= dnprec)) {
                         dndir = 0;
                         pnode = uprecpnode;
                    } else {
                        dndir = 1;
                        if (!rlpnode->act)
                            action_ex_inline(gills,
                                rlpnode->rl_next, rlpnode);
                        pnode = dnrecpnode;
                    }
                    rlpnode = pnode;
                    goto action_crecur_loop_start;
                }
            } else {
                if ((!upprec && !prec && upassoc >= assoc) ||
                    (upprec >= prec)) {
                    dndir = 0;
                    pnode = uprecpnode;
                } else if ((!dnprec && !prec && dnassoc > assoc) ||
                    (dnprec >= prec)) {
                    dndir = 1;
                    if (!rlpnode->act)
                        action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                    pnode = dnrecpnode;
                }
                rlpnode = pnode;
                goto action_crecur_loop_start;
            }
        }

        if (crecur && right_recur &&
            !dnrecpnode && rtdnrecpnode) {
            if ((((!prec && !rtdnprec && assoc >= rtdnassoc) ||
                  (prec >= rtdnprec)) &&
                  ((uppnodereached && !upright_recur) ||
                  (((!prec && !upprec && assoc >= upassoc) ||
                 ((prec >= upprec))))))) {
                if (uppnodereached && !upright_recur) {
                    rlpnode->val = rlpnode->up->val;
                } else {
                    if (upright_recur && uprecpnode1) {
                        rlpnode->val = uprecpnode1->val;
                    } else {
                        rlpnode->val = uprecpnode->val;
                    }
                }
                if (!rlpnode->act)
                    action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                action_ex(gills, rlpnode, rlpnode);
                if (uppnodereached && !upright_recur) {
                    rlpnode->up->val = rlpnode->val;
                    rlpnode->up->recdn_pnode = rtdnrecpnode;
                    rtdnrecpnode->recup_pnode = rlpnode->up;
                    rtdnrecpnode->up = rlpnode->up;
                    rlpnode->up->parse_reclist = rlpnode1->parse_reclist;
                    change_parsereclist_recup_pnode(rlpnode1->parse_reclist, 
                    rlpnode->up, rlpnode->up);
                    rlpnode1->parse_reclist = NULL;
                    free_parse_rule(gills, rlpnode);
                    pnode = rtdnrecpnode;
                    rlpnode = pnode;
                    goto action_crecur_loop_start;
                } else {
                    if (upright_recur) {
                        uprecpnode2 = uprecpnode1;
                    } else {
                        uprecpnode2 = uprecpnode;
                    }
                    uprecpnode2->val = rlpnode->val;
                    uprecpnode2->recdn_pnode = rtdnrecpnode;
                    rtdnrecpnode->recup_pnode = uprecpnode2;
                    rtdnrecpnode->up = upright_recur ? uprecpnode1 : uprecpnode->up;
                    uprecpnode2->parse_reclist = rlpnode1->parse_reclist;
                    change_parsereclist_recup_pnode(rlpnode1->parse_reclist,
                    uprecpnode2, rtdnrecpnode->up);
                    rlpnode1->parse_reclist = NULL;
                    free_parse_rule(gills, rlpnode);
                    if ((!upprec && !rtdnprec &&
                          upassoc > rtdnassoc) ||
                        (upprec >= rtdnprec)) {
                        dndir = 0;
                        pnode = uprecpnode;
                    } else {
                        dndir = 1;
                        pnode = rtdnrecpnode;
                    }
                    rlpnode = pnode;
                        goto action_crecur_loop_start; //continue;
                }
            } else {
                if (uppnodereached && !upright_recur) {
                    if (!rlpnode->act)
                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                    pnode = rtdnrecpnode;
                    rlpnode = pnode;
                    dndir = 1;
                    goto action_crecur_loop_start;
                } else {
                    if ((!upprec && !prec && upassoc > assoc) ||
                        (upprec >= prec)) {
                        dndir = 0;
                        pnode = uprecpnode;
                    } else if ((!rtdnprec && !prec &&
                                 rtdnassoc > assoc) ||
                                (rtdnprec >= prec)) {
                        if (!rlpnode->act)
                            action_ex_inline(gills,
                                rlpnode->rl_next, rlpnode);
                        dndir = 1;
                        pnode = rtdnrecpnode;
                    }
                    rlpnode = pnode;
                    goto action_crecur_loop_start;
                }
            }
        }
        if (!crecur && right_recur) {
#ifndef GILLS_FAST
            if (rlpnode1->parse_reclist)
                return;
#endif
            if (!rlpnode1->recact) {
                pnode = rlpnode1;
                continue;
            } else {
                action_ex(gills, rlpnode->up, rlpnode);
                pnode = rlpnode->up;
                free_parse_rule(gills, rlpnode);
                continue;
            }
        }
    }
}

void free_parsereclist_lnode (gills_context_t *gills, parse_node_t *pnode)
{
#ifndef GILLS_FAST
    parse_node_t *up_pnode, *uprecurpnode;
    list_t *parsereclist;
    list_t *llnode, *lnode;

    llnode = (list_t *)(pnode->parserec_llnode->data);
    llnode = list_del_node(gills, llnode, pnode->parserec_lnode);
    pnode->parserec_lnode = NULL;
    if (llnode) {
        pnode->parserec_llnode = NULL;
    }
    if (!llnode) {
        llnode = pnode->parserec_llnode;
        pnode->recup_pnode->parse_reclist = list_del_node(gills, 
                      pnode->recup_pnode->parse_reclist,
                      llnode);
        pnode->parserec_llnode = NULL;
    }
#endif
}

void free_parse_rule (gills_context_t *gills, parse_node_t *tkpnode)
{
#ifndef GILLS_FAST
    parse_node_t *pnode = tkpnode, *nxtrlpnode;
    int end = 0;

    while (1) {
        nxtrlpnode = pnode->rl_next;
        end = pnode->rule_end;
        free_pnode(gills, pnode);
        if (end)
            break;
        pnode = nxtrlpnode;
    }
#endif
}

void free_recur_list (gills_context_t *gills, parse_node_t *pnode)
{
#ifndef GILLS_FAST
    list_t *recurlist = pnode->recur_list;
    parse_node_t *recpnode;

    if (!recurlist)
        return;

    while (1) {
        recurlist = recurlist->next;
        if (recurlist == pnode->recur_list) {
            pnode->recur_list = NULL;
            break;
        }
        recpnode = (parse_node_t *)(recurlist->data);
        free_pnode(gills, recpnode);
    }
    pnode->recur_list = NULL;
#endif
}

void free_parse_recpnode (gills_context_t *gills,
                          parse_node_t *prevpnode,
                          int *recpnode_npass,
                          int *recpnode_pass, int updir)
{
#ifndef GILLS_FAST
    if (prevpnode->recur ||
        prevpnode->int_recur ||
        prevpnode->right_recur) {
        if (*recpnode_pass &&
            recpnode_npass &&
            !*recpnode_npass) {
            *recpnode_pass -= 1;
        }
        if (recpnode_npass)
            *recpnode_npass = 0;
        if (!prevpnode->int_recur &&
            !prevpnode->right_recur &&
            !prevpnode->crecur &&
             prevpnode->recur &&
             prevpnode->recur_list)
           free_recur_list(gills, prevpnode);
    }
#endif
}

void free_parse_nodes (gills_context_t *gills,
                       parse_node_t *tkpnode)
{
#ifndef GILLS_FAST
    parse_node_t *pnode = tkpnode, *uppnode, *recuppnode, *dnpnode, *uprecur_prevpnode, *prevpnode, *fpnode, *nextpnode, *fpuppnode;
    list_t *parsereclist, *lnode, *toplasttk_list, *recuruplist;
    int recpnode_pass = 0, recpnode_npass = 0, free_flag = 1, ret;
    int recfree_idx = 0, tk_num;
    int prev = 0, next = 0;
    int updir, lasttk_top_floating = 0, top_res, recur_up_top, topfreed = 0;

    if (pnode->token->token_num == gills->last_tk_num) {
        top_res = pnode->lasttk_toppnode->res;
        fpnode = pnode->lasttktotoprule_pnode; 
        lasttk_top_floating = pnode->lasttk_top_floating;
        pnode->lasttk_toppnode->topres = 0;
        toplasttk_list = pnode->lasttk_toppnode->top_lasttk_pnode_list;
        pnode->lasttk_toppnode->top_lasttk_pnode_list = 
            list_del_node(gills, toplasttk_list,
                          pnode->toplist_lnode);
        if (!pnode->lasttk_toppnode->top_lasttk_pnode_list &&
            !pnode->lasttk_toppnode->dn &&
            !pnode->lasttk_toppnode->dnres_pnode) {
            free_pnode(gills, pnode->lasttk_toppnode);
            gills->toppnode = NULL;
            gills->top_free = 1;
            free_pnode(gills, pnode);
            return;
        }
        free_pnode(gills, pnode);
        if (!top_res)
            return;
        if (!fpnode->token)
            return;
        pnode = fpnode;
        goto rlnextfreelabel;
    }

rlnextfreelabel: 
    nextpnode = pnode;
    fpnode = pnode;
    while (!nextpnode->rl_next->rule_start)
        nextpnode = nextpnode->rl_next;
    pnode = nextpnode;
    next = 1;
    if (lasttk_top_floating) {
        if (fpnode->recur)
            goto rldnfreelabel;
    }
    if (pnode->recur || pnode->right_recur ||
        pnode->int_recur) {
        if (!pnode->parse_reclist &&
                !pnode->recdn_pnode) {
            if (pnode->dnres_pnode || pnode->dn) {
                goto rldnfreelabel;
            } else {
                if (fpnode != pnode)
                    recpnode_npass = 1;
                goto rlprevfreelabel;
            }
        } else {
            recpnode_pass += 1;
            goto recurfreelabel;
        }
    }
    if (pnode->dnres_pnode || pnode->dn) 
        goto rldnfreelabel;
    else
        goto  rlprevfreelabel;
recurfreelabel:
    if (pnode->recdn_pnode) {
        fpnode = pnode;
        while (1) {
            if (!fpnode->recdn_pnode) {
                if (fpnode->parse_reclist)
                    return;
                break;
            }
            fpnode = fpnode->recdn_pnode;
        }
        pnode = fpnode;
        goto rlnextfreelabel;
    } else if (!pnode->recdn_pnode &&
        pnode->parse_reclist) {
        return;
    } else if (!pnode->recdn_pnode &&
        !pnode->parse_reclist) {
        if (pnode->dnres_pnode) {
            goto rldnfreelabel;
        } else {
            goto rlnextfreelabel;
        }
    }
crecurfreelabel:
    fpnode = pnode->recup_pnode;
    fpuppnode = pnode->up == pnode->recup_pnode ?
    pnode->up : NULL;
    if (pnode->parse_reclist ||
        pnode->recdn_pnode)
        return;
    if (fpnode->recdn_pnode &&
        fpnode->recdn_pnode == pnode)
        fpnode->recdn_pnode = NULL;
    free_pnode(gills, pnode);
    pnode = fpnode;
                
    if (recpnode_pass) {
        if (pnode->parse_reclist)
            return;
        if (pnode->recdn_pnode) {
            pnode = pnode->recdn_pnode;
            goto rlnextfreelabel;
        }
        if (fpuppnode && fpuppnode->dnres_pnode) {
            goto rldnfreelabel;
        } else {
            goto rlnextfreelabel;
        }
    } else {
        if (fpnode->parse_reclist || fpnode->recdn_pnode)
            return;
        while (1) {
            if (fpnode->crecur) {
                fpnode = fpnode->up;
            } else if (fpnode->right_recur) {
                  fpnode = fpnode->rl_next;
            } else if (fpnode->recur || fpnode->int_recur) {
                break;
            }
        }
        fpnode->recres = 1;
        return;
    }

rldnfreelabel:
    if (lasttk_top_floating) {
        if (!fpnode->recur) {
            fpnode = pnode->dnres_pnode ? pnode->dnres_pnode :
                                      pnode;
            pnode = fpnode;
            goto rlnextfreelabel;
        }

        if (fpnode->res && fpnode->dnres_pnode) {
            if (fpnode->recres) {
                pnode = fpnode->dnres_pnode;
                goto rlnextfreelabel;
            } else {
                pnode = fpnode->up;
                goto rlupresnulllabel;
            }
        }

        lasttk_top_floating = 0;
        if (fpnode->res &&
            !fpnode->dnres_pnode) {
            if (fpnode->recres) {
                if (fpnode->dn) {
                    pnode = fpnode->up;
                    goto rlupresnulllabel;
                } else {
                    goto rlnextfreelabel;
                }
            } else {
                pnode = fpnode->up;
                goto rlupresnulllabel;
            }
        }
    }
    if (!pnode->dnres_pnode && pnode->dn)
        return;
    if (!pnode->dnres_pnode && !pnode->dn)
        goto rlnextfreelabel;
    pnode = pnode->dnres_pnode;
    goto rlnextfreelabel;

rlprevfreelabel:
    prevpnode = pnode;
    if (prevpnode->token->token_num == gills->top_token->token_num) {
        if (!prevpnode->rule_start) {
            pnode = prevpnode->lasttktotoprule_pnode;
            fpnode = prevpnode->rl_prev;
            fpnode->rl_next = prevpnode->rl_next;
            prevpnode->rl_next->rl_prev = fpnode;
            free_pnode(gills, prevpnode);
            gills->toppnode = NULL;
            goto rldnfreelabel;
        } else {
            if (!prevpnode->top_lasttk_pnode_list) {
                free_pnode(gills, prevpnode);
                gills->toppnode = NULL;
                gills->top_free = 1;
                return;
            } else {
                return;
            }
        }
    }

    if (!prevpnode->rule_start) {
        updir = 0;
        fpnode = prevpnode->rl_prev;
    } else {
        updir = 1;
        fpnode = prevpnode->up;
    }

    if (fpnode->crecur) {
        free_pnode(gills, prevpnode);
        fpnode->rl_next = NULL;
        pnode = fpnode;
        goto crecurfreelabel;
    }    
    if (!prevpnode->dn) {
        if (!prevpnode->rule_start &&
            (fpnode->recur || fpnode->int_recur ||
             fpnode->right_recur)) {
             if (!fpnode->parse_reclist &&
                 !fpnode->recdn_pnode)
                recpnode_npass = 1;
        }
        if (!updir) {
            fpnode->rl_next = prevpnode->rl_next;
            prevpnode->rl_next->rl_prev = fpnode;
            free_parse_recpnode(gills, prevpnode, &recpnode_npass,
                                &recpnode_pass, updir);
            free_pnode(gills, prevpnode);
            pnode = fpnode;
        }
        if (updir)
            goto rlupfreelabel;
        else
            goto rlnextfreelabel;

    }

rlupfreelabel:
    uppnode = pnode->up;
    if (uppnode &&
       (uppnode->recur ||
        uppnode->right_recur ||
        uppnode->int_recur)) {
        if (!uppnode->parse_reclist &&
            !uppnode->recdn_pnode)
            recpnode_npass = 1;
    }
         
    if (uppnode->res || uppnode->dnres_pnode) {
        if (pnode == uppnode->dnres_pnode) {
            uppnode->dnres_pnode = NULL;
            pnode->res = 0;
            free_parse_recpnode(gills, pnode,
            &recpnode_npass,
            &recpnode_pass, updir);
            free_pnode(gills, pnode);
            if (uppnode->dn) {
                pnode = uppnode;
rlupresnulllabel:
                fpnode = pnode;
                free_flag = 1;
                while (1) {
                    fpuppnode = fpnode->up;
                    if (free_flag) {
                        if ((fpnode->dn) ||
                            (fpnode->dnres_pnode) ||
                            (!fpnode->dnres_pnode && fpnode->act))
                            free_flag = 0;
                    }
                    if (!fpnode->rule_start || !fpnode->rule_end) {
                        fpnode->res = fpnode->recres = 0;
                        if (fpnode->dn)
                            return;
                        else
                            break;
                    } else if (fpnode->recur &&
                        fpnode->token->crecur &&
                        !fpnode->act &&
                        !fpnode->dnres_pnode) {
                        fpnode->res = 0;
                    } else if (fpnode->recur && 
                        !fpnode->token->crecur &&
                        !fpnode->dnres_pnode) {
                        fpnode->res = 0;
                    } else {
                        fpnode->res = 0;

                    }
                    if (!free_flag && fpnode->dnres_pnode) {
                        fpnode->dn = list_add(gills, fpnode->dn,
                                     fpnode->dnres_pnode, &lnode);
                        fpnode->dnres_pnode->updn_lnode = lnode;
                        fpnode->dnres_pnode = NULL;
                    }
                    if (fpnode->token->token_num == 0) {
                        if (!free_flag) {
                            gills->topreach = 0;
                            return;
                        } else {
                            topfreed = 1;
                        }
                    }
                    free_parse_recpnode(gills, fpnode,
                        &recpnode_npass,
                        &recpnode_pass, updir);
                    if (free_flag)
                        free_pnode(gills, pnode);
                    fpnode = fpuppnode; // fpnode->up;
                    if (topfreed) 
                        return;
                }
                return;
            } else {
                pnode = uppnode;
                goto rlnextfreelabel;
            }
        } else {
            uppnode->dn = list_del_node(gills, uppnode->dn, pnode->updn_lnode);
            free_parse_recpnode(gills, pnode,
            &recpnode_npass,
            &recpnode_pass, updir);
            free_pnode(gills, pnode);
            return;
        }
    } else {
        uppnode->dn = list_del_node(gills, uppnode->dn, pnode->updn_lnode);
        free_parse_recpnode(gills, pnode,
                        &recpnode_npass,
                        &recpnode_pass, updir);
        free_pnode(gills, pnode);
        if (uppnode->dn) {
            return;
        } else {
            pnode = uppnode;
            goto rlnextfreelabel;
        }
    }
#endif
    return;
}

int reduce_parse (gills_context_t *gills, parse_node_t *tkpnode)
{
    int ret;
    parse_node_t *retpnode, *lastrespnode;

    ret = generate_parse(gills, tkpnode, tkpnode->token, tkpnode->rule_node, 0, NULL, NULL, NULL);
    if (ret == CREC_AMBIGUOUS) {
        exit_parse(gills, -1);
    }
    if (ret == UPPNODE_AMBIGUOUS) {
        exit_parse(gills, -1);
    }
    if (ret) {
        exit_parse(gills, -1);
    }
    return 0;
}

int parse (gills_context_t *gills, token_node_t *starttoken)
{
    int tk, tkread = 0, ret, ambiguity, top_reached, i, nonempty_tkcount, loop, prevop_res;
    parse_node_t *pnode, *tkpnode, *tkuppnode, *ltoppnode, *retpnode, *listpnode, *crecpnode, *crecuppnode;
    token_node_t *tknode;
    list_t *tklist, *tknelist, *listnode, *prevlistnode, *toplasttk_list, *listmesh_node, *nextop_list;
    GILLSYYSTYPE *valptr;
    struct timespec timespecstart, timespecend;

reparse_genlabel:
    gills->retfree = 0;
    ret = generate_parse(gills, NULL, starttoken, NULL, 0, NULL, &retpnode, NULL);
    gills->toppnode = retpnode;
    if (tkread) {
        tkread = 0;
        goto reparsenontklabel;
    }
reparselabel:
    tk = get_token(gills, gills->scan_active_idx);
    if (tk == GILLS_SCANNER_ERROR) {
        printf("scanner error ...exiting\n");
        exit(-1);
    }

reparsenontklabel:
    gills->retfree = 0;

#ifndef GILLS_MEM_TKSTACK_UNLIM
 //   if (gills->tkstack_idx) {
  //      gills->tkstack_idx += 1;
 //   }
#else
    gills->prevop_tkstack_listmesh_list = gills->nextop_tkstack_listmesh_list;
    gills->nextop_tkstack_listmesh_list = NULL;
#endif
    gills->prevop_tkstack_update = 0;

    gills->top_free = 0;
#ifndef GILLS_MEM_TKSTACK_UNLIM
    tklist = gills->tkstack[gills->tkstack_idx];
    gills->prevop_tkstack_lnode = gills->tkstack[gills->tkstack_idx] ?
                                  gills->tkstack[gills->tkstack_idx]->next :
                                  NULL;
#else
    listmesh_node = gills->prevop_tkstack_listmesh_list;
    gills->prevop_tkstack_lnode = listmesh_node ? listmesh_node->next : NULL;
#endif
    while (1) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == gills->tkstack[gills->tkstack_idx])
            break;
#else
        if (!gills->prevop_tkstack_lnode)
            break;
        if (gills->prevop_tkstack_lnode == listmesh_node)
            break;
#endif
        pnode = (parse_node_t *)(gills->prevop_tkstack_lnode->data);
        if (((pnode->token->token_num == gills->empty_tk_num)) ||
            pnode->token->token_num == tk) {
            gills->prevop_tkstack_lnode = gills->prevop_tkstack_lnode->next;
            continue;
        }
        if (pnode->token->token_num != tk) {
            free_parse_nodes(gills, pnode);
#ifdef GILLS_FAST
            listnode = gills->prevop_tkstack_lnode->next;
            gills->tkstack[gills->tkstack_idx] = list_del_node(gills, gills->tkstack[gills->tkstack_idx], pnode->tkstack_lnode);
            gills->prevop_tkstack_lnode = listnode;
#endif
            if (gills->top_free) {
                if (tk != -1) {
                    printf("parse failed with input unexpected token for expected tokens ...exiting\n");
                } else {
                    printf("parse failed with unexpected end of input tokens(EOF) ...exiting\n");
                }
                exit_parse(gills, -1);
            }
        }
    }

    if (!gills->topreach &&
#ifndef GILLS_MEM_TKSTACK_UNLIM
        !gills->tkstack[gills->tkstack_idx]
#else
        !gills->prevop_tkstack_listmesh_list
#endif
      ) {
        printf("unexpected token %d parse terminated ...exiting\n", tk);
        exit_parse(gills, -1);
    }

    gills->tkstack_idx += 1;

    gills->prevop_tkstack_update = 0;
#ifndef GILLS_MEM_TKSTACK_UNLIM
    tklist = gills->tkstack[gills->tkstack_idx];
    gills->prevop_tkstack_lnode = gills->tkstack[gills->tkstack_idx - 1] ?
                                  gills->tkstack[gills->tkstack_idx - 1]->next :
                                  NULL;
#else
    listmesh_node = gills->prevop_tkstack_listmesh_list;
    gills->prevop_tkstack_lnode = listmesh_node ? listmesh_node->next : NULL;
#endif
    while (1) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == gills->tkstack[gills->tkstack_idx - 1])
            break;
#else
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == listmesh_node)
            break;
#endif
        tkpnode = (parse_node_t *)(gills->prevop_tkstack_lnode->data);
        if ((tkpnode->token->token_num == tk)  &&
             (tkpnode->token->token_num != gills->empty_tk_num)) {
            tkpnode->res = 1;
            if (tkpnode->token->token_num == gills->last_tk_num) {
                if (tkpnode->lasttk_toppnode->topres) {
                    printf("ambiguous at top node %s %d ...exiting\n",
                           tkpnode->lasttk_toppnode->token->token_name,
                           tkpnode->lasttk_toppnode->token->token_num);
                    exit_parse(gills, -1);
                }
                tkpnode->res = 1;
                tkpnode->lasttk_toppnode->res =
                tkpnode->lasttk_toppnode->topres = 1;
                tkpnode->lasttk_toppnode->topres_top_or_lasttk = 2;
                ltoppnode = tkpnode->lasttk_toppnode;
                if (!ltoppnode->res) {
                    pnode->lasttk_toppnode->top_lasttk_pnode_list = 
                              list_del_node(gills, toplasttk_list,
                              pnode->updn_lnode);
                }

                while (1) {
                    if (ltoppnode->rule_start)
                        break;
                    ltoppnode = ltoppnode->rl_prev;
                    if (ltoppnode->topres) {
                        printf("ambiguous top token %s %d resolved ...exiting\n",
                               ltoppnode->token->token_name,
                               ltoppnode->token->token_num);
                        exit_parse(gills, -1);
                    } else {
                        ltoppnode->topres = 1;
                        ltoppnode->topres_top_or_lasttk = 1;
                        gills->topreach = 1;
                    }
                }
            } else {
#ifdef TOKEN_MEM_INTERNAL
                valptr = (GILLSYYSTYPE *)calloc(1, sizeof(GILLSYYSTYPE));
                if (!valptr) {
                    printf("valptr calloc failed ...exiting\n");
                    exit(-1);
                }
                strcpy(valptr->str, ((YYSTYPE *)(gills->scan_funcs_val[gills->scan_active_idx]))->str);

                tkpnode->val = (void *)valptr;
#else
                tkpnode->val = gills->scan_funcs_val[gills->scan_active_idx];
#endif 
                ret = reduce_parse(gills, tkpnode);
                if (ret) {
                    printf("error ...exiting\n");
                    exit_parse(gills, -1);
                }
            }
        }
        if (gills->prevop_tkstack_update) {
            gills->prevop_tkstack_update = 0;
        } else {
            gills->prevop_tkstack_lnode = gills->prevop_tkstack_lnode->next;
        }
    }

    if (gills->tkstack_idx == 1) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
        gills->prevact_tkstack_idx = 0;
        gills->prevact_tkstack_list = gills->tkstack[0];
#else
         gills->prevact_tkstack_llist = gills->tkstack_listmesh ?
                                        gills->tkstack_listmesh->next :
                                        NULL;
        gills->prevact_tkstack_list = gills->prevact_tkstack_llist ?
                                      (list_t *)(gills->prevact_tkstack_llist->data) :
                                      NULL;
#endif
    }

#ifndef GILLS_MEM_TKSTACK_UNLIM
    nextop_list = gills->tkstack[gills->tkstack_idx];
#else
    nextop_list = gills->nextop_tkstack_listmesh_list;
#endif

    prevop_res = 1;
    pnode = NULL;
    if (gills->prevop_tkstack_lnode) {
        listnode = gills->prevop_tkstack_lnode->next;
        while (listnode && listnode != gills->prevop_tkstack_lnode) {
            tkpnode = (parse_node_t *)(listnode->data);
            if (tkpnode->token->token_num != gills->empty_tk_num) {
                if (!pnode) {
                    pnode = tkpnode;
                } else {
                    prevop_res = 0;
                    break;
                }
           }
           listnode = listnode->next;
        }

        if (prevop_res) {
            tkpnode = pnode;
            pnode = gills->prevact_tkstack_list ?
                    (parse_node_t *)(gills->prevact_tkstack_list->next->data) : NULL;
            if (tkpnode->res) {
                if (tkpnode->token->token_num == gills->last_tk_num) {
                    if (!nextop_list) {
#ifndef GILLS_MEM_TKSTACK_UNLIM

#ifndef REDUCE_ACTION_INLINE
                        gills->action = 1;
                        action_ex_inline(gills, (parse_node_t *)(gills->tkstack[0]->next->data), NULL);
                        gills->action = 0;
                        
#else
                        gills->action = 1;
                        action_ex_inline(gills, pnode, NULL);
                        gills->action = 0;
#endif

#else
#ifndef REDUCE_ACTION_INLINE
                        gills->action = 1;
                        action_ex_inline(gills,
                          (parse_node_t *)(((list_t *)
                           (gills->tkstack_listmesh->next->data))->next->data),
                           NULL);
                        gills->action = 0;
#else
                        gills->action = 1;
                        action_ex_inline(gills, pnode, NULL);
                        gills->action = 0;
#endif
#endif
                        return 0;
                    } else {
                        printf("top token (EOF) resolved and ambiguous ...exiting\n");
                        exit_parse(gills, -1);
                    }
                } else {
                    if (!nextop_list) {
                        printf("unexpected end of parse tree ...exiting\n");
                        exit_parse(gills, -1);
                    } else {
#ifdef REDUCE_ACTION_INLINE
                        gills->action = 1;
                        action_ex_inline(gills, pnode, NULL);
                        gills->action = 0;
                        goto prereparselabel;
#else
                        goto prereparselabel;
#endif
                    }
                }
            }
        } else {
            goto prereparselabel;
        }
    } else {
        printf("parse failed with input unexpected token for expected tokens ...exiting\n");
        exit_parse(gills, -1);
    }

prereparselabel:
    gills->prevop_tkstack_lnode = NULL;
    gills->prevop_tkstack_update = 0;
    goto reparselabel;
topreachlabel:
//#define REDUCE_ACTION_INLINE 1
    if (top_reached) {
    }

check_reparse:
    top_reached = (gills->topreach && gills->prevop_tkstack_list_res == 1);
    if (top_reached && tk != EOF) {
        printf("parse failed with input unexpected token for expected tokens (EOF) ...exiting\n");
        exit_parse(gills, -1);
    }
    if ((!top_reached ||
         (gills->toppnode && !gills->toppnode->res)) && tk == EOF) {
                 printf("parse failed with unexpected end of input tokens(EOF) ...exiting\n");
                        exit_parse(gills, -1);
    }
    if (!top_reached && gills->retfree) {
        printf("unexpected token %d ...exiting\n", tk);
        return UNEXPECTED_TOKEN;
    }

    if (top_reached && tk != EOF) {
        gills->topreach = 0;
 //       gills->topres = 0;
        gills->toppnode = NULL;
        gills->tkstack_idx = 0;
 //       gills->pnodemem_idx = 0;
 //       gills->pnodeptrs_start = 0;
 //       gills->pnodeptrs_end = 0;
        goto reparse_genlabel;
    }

    if (!top_reached) {
        goto reparselabel;
    }
//    if (gills->topreach && !ambiguity && tk != EOF)
//        goto reparse;
    if (gills->topreach && tk == EOF)
        return 0;
}

