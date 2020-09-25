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
// void free_uprecurlist(parse_node_t *pnode);
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
// void set_uppnode_recur_list(list_t *rec_list, list_t *up_list, int recur_up_top);
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
/*
void mem_pool_alloc (gills_context_t *gills, list_t **pool_list,
                int segment_size)
{
    list_t *listnode = *pool_list, *retlistnode;

    memsegment = (void 
    listnode = list_add(listnode, memsegment, &retlistnode);

    
*/
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

 //   if (fpnode->rl_next)
 //       return;

   while (1) {
       if (fpnode->rule_start)
           return;
 //      fpnode = fpnode->rl_prev;
 //      if (!fpnode)
 //          return;
       if (fpnode->rule_node->right_recur || fpnode->rule_node->int_recur) {
           if (recpnode) {
               printf("invalid recursvie syntax at rule ");
//               print_rule(fpnode->rule);
               printf("   ...exiting\n");
               exit(-1);
           }
           recpnode = fpnode;
 //          fpnode = fpnode->rl_prev;
 //          continue;
       } else if (!fpnode->nonterm) {
           if (recpnode) {
               recpnode->assoc = fpnode->assoc;
               recpnode->prec = fpnode->prec;
           }
           recpnode = NULL;
 //          fpnode = fpnode->rl_prev;
 //          if (!fpnode->rl_prev)
 //              return;
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
//    fpnode = fpnode->rl_prev;
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
/*
        if (fpnode->int_recur)
            return;
*/
        if (fpnode->recur) {
            continue;
        }
        if (!fpnode->nonterm) {
            pnode->assoc = fpnode->assoc;
            pnode->prec = fpnode->prec;
//            return;
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
//              uptknode->rules[m] = rules[m];
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
//                      tknode->token_num = token_num_max;
                      tknode->rules_num = 0;
                      gills->token_list = list_add(gills, gills->token_list, tknode, NULL);
                      gills->term_token_list = list_add(gills, gills->term_token_list, tknode, NULL);
                 }
/*
                      printf("unknown token %s ...exiting\n", token_name);
                      return UNKNOWN_TOKEN;
*/
//                  rlnode = (rule_node_t *)calloc(1, sizeof(rule_node_t));
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
//                  uptknode->rules[m].rule_nodes = list_add(gills, uptknode->rules[m].rule_nodes, rlnode, &lnode);
//                  rlnode->lnode = lnode;
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
//             if (gills->top_token)
//                 *toptoken = tknode;
               gills->top_token = tknode;
//               gills->top_token->rule_node = &gills->top_rule_node;
             break; //return 0;
          }
      }

      gills->tokens_num = tokens_num;
      return 0;
}

parse_node_t* get_pnode (gills_context_t *gills)
{
    parse_node_t *pnode;

    if (gills->pnodemem_idx != gills->pnode_max_num) {
        pnode = &gills->pnodemem[gills->pnodemem_idx];
        gills->pnodemem_idx += 1;
 //       memset(pnode, 0, sizeof(parse_node_t));
        return pnode;
    } else if (gills->pnodeptrs_start != gills->pnodeptrs_end) {
        pnode = gills->pnodeptrs[gills->pnodeptrs_start];
        gills->pnodeptrs_start += 1;
//        memset(pnode, 0, sizeof(parse_node_t));
        return pnode;
    } else {
        printf("parse memory exhausted ...exiting\n");
//#ifdef MEM_NULL_EXIT
        exit_parse(gills, -1);
//#else
//        return NULL;
//#endif
    }
}

#if 0
void free_uprecurlist (parse_node_t *pnode)
{
    list_t *upreclist = pnode->up_recur_list, *parsereclist, *parsereclist1, *recuplist;
    parse_node_t *recpnode, *recuppnode;

    if (!upreclist)
        return;

    recpnode = (parse_node_t *)(upreclist->next->data);
    parsereclist = pnode->up_recur_list;
//    recuplist = recpnode->recur_up_list;
    pnode->up_recur_list = NULL;
//    free_list(recuplist);
//    pnode->up_recur_list = NULL;
    pnode->recur_up_list = NULL;
    if (!parsereclist)
        return;
    recpnode = (parse_node_t *)(parsereclist->next->data);
    recuppnode = recpnode->recup_pnode;
    if (recuppnode->recdn_pnode) {
        recuppnode->recdn_pnode->recur_up_list = NULL;
    }
    parsereclist1 = parsereclist;
    while (1) {
        parsereclist = parsereclist->next;
        if (parsereclist == parsereclist1) {
//            free_list(parsereclist1);
            pnode->up_recur_list = NULL;
            break;
        }
        recpnode = (parse_node_t *)(parsereclist->data);
        recpnode->recur_up_list = NULL;
    }
}
#endif

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
/*
            upparsereclist = pnode->recup_pnode->parse_reclist;
            pnode->recup_pnode->parse_reclist = 
                list_del_node(gills, pnode->recup_pnode->
                parse_reclist, pnode->parserec_lnode);
*/
        }
        pnode->recur_up_list = NULL;
/*
        if (recuplist) {
            uprecpnode = (parse_node_t *)(recuplist->next->data);

            free_list(recuplist);
            pnode->recur_up_list = NULL;
        }
*/
     }
#if 0
     if (pnode->up_recur_list /* || pnode->recur_up_list */ ) {
         free_uprecurlist(pnode);
     }
#endif
/*
        parsereclist = pnode->upparse_rec_list;
        uppreclist_lnode = pnode->uppreclist_lnode;
        parsereclist1 = list_del_node(gills, parsereclist, uppreclist_lnode);
        if (parsereclist1 && parsereclist1 !=
            parsereclist->next) {
            pnode->up->parse_reclist = list_del_node(gills, pnode->up->parse_reclist, parsereclist);
            pnode->up->parse_reclist = list_add(gills, pnode->up->parse_reclist, parsereclist1);
            if (pnode->recur_up_list) {
                recuruppnode = (parse_node_t *)pnode->recur_up_list;
                recuruppnode->up_recur_list = parsereclist1;
            }
        } else if (!parsereclist1) {
            pnode->up->parse_reclist = list_del_node(gills, pnode->up->parse_reclist, parsereclist);
            if (pnode->recur_up_list) {
                recuruppnode = (parse_node_t *)pnode->recur_up_list;
                recuruppnode->up_recur_list = NULL;
            }
        }
    }
*/

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
                if (!gills->prevop_tkstack_lnode->data) {
                    if (gills->prevop_tkstack_lnode->next->next ==
                        gills->prevop_tkstack_lnode) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
                        gills->tkstack[pnode->tkstackidx] = NULL;
#else
                        gills->prevop_tkstack_listmesh_list = NULL;
#endif
                    }
                    gills->prevop_tkstack_lnode = NULL;
                }
                gills->prevop_tkstack_update = 1;
            }
            if (/* gills->action && */ gills->prevact_tkstack_list) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
                if (gills->prevact_tkstack_list == 
                    gills->tkstack[pnode->tkstackidx]) {
                    if (pnode->tkstack_lnode ==
                        pnode->tkstack_lnode->next->next) {
 //                       gills->tkstack[pnode->tkstackidx]) {
/*
                        if ((!gills->action &&
                             gills->tkstack_idx == (gills->prevact_tkstack_idx + 1)) ||
                            (gills->action &&
                             gills->tkstack_idx < (gills->prevact_tkstack_idx + 1))) {
                            gills->prevact_tkstack_list = NULL;
                            gills->prevact_tkstack_idx = -1;
                        } else {
*/
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
  //                      }
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
/*
        if (pnode->tkstack_llnode && gills->prevop_tkstack_lnode) {
            if (gills->prevop_tkstack_lnode == pnode->tkstack_lnode) {
                gills->prevop_tkstack_lnode = pnode->tkstack_lnode->next;
                gills->prevop_tkstack_update = 1;
            }
        } else {
            gills->prevop_tkstack_lnode = NULL;
            if (!pnode->tkstack_llnode) {
                gills->prevop_tkstack_list_res = -1;
            }
        }
*/
#endif
#ifdef TOKEN_MEM_INTERNAL
//        free(pnode->val);
#endif
    }
/*
                            if (pnode->up_recur_list) {
                                uprecurlist = pnode->up_recur_list;
                                while (1) {
                                    uprecurlist = uprecurlist->next;
                                    if (uprecurlist == pnode->up_recur_list)
                                        break;

                                    recurpnode = (parse_node_t *)uprecurlist->data;
                                    free_list(recurpnode->recur_up_list);
                                    recurpnode->recur_up_list = NULL;
                                }
                            }
                        }
*/
    gills->pnodeptrs[gills->pnodeptrs_end] = pnode;
    memset(pnode, 0, sizeof(parse_node_t)); 
    gills->pnodeptrs_end += 1;
#endif
}

void reset_pnode_mem (gills_context_t *gills)
{
    gills->pnodemem_idx = 0;
    gills->pnodeptrs_start = gills->pnodeptrs_end = 0;
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
//    nlist = &(gills->listnodearr[gills->listnodearr_idx]);
//    gills->listnodearr_idx += 1;
    if (!listhead) {
//        dlist = calloc(1, sizeof(list_t));
        listhead = &(gills->listnodearr[gills->listnodearr_idx]);
        gills->listnodearr_idx += 1;
//        nlist->next = dlist;
        listhead->next = listhead->prev = listhead;
/*
        dlist->data = data;
        dlist->prev = nlist;
        dlist->next = nlist;
        nlist->prev = dlist;
        nlist->next = dlist;
        if (lnode)
            *lnode = dlist;

        list = l
*/
//        return nlist;
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
/*
    if (node == list) {
        if (list->next == list)
            retnode = NULL;
        else
            retnode = list->next;
        free(list);
        return retnode;
    } else {
*/
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
//    }
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
 //   if (!*list)
 //       *list = listptr;
 //   if (!llistptr) {
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
    gills->pnode_max_num = PNODE_DEF_NUM;
}

void init_gills_params (gills_context_t *gills,
                       int init_pstack_max_num,
//                       int init_pstackidx_max_num,
                       int init_tkstack_max_num,
                       int init_listnodearr_max_num,
                       int init_scan_funcs_max_num,
                       int init_pnode_max_num,
                       int init_action_params_max_num,
                       int init_last_tk_num)
{
    gills->pstack_max_num = init_pstack_max_num;
//    gills->pstackidx_max_num = init_pstackidx_max_num;
    gills->tkstack_max_num = init_tkstack_max_num;
    gills->listnodearr_max_num = init_listnodearr_max_num;
    gills->scan_funcs_max_num = init_scan_funcs_max_num;
    gills->pnode_max_num = init_pnode_max_num;
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
    gills->pnodemem_idx = 0;
    gills->pnodeptrs_start = 0;
    gills->pnodeptrs_end = 0;
    gills->listnodearr = (list_t *)calloc(gills->listnodearr_max_num, sizeof(list_t));
    gills->pstack = (parse_node_t **)calloc(gills->pstack_max_num, sizeof(parse_node_t *));
//    gills->pnodemem = (parse_node_t *)calloc(gills->pnode_max_num, sizeof(parse_node_t));
//    gills->listnodearr = (list_t *)calloc(gills->listnodearr_max_num, sizeof(list_t));
    gills->pnodeptrs = (parse_node_t **)calloc(gills->pnode_max_num, sizeof(parse_node_t *));
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
    gills->pnodemem = (parse_node_t *)calloc(gills->pnode_max_num, sizeof(parse_node_t));
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
    free(gills->pnodeptrs);
    free(gills->pstack);
//    free(gills->pstackidx_stack);
#ifndef GILLS_MEM_TKSTACK_UNLIM
    free(gills->tkstack);
#endif
    free(gills->scan_funcs);
    free(gills->scan_funcs_val);
    free(gills->scan_val_size);
    free(gills->pnodemem);
    free(gills->listnodearr);
    free(gills);
}

#if 0
void init_composer_gills(gills_context_t *gills)
{
 //   init_gills_param(
    init_gills_scan_func(gills, &prepyylex, &prepval, sizeof(prepval));
 //   init_gills_scan_func(gills, &makeyylex, &makeval, sizeof(makeval));
}
#endif

#if 0
    scan_funcs[PREP_SCAN_FUNC_IDX] = prepyylex;
//    scan_funcs[MAKE_SCAN_FUNC_IDX] = makeyylex;
    scan_func_val[PREP_SCAN_FUNC_IDX] = (void **)&prepval;
    scan_func_val[MAKE_SCAN_FUNC_IDX] = (void **)&makeval;
    scan_val_size[PREP_SCAN_FUNC_IDX] = sizeof(prepval);
    scan_val_size[MAKE_SCAN_FUNC_IDX] = sizeof(makeval);
#endif

void exit_parse (gills_context_t *gills, int exit_ret)
{
    parse_node_t *listpnode;
    list_t *lnode, *llnode;
    int i;

    for (i = 0; i < gills->tkstack_idx; i++) {
         if (!gills->tkstack[i])
             continue;
         gills->prevop_tkstack_lnode = gills->tkstack[i]->next;
         gills->prevop_tkstack_update = 0;
         while (1) {
//             gills->prevop_tkstack_lnode = llnode;
             if (!gills->tkstack[i])
                 break;
/*
             llnode = llnode->next;
             listpnode = (parse_node_t *)(llnode->data);
*/
             listpnode = (parse_node_t *)(gills->prevop_tkstack_lnode);
             free_parse_nodes(gills, listpnode);
             if (gills->prevop_tkstack_update) {
                 gills->prevop_tkstack_update = 0;
             } else {
                 gills->prevop_tkstack_lnode = gills->prevop_tkstack_lnode->next;
             }
         }
     }
    gills->prevop_tkstack_lnode = NULL;
    gills->prevop_tkstack_update = 0;
    if (gills->toppnode)
        free_parse_nodes(gills, gills->toppnode);
    printf("pnodemem_idx %d\npnodeptrs_end %d \n", gills->pnodemem_idx, gills->pnodeptrs_end);
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
    int rnum = 0, upflag = 0, updir = 0, toprecur;
    rule_t *rules, *rule;
    rule_node_t *rlnode, *nxtrlnode = NULL;
    list_t *lnode, *up_recur_list, *up_precur_list, *reclist, *reclist1, *parselistnode, *listnode;
    int i, recur, ret = 0, top_reached = 0, fl = 1, recur_up_top;

//    pnode = (parse_node_t *)calloc(1, sizeof(parse_node_t));
    fpnode = NULL;
    pnode = start_pnode;
/*
    if (start_pnode && start_pnode->res && start_pnode->crecur) {
        fpnode = NULL;
        pnode = start_pnode;
    } else */
/*
    if (pnode && pnode->crecur && floating) {
        pnode = pnode->up;
    }
*/
    if (start_pnode) { /* && */
/*
        ((start_pnode->res && !start_pnode->crecur) ||
         (!start_pnode->res && start_pnode->crecur *//* && !start_pnode->dn */ /*) || */
         /* (start_pnode->res && start_pnode->crecur *//* && start_pnode->dn *//*))) { */

//        pnode = start_pnode;
/*
        if (pnode->crecur) {
            pnode = pnode->up;
            if (pnode->right_recur) {
                fpnode = pnode;
                while (fpnode->rl_prev)
                    fpnode = fpnode->rl_prev;
                if (fpnode->crecur)
                    fpnode = fpnode->up;
                pnode = fpnode;
            } else 
                break;
        }
*/
/*        while (1)  */
            fpnode = pnode;
            nxtrlnode = NULL;
            toprecur = 0;
            while (1 /* fpnode->rl_prev */) {
/*                if (fpnode->up_precur_list) */
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
//                fpnode = fpnode->rl_prev;
        rlnode = pnode->rule_node;
//        if (rlnode->next != (rule_node_t *)(fpnode->lnode->data)) {
#if 0
        if (updir && /* !floating && */ pnode->token->crecur &&
           (pnode->recur || pnode->int_recur /* ||
            pnode->right_recur */)) {
            updir = 0;
            break;
        }
#endif
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
#if 0
            while (fpnode->rl_prev) {
                 if (fpnode->recurup) {
                     if (fpnode->rl_prev &&
                         fpnode->rl_prev->rl_next &&
                         fpnode->rl_prev->rl_next != fpnode) {
                         printf("ambiguity ...exiting\n");
                         exit_parse(gills, -1);
                     }
 //                    uprecpnode = fpnode->rl_prev;
 //                    up_recur_list = uprecpnode->up_recur_list;
                     up_recur_list = fpnode->up_recur_list;
                     if (up_recur_list) {
                     recuppnode = ((parse_node_t *)
                                  (up_recur_list->next->data))->recup_pnode;
                     if ((recuppnode->recdn_pnode) /* &&
                         (recuppnode->recdn_pnode->parserec_llnode ==
                         up_recur_list) */ ) {
                         printf("ambiguity rule node "
                                "%s and %s ...exiting\n",
                                 recuppnode->token->token_name,
                                 uprecpnode->token->token_name);
                         exit_parse(gills, -1);
                     }
                     } // {
                         if (!fpnode->rl_prev->rl_next) {
                         fpnode->rl_prev->rec_prevup_list = list_del_node(gills, 
                                          fpnode->rl_prev->rec_prevup_list,
                                          fpnode->recprevup_lnode);
                        fpnode->rl_prev->rl_next = fpnode;
                        }
 //                    }
                 }    
                fpnode = fpnode->rl_prev;
            }
#endif
#if 0
        if (rlnode->num != rlnode->rule->rule_nodes_num - 1) {

            nxtrlnode = &(rlnode->rule->rule_nodes[rlnode->num + 1]);
            tnode = nxtrlnode->token;
            break;
        } else {
            fpnode1 = fpnode;

            rule = fpnode->rule_node->rule;
            while (fpnode->rl_prev) {
                 if (fpnode->recurup) {
                     if (!fpnode->rl_prev) {
                         if (fpnode->up->dn && fpnode->up->dn != fpnode) {
                             printf("ambiguity ...exiting\n");
                             exit_parse(gills, -1);
                         } else if (!fpnode->up->dn) {
                               break;
//                             goto upreducelabel;
                         }
                     } else if (fpnode->rl_prev) {
                         if (fpnode->rl_prev->rl_next &&
                             fpnode->rl_prev->rl_next != fpnode) {
                             printf("ambiguity ...exiting\n");
                             exit_parse(gills, -1);
                         } else if (!fpnode->rl_prev->res) {
                             
                     }
 //                    uprecpnode = fpnode->rl_prev;
 //                    up_recur_list = uprecpnode->up_recur_list;
                     up_recur_list = fpnode->up_recur_list;
                     if (up_recur_list) {
                     recuppnode = ((parse_node_t *)
                                  (up_recur_list->next->data))->recup_pnode;
                     if ((recuppnode->recdn_pnode) /* &&
                         (recuppnode->recdn_pnode->parserec_llnode ==
                         up_recur_list) */ ) {
                         printf("ambiguity rule node "
                                "%s and %s ...exiting\n",
                                 recuppnode->token->token_name,
                                 uprecpnode->token->token_name);
                         exit_parse(gills, -1);
                     }
                     } // {
                         if (!fpnode->rl_prev->rl_next) {
                         fpnode->rl_prev->rec_prevup_list = list_del_node(gills, 
                                          fpnode->rl_prev->rec_prevup_list,
                                          fpnode->recprevup_lnode);
                        fpnode->rl_prev->rl_next = fpnode;
                        }
 //                    }
                 }    
                fpnode = fpnode->rl_prev;
            }
#endif
/*
           if (rule->int_recur) {
               action_ex(fpnode->up
           }
*/

/*
           if (rule->prec && rule->assoc) {
               if (rule->crecur && rule->right_recur) {
                   if (rule->assoc == LEFT_ASSOC) {
                       fpnode->recres = 1;
                       pnode = fpnode;
                       break;
                   } else if (rule->assoc == RIGHT_ASSOC) {
                       pnode = fpnode1;
                       break;
                   }
               }

               if (!rule->crecur && rule->right_recur) {
                   
           if (!rule->crecur &&
                rule->right_recur) {
                pnode = fpnode1;
                pnode->res = 1;
                break;
           }
           if (fpnode->crecur &&
               fpnode->recup_pnode == fpnode->up &&
               fpnode->up->right_recur) {
               uppnode = fpnode->up;
               if (uppnode->assoc == fpnode->assoc &&
                   uppnode->prec == f 1 + 2 - 3 - 4
           if (rule->crecur && rule->right_recur) {
*/               
/*
           if (rule->crecur &&
               rule->right_recur) {
///*
               if (fpnode1->assoc == LEFT_ASSOC) {
                   pnode = fpnode;
                   goto crecureducelabel;
                   break;
               } else {
//
                   if (fpnode->recur_up_list) {
                       recuruppnode = ((parse_node_t *)(
                                      fpnode->recur_up_list->next->data));
                       if (recuruppnode->res) {
                           printf(" ambiguity rule node "
                                  "%s %s ...exiting\n",
                                  fpnode->token->token_name,
                                  recuruppnode->token->token_name);
                           exit_parse(gills, -1);
                       }
                   }
                   pnode = fpnode1;
                   fpnode->recdn_pnode = NULL;
                   fpnode->parse_reclist = NULL;
                   fpnode1->res = 1;
                   break;
//               }
           }
*/
//        if (floating && rule == start_pnode->rule) {
            
        if ( /* updir && !floating  && */ fpnode->rule->token->crecur &&
           (!fpnode->crecur && fpnode1->right_recur)) {
            updir = 0;
            pnode = fpnode1;
            goto crecurgenlabel;
//            break;
        }

        if (/* updir && !floating && */ fpnode->rule->token->crecur &&
           (fpnode->crecur /* && fpnode1->right_recur */)) {
            updir = 0;
                fpnode->res = 1;
                if (fpnode->recup_pnode->recres) {
                    printf("ambiguous syntax token %s %d"
                            "with token %s %d and %s %d ...exiting\n",
                            fpnode->crecurup_pnode->token->token_name, fpnode->crecurup_pnode->token->token_num,
                            fpnode->crecurup_pnode->recdn_pnode->token->token_name, fpnode->crecurup_pnode->recdn_pnode->token->token_num,
                            fpnode->token->token_name, fpnode->token->token_num);
                    return CREC_AMBIGUOUS;
//                    exit_parse(gills, -1);
                }
#if 0
                if (fpnode->recur_up_list) {
                    recupfl_pnode = (parse_node_t *)(fpnode->recur_up_list->next->data);
                    if (recupfl_pnode->res) {
                        printf("ambiguous between %s %d and %s %d tokens ...exiting\n",
                        fpnode->token->token_name, fpnode->token->token_num,
                        recupfl_pnode->token->token_name,
                        recupfl_pnode->token->token_num);
                        exit_parse(gills, -1);
                    }
                }
#endif
                free_parsereclist_lnode(gills, fpnode);
//                fpnode->recup_pnode->recres_pnode = fpnode;
                fpnode->recup_pnode->recdn_pnode = fpnode;
                if (!fpnode->recup_pnode->recur &&
                    !fpnode->recup_pnode->int_recur &&
                    !fpnode->recup_pnode->right_recur)
                    fpnode->recup_pnode->recres = 1;
                pnode = fpnode1->right_recur ? fpnode1 : fpnode;
                goto crecurgenlabel;
//            break;
        }

/*
           if (floating && rule->crecur)
               goto upreducelabel;
*/
           if (/* !rule->int_recur && */ rule->crecur &&
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

/*
            if (!fpnode->crecur &&
                 fpnode->up->right_recur) {
                 if (fpnode1->assoc == LEFT_ASSOC
*/
/*
            if (fpnode1->right_recur &&
                fpnode->crecur &&
                fpnode->assoc == LEFT_ASSOC) {
*/
            if (fpnode->crecur) {
crecurreducelabel:
                fpnode->res = 1;
                if (fpnode->recup_pnode->recres) {
                    printf("ambiguous syntax token %s %d"
                            "with token %s %d and %s %d ...exiting\n",
                            fpnode->crecurup_pnode->token->token_name, fpnode->crecurup_pnode->token->token_num,
                            fpnode->crecurup_pnode->recdn_pnode->token->token_name, fpnode->crecurup_pnode->recdn_pnode->token->token_num,
                            fpnode->token->token_name, fpnode->token->token_num);
                    return CREC_AMBIGUOUS;
//                    exit_parse(gills, -1);
                }
                free_parsereclist_lnode(gills, fpnode);
//                fpnode->recup_pnode->recres_pnode = fpnode;
                fpnode->recup_pnode->recdn_pnode = fpnode;
                if (!fpnode->recup_pnode->recur &&
                    !fpnode->recup_pnode->int_recur &&
                    !fpnode->recup_pnode->right_recur)
                fpnode->recup_pnode->recres = 1;
/*
                if (fpnode->rule->right_recur) {
                    fpnode->up->recres = 1;
                    pnode = fpnode->up;
                    continue;
                } else if (!fpnode->recup_pnode->parse_reclist &&
                    !fpnode->recup_pnode->recdn_pnode) {
                    fpnode->up->recres = 1;
                    fpnode->recup_pnode->recres = 1;
                    pnode = fpnode->up;
                }
                if (!fpnode->rule->right_recur) {
                    pnode = fpnode;
                    tnode = fpnode->token;
                    break;
                }
*/                    
//                fpnode->crecurup_pnode->recres = 1;
/*                if (fpnode->recup_pnode->recres_pnode) {
                    printf("ambiguity tokens %d %d ...exiting\n");
                    exit_parse(gills, -1);
                }
                fpnode->recup_pnode->recres_pnode = fpnode;
//                fpnode->recup_pnode = fpnode->crecurup_pnode;
//                fpnode->crecurup_pnode->recdn_pnode = fpnode;
                if (fpnode->
                pnode = fpnode;
                tnode = fpnode->token;
                break;
*/
            }
upreducelabel:
            uppnode = fpnode->up;
/*
            if (!uppnode->up) {
                if (uppnode->
                (uppnode->
*/
            if (!uppnode->up  /* && !floating */) {
//                if (retpnode)
//                    *retpnode = uppnode;
                gills->topreach = 1;
                uppnode->res = 1;
                uppnode->dnres_pnode = pnode;
                uppnode->dn = list_del_node(gills, uppnode->dn, pnode->updn_lnode);
                toplastexptk = get_pnode(gills);
                toplastexptk->rl_next = toplastexptk->rl_prev = toplastexptk;
                memcpy(toplastexptk, &gills->top_last_exp_pnode, sizeof(parse_node_t));
                toplastexptk->lasttk_toppnode = uppnode;
//                fpnode->rl_next = toplastexptk;
#ifndef GILLS_MEM_TKSTACK_UNLIM
                gills->tkstack[gills->tkstack_idx] = list_add(gills, gills->tkstack[gills->tkstack_idx], toplastexptk, &lnode); 
                toplastexptk->tkstackidx = gills->tkstack_idx;
                toplastexptk->tkstack_lnode = lnode;
#else
            list_mesh_add(gills, &gills->tkstack_listmesh,
                          &gills->nextop_tkstack_listmesh_list,
                          (void *)toplastexptk,
                           &gills->nextop_tkstack_listmesh_llist,
                           &toplastexptk->tkstack_lnode);
            toplastexptk->tkstack_llnode = gills->nextop_tkstack_listmesh_list;
            toplastexptk->tkstack_llmeshnode = gills->nextop_tkstack_listmesh_llist;
#endif
                toplastexptk->lasttk_toppnode = uppnode;
                toplastexptk->lasttktotoprule_pnode = fpnode;
                uppnode->top_lasttk_pnode_list =
                  list_add(gills, uppnode->top_lasttk_pnode_list,
                                  toplastexptk, &lnode);
 //               toplastexptk->updn_lnode = lnode;
                toplastexptk->toplist_lnode = lnode;
                if (floating)
                    toplastexptk->lasttk_top_floating = 1;
                if (retpnode)
                    *retpnode = toplastexptk;
/*
                ret = generate_parse(gills, NULL, gills->top_token,
                      NULL, 0, NULL, &lretpnode, NULL);
                uppnode->rl_next = lretpnode;
                lretpnode->rl_prev = uppnode;
                lretpnode->lasttktotoprule_pnode = fpnode;
*/
/*
                toppnode_list = list_add(gills, toppnode_list,
                                lretpnode, &lnode);
                lretpnode->toplist_lnode = lnode;
*/
                if (lastres_pnode)
                    *lastres_pnode = toplastexptk;
                return 0;
            }
/*            if (!floating && uppnode->res) {
                printf("ambiguous syntax token %s %d resolved"
                       "by token %s %d and %s %d ...exiting\n",
                        uppnode->dnres_pnode->token->token_name, uppnode->dnres_pnode->token->token_num,
                        fpnode->token->token_name, fpnode->token->token_num);
                return UPPNODE_AMBIGUOUS;
//                exit_parse(gills, -1);
            }
*/            uppnode->res = 1;
            uppnode->dnres_pnode = pnode;
#ifndef GILLS_FAST
            uppnode->dn = list_del_node(gills, uppnode->dn,
                                        pnode->updn_lnode);
#endif
            pnode = uppnode;
            updir = 1;
        if (updir && /* !floating && */
            pnode &&
            pnode->token->crecur &&
           (pnode->recur || pnode->int_recur /* ||
            pnode->right_recur */)) {
            updir = 0;
            goto crecurgenlabel;
//            break;
        }
        }
        }
        } // else if (
#if 0
     if (/*(floating && 
         !floating && pnode && */
         pnode && ((pnode->res && !pnode->recres &&
           pnode->token->crecur &&
         (pnode->recur ||
         pnode->int_recur ||
         pnode->right_recur)) ||
         ( /* !pnode->right_recur && */
          pnode->up &&
          !pnode->up->rule_node->right_recur &&
          pnode->crecur && pnode->res && !pnode->recres))) {
         if (lastres_pnode) *lastres_pnode = pnode;
         grpnode = gen_recur_pnode ? gen_recur_pnode : pnode;
         goto crecurgenlabel;
     }
// #if 0
     if ((floating && 
         !floating && pnode &&
         ((pnode->res && !pnode->recres &&
           pnode->token->crecur &&
         (pnode->recur ||
         pnode->int_recur ||
         pnode->right_recur)) ||
         ( /* !pnode->right_recur && */
          pnode->up &&
          !pnode->up->rule_node->right_recur &&
          pnode->crecur && pnode->res && !pnode->recres))) {
         if (lastres_pnode) *lastres_pnode = pnode;
         grpnode = gen_recur_pnode ? gen_recur_pnode : pnode;
         goto crecurgenlabel;
     }
#endif
    fpnode = pnode;
    if (lastres_pnode)
         *lastres_pnode = fpnode ? fpnode : start_pnode;
    pnode = get_pnode(gills);
//    pnode->up = fpnode ? fpnode->up : NULL;
    if (!pnode) {
//        if (retpnode) *retpnode = NULL;
        return MEM_EXHAUSTED;
    }
    pnode->rl_next = pnode->rl_prev = pnode;
    if (!fpnode) {
        pnode->rule_start = pnode->rule_end = 1;
    }
    pnode->up = fpnode ? fpnode->up : NULL;
    if (nxtrlnode && nxtrlnode->num == nxtrlnode->rule->rule_nodes_num - 1) {
        pnode->rule_end = 1;
    }
 //   grpnode = gen_recur_pnode ? gen_recur_pnode : pnode;
//    gills->pstackidx_top += 1;
    if (fpnode) {
 //       if (!floating) {
#if 0
            fpnode->uprecur = 0;
            pnode->recurup = 0;
#endif
/*
            if (!fpnode->rl_next->rule_start && pnode->floating) {
                printf("ambiguous %d %s %d %s ...exiting\n",
                       fpnode->token->token_num, fpnode->token->token_name,
                       pnode->token->token_num, pnode->token->token_name);
               exit(-1);
            }
*/
            fpnode->rl_next->rl_prev = pnode;
            pnode->rl_next = fpnode->rl_next;
            fpnode->rl_next = pnode;
            pnode->rl_prev = fpnode;
 //       }
#if 0
 else {
            fpnode->uprecur = 1;
            pnode->recurup = 1;
            fpnode->rec_prevup_list = list_add(gills, fpnode->rec_prevup_list, pnode, &lnode);
            pnode->rl_prev = fpnode;
            pnode->recprevup_lnode = lnode;
        }
#endif
    }
//    gills->pstackidx_stack[gills->pstackidx_top] = gills->pstack_idx;
    pnode->token = tnode;
    pnode->rule_node = nxtrlnode;
    pnode->rule = nxtrlnode ? nxtrlnode->rule : NULL;
//    pnode->lnode = lnode;
    pnode->nonterm = tnode->nonterm;
    if (retpnode) *retpnode = pnode;
    if (pnode && nxtrlnode && (nxtrlnode->right_recur) /* &&
        (nxtrlnode->num == nxtrlnode->rule->rule_nodes_num - 1) */) {
// pnoderuleslabel:
//    if (pnode->right_recur) {
#ifdef OPPREC_ASSOC_BOTTOM
        parse_set_rule_assoc_prec(gills, pnode);
#endif
#define OPPREC_ASSOC_LAST_TERM 1
#ifdef OPPREC_ASSOC_LAST_TERM
        parse_set_rule_assoc_prec_last_term(gills, pnode);
#endif        
    }


/*
    if (nxtrlnode && (nxtrlnode->right_recur) &&
        (nxtrlnode->num == nxtrlnode->rule->num - 1)) {
pnoderuleslabel:
//    if (pnode->right_recur) {
        parse_set_rule_assoc_prec(gills, pnode);
    }
*/
    if (pnode->token->rules_num) {
//        gills->pstackidx_top += 1;
/*
        if (gills->pstackidx_top > gills->pstackidx_max_num) {
            printf("parse node stack idx memory %d exhausted ...exiting\n",
                   gills->pstackidx_max_num);
            exit(-1);
        }
*/
//        gills->pstack_idx += 1;
//        gills->pstackidx_stack[gills->pstackidx_top] = gills->pstack_idx;
        gills->pstack_idx += 1;
        gills->pstack[gills->pstack_idx] = pnode;
    } else {
 //       token_seq_num += 1;
//        if (gen_recur_pnode)
 //           pnode->gen_recur_pnode = gen_recur_pnode;
 //       pnode->seq_num = token_seq_num;
        pnode->prec = pnode->token->prec;
        pnode->assoc = pnode->token->assoc;
        pnode->rule = pnode->rule_node->rule;
        if (pnode != start_pnode) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
            gills->tkstack[gills->tkstack_idx] = 
                list_add(gills, gills->tkstack[gills->tkstack_idx], pnode, &lnode);
            pnode->tkstackidx = gills->tkstack_idx;
            pnode->tkstack_lnode = lnode;
#else
            list_mesh_add(gills, &gills->tkstack_listmesh,
                          &gills->nextop_tkstack_listmesh_list,
                          (void *)pnode,
                           &gills->nextop_tkstack_listmesh_llist,
 //                          &pnode->tkstack_llmeshnode,
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
//        pnode->crecur = pnode->node->crecur;
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
/*
            if ((rule_node->token == pnode->token) &&
                (pnode->recur || pnode->right_recur))
                continue;

            if ((rule_node->token != pnode->token) &&
                (pnode->crecur))

            if (pnode->crecur)
                continue;
*/
            if (rlnode->crecur && !pnode->res &&
                (pnode->int_recur ||
                 pnode->right_recur)) {
                pnode->recur_list = pnode->up->recur_list;
                continue;
            } 
            rpnode = get_pnode(gills);
            if (!rpnode) {
//                if (retpnode)
//                    *retpnode = NULL;
                return MEM_EXHAUSTED;
            }
 //           rnode = ((rule_node_t *)(rlnode->data))->tknode;
/*
            if (rlnode->crecur && !pnode->res &&
                (pnode->int_recur ||
                 pnode->right_recur)) {
                pnode->recur_list = pnode->up->recur_list;
                pnode->uprecur_pnode = pnode->up->uprecur_pnode;
                continue;
            } 
*/
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
/*
            if (rpnode->crecur && !pnode->res &&
                (pnode->int_recur ||
                 pnode->right_recur)) {
                pnode->recur_list = pnode->up->recur_list;
            } 
*/
//            if (rpnode->crecur && !rpnode->recur)
//                rpnode->res = 1;
            if (!pnode->res &&
                pnode->recur /* || pnode->int_recur ||
                 pnode->right_recur) */ && rpnode->crecur) {
                pnode->recur_list = list_add(gills, pnode->recur_list, rpnode, NULL);
//                rpnode->crecur = 1;
            }
/*
            if (((!pnode->recur) ||
                (pnode->crecur && 
                 !pnode->res &&f
                 pnode->crecurup_pnode->recur &&
                 !rpnode->crecur))) {
*/
            if (/*!pnode->crecur && */ !rpnode->crecur) {
                pnode->dn = list_add(gills, pnode->dn, rpnode, &lnode);
                rpnode->updn_lnode = lnode;
                rpnode->up = pnode;
                if (rpnode->token->rules_num) {
                    gills->pstack_idx++;
                    gills->pstack[gills->pstack_idx] = rpnode;
            rpnode->uprnum = rnum;
//            pnode->dn = list_add(gills, pnode->dn, rpnode, &lnode);
//            rpnode->updn_lnode = lnode;
//            rpnode->up = pnode;
/*
            rlnode = rlnode->next;
            if (rlnode == rules[i])
                break;
*/
            rnum += 1;
       } /* else */ if (!rpnode->token->rules_num) {
 //          token_seq_num += 1;
 //          pnode->seq_num = token_seq_num; 
 //          pnode->gen_recur_pnode = grpnode;
#ifndef GILLS_MEM_TKSTACK_UNLIM
           gills->tkstack[gills->tkstack_idx] = list_add(gills, gills->tkstack[gills->tkstack_idx], rpnode, &lnode);
           rpnode->tkstackidx = gills->tkstack_idx;
           rpnode->tkstack_lnode = lnode;
#else
            list_mesh_add(gills, &gills->tkstack_listmesh,
                          &gills->nextop_tkstack_listmesh_list,
                          (void *)rpnode,
 //                         &rpnode->tkstack_llmeshnode,
                           &gills->nextop_tkstack_listmesh_llist,
                          &rpnode->tkstack_lnode);
            rpnode->tkstack_llnode = gills->nextop_tkstack_listmesh_list;
            rpnode->tkstack_llmeshnode = gills->nextop_tkstack_listmesh_llist;
#endif
           
        }
        }
     }
/*
        if (gills->pstack_idx ==
            gills->pstackidx_stack[gills->pstackidx_top])
*/
        if (gills->pstack_idx == -1)
            break;
     }
crecurgenlabel:
        if (/*pnode->recur || */
            (pnode->res && !pnode->recres &&
             pnode->token->crecur &&
              (pnode->recur || pnode->right_recur ||
             pnode->int_recur)) ||
             (!pnode->recur &&
              !pnode->right_recur && pnode->crecur &&
              pnode->res && !pnode->recres)) { /* ||
            (pnode->crecur && !pnode->res && !pnode->dn)) */
//            if (pnode->crecur) {
                reclist = pnode->crecur ?
                          pnode->up->recur_list :
                          pnode->recur_list;
                reclist1 = reclist;
//                recuruppnode = pnode->recurup_pnode;
//                crecuruppnode = pnode;
/*            } else {
                reclist = pnode->rec_list;
                recuruppnode = pnode;
                crecuruppnode = NULL;
            }

            listnode = NULL;
            crecuplistnode = NULL;
*/
/*
            parselistnode = &(gills->listnodearr[gills->listnodearr_idx]);
            gills->listnodearr_idx += 1;
//            memset(parselistnode, 0, sizeof(list_t));
            pnode->parse_reclist = list_add(gills, pnode->parse_reclist,
                         parselistnode, &listnode);
            parselistnode->next = parselistnode->prev =
            parselistnode;
            parselistnode->data = NULL;
*/
            parselistnode = list_create(gills);
            pnode->parse_reclist = list_add(gills, pnode->parse_reclist,
                         parselistnode, &listnode);

            while (reclist1) {
                 reclist = reclist->next;
                 if (reclist == reclist1)
                     break;
                 recpnode = (parse_node_t *)reclist->data;
//                 reclnode = recpnode->lnode;
                 crecpnode = get_pnode(gills);
 //                crecpnode->node = (token_node_t *)reclnode->data;
                 crecpnode->rule_start = 1;
                 crecpnode->rl_next = crecpnode->rl_prev = crecpnode;
                 crecpnode->token = recpnode->token;
                 crecpnode->rule_node = recpnode->rule_node;
                 crecpnode->rule = recpnode->rule_node ?
                                 recpnode->rule_node->rule :
                                 NULL;
//                 crecpnode->lnode = recpnode->lnode;
                 crecpnode->recup_pnode = pnode;
                 parselistnode = list_add(gills, parselistnode,
                                 crecpnode, &lnode);
//                 pnode->parse_reclist = list_add(gills, pnode->parse_reclist, crecpnode, &lnode);
                 crecpnode->crecur = 1;
                 crecpnode->parserec_llnode = listnode;
//                 crecpnode->parserec_lnode = lnode;
                 crecpnode->parserec_lnode = lnode;
                 crecpnode->recurup_pnode = crecpnode->up = pnode->recur ? pnode : pnode->up;
                 crecpnode->crecurup_pnode = pnode;
                 crecpnode->uprecur_pnode = pnode->uprecur_pnode;
                 crecpnode->assoc = crecpnode->token->assoc;
                 crecpnode->prec = crecpnode->token->prec;
/*
                 if (!pnode->recur)
                     crecpnode->res = 1;
*/
 //                crecpnode->recur = recpnode->recur;

/*
                     crecpnode->rl_next = recnxtpnode;
                     recnxtpnode->rl_prev = crecpnode;
*/
//                 crecpnode->rec_seq_num = rlpnode->up->rec_seq_num;
//                 recnxtpnode->rec_seq_num = rlpnode->up->rec_seq_num;
//                 crecpnode->up = crecpnode->recurup_pnode = recuruppnode;
//                 crecpnode->crecurup_pnode = crecuruppnode ? crecuruppnode : recuruppnode;
//                 recnxtpnode->up = rlpnode->up;
//                 crecpnode->res = 1;
                 ret = generate_parse(gills, crecpnode,
                       crecpnode->token, 
                       crecpnode->rule_node, 0,
                       crecpnode, NULL,
                       NULL);
                 crecpnode->res = 1;
            }
//        }
//            pnode->parse_reclist = list_add(gills, pnode->parse_reclist,
//                         parselistnode, &listnode);
/*
                 if (pnode->recur) {
                     ret = generate_parse(gills, pnode, pnode->token, pnode->lnode, 1, NULL, gills->tkstack_idx, NULL, &recnxtpnode);
                     if (ret) {
                         printf("generate_parse %d failed ...exiting\n", ret);
                         return ret;
//                         exit_parse(gills, -1);
                     }
                 } else */

                 if ((pnode->res && !pnode->recres &&
                     pnode->token->crecur &&(/* pnode->crecur || */
                     pnode->recur || pnode->int_recur ||
                     pnode->right_recur)) ||
                     (pnode->res && !pnode->recres && /* !pnode->right_recur  && */ pnode->crecur)) {
                     fl = 1;
//                     fpnode = pnode->crecur ? pnode->up : pnode;
                     fpnode = pnode;
                     while (1) {
                         if (fpnode->crecur) {
                             fpnode = fpnode->up;
                         } else if (fpnode->right_recur) {
                               fpnode = fpnode->rl_next;
 //                            while (fpnode->rl_prev)
 //                                fpnode = fpnode->rl_prev;
                         } else if (fpnode->recur || fpnode->int_recur) {
                             break;
                         }
                     }
                         
                     lretpnode = recnxtpnode = NULL;
                     ret = generate_parse(gills, fpnode, /* (token_node_t *)(reclnode->next->data) */ NULL, fpnode->rule_node, fl, NULL, &lretpnode, &recnxtpnode);
                     if (ret) {
                         printf("generate_parse %d failed ...exiting\n", ret);
                         return ret;
//                         exit_parse(gills, -1);
                     }
//                 }
                     recnxtpnode->floating = 1;
/*
                   if (recnxtpnode) {
                       fpnode->recur_fl_lasttk_pnode = recnxtpnode;
                   }
*/
#if 0
                    up_precur_list = NULL;
#endif
#if 0
                    if (lretpnode->token->token_num ==
                         gills->top_token->token_num)
                        recur_up_top = 1;
                    else
                        recur_up_top = 0;
                    if (!recur_up_top) {
                    up_precur_list = list_add(gills, up_precur_list, lretpnode, NULL);
                    set_uppnode_recur_list( /* pnode->parse_reclist */ parselistnode, up_precur_list,
                                           recur_up_top);
                }
#endif
                    }
            }
//    }
//    *retpnode = retpnode;
//    gills->pstackidx_top -= 1;
    return 0;
}

int get_token (gills_context_t *gills, int scan_idx)
{
    int tk;
/*
    static int tk = 2;
    static int num = 1;
    static int tk_count = 0;

    if (tk_count == 999)
        return -1;
    tk_count += 1;
    if (num) {
        num = 0;
        return 2;
    } else {
        num = 1;
    }
    tk += 1;
    if (tk == 7) {
        tk = 3;
        return tk;
    } else {
        return tk;
    }
*/
    tk = gills->scan_funcs[scan_idx](gills->scan_funcs_val[scan_idx], gills->yyscan);
#if 0
    if (tk == SIMPLEX_END_OF_SCAN) {
        tk = EOF;
    } else if (tk == SIMPLEX_FILE_READ_ERROR) {
        tk = GILLS_SCANNER_ERROR;
    }
#endif

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
//                    i = 1;
                    gills->act_val[0] = NULL;
                    while (1) {
                         gills->act_val[i] = rlpnode->val;
                         if (rlpnode->rule_end)
                             break;
                         i++;
                         rlpnode = rlpnode->rl_next;
 //                        if (!rlpnode)
 //                            break;
                    }
//    if (pnode->rule_node->rule->action)
//        (pnode->rule_node->rule->action)();
      if (lrlpnode->rule->action)
          (lrlpnode->rule->action)(gills);
                if (!gills->act_val[0])
                    pnode->val = gills->act_val[1];
                pnode->act = 1;
}

#if 0
int action_parse (gills_context_t *gills, parse_node_t *toppnode)
{
    parse_node_t *pnode = toppnode, *rlpnode, *dnres_pnode, *dnpnode, *parsereclist, *recpnode, *recuppnode;
    list_t *dnlist;
    int i;

            goto descentlabel;
uplabel:
                if (!pnode->crecur && pnode->dn) {
                    rlpnode = pnode->dnres_pnode;
                    action_ex(gills, pnode, rlpnnode);
                }
                if (pnode->crecur) {
                    while (pnode->assoc == RIGHT_ASSOC &&
                           pnode->recdn_pnode)
                           pnode = pnode->recdn_pnode;
                    action_ex(gills, pnode);
                    if (pnode->recdn_pnode) {
                        pnode->recdn_pnode->val = pnode->val;
                        pnode = pnode->recdn_pnode;
                    } else {
                        while (1) {
                                recuppnode = pnode;
                                while (recuppnode->recup_pnode != recuppnode->up &&
                                       recuppnode->assoc == LEFT_ASSOC)
                                       recuppnode = recuppnode->recup_pnode;
                                if (recuppnode->recup_pnode == recuppnode->up &&
                                    recuppnode->assoc == RIGHT_ASSOC) {
                                    recuppnode->val = recuppnode->up->val;
                                    action_ex(gills, recuppnode, recuppnode);
                                    recuppnode->up->val = recuppnnode->val;
                                    pnode = recuppnode->up;
                                    goto rlnextlabel;
                                    break;
                                } else if (recuppnode->recup_pnode == recuppnode->up &&
                                    recuppnode->assoc != RIGHT_ASSOC) {
                                    recuppnode->up->val = pnode->val;
                                    pnode = recuppnode->up;
                                    goto rlnextlabel;
                                    break;
                                } else {
                                    pnode = recuppnode->recup_pnode;
                                }
                        if (pnode->assoc == RIGHT_ASSOC) {
                            action_ex(gills, pnode, pnode);
                            if (pnode->recup_pnode != pnnode->up &&
                                pnode->recup_pnode->assoc != RIGHT_ASSOC) {
                                pnode->val = recuppnode->val;
                                action_ex(gills, pnode, pnode);
                            } else {
                                recuppnode = pnode->recup_pnode;
                                action_ex(gills, pnode, pnode);
                            }
                        }

                if (pnode->node == gills->top_token)
                    return 0;
                else if (!pnode->up) {
                    printf("token %s %d out of parse tree ...exiting\n",
                           pnode->node->token_name, pnode->node->token_num);
                    return TOKEN_OUT_OF_PARSE_TREE;
                }
                if (pnode->recur && pnode->act) {
                    if (pnode->recdn_pnode) {
                        pnode = pnode->recdn_pnode;
                        goto uplabel;
                    } else {
                        goto rlnextlabel;
                    }
/*
                    parsereclist = pnode->parse_reclist;
//                    parsereclist = parsereclist->data;
                    recpnode = (parse_node_t *)parsereclist->data;
                    recpnode->val = pnode->val;
                    pnode = recpnode;
                    goto uplabel;
*/
                }
/*
                if (pnode->crecur && !pnode->recdn_pnode) {
                    pnode = pnode->up;
                    goto rlnextlabel;
                if (pnode->crecur && pnode->recdn_pnode) {
                    while (pnode->rec_assoc == RIGHT_ASSOC &&
                           pnode->recdn_pnode)
                           pnode = pnode->recdn_pnode;
                    pnode->recdn_pnode->val = pnode->val;
                    pnode = recdn_pnode;
                    goto uplabel;
                } else if (pnode->crecur && (pnode->assoc == RIGHT_ASSOC || 
                           !pnode->recdn_pnode) {
//                    pnode->up->val = pnode->val;
                      while (pnode->act && pnode->up != pnode)
                           pnode = pnode->recup_pnode;
                      if (pnode != pnode->up)
                          goto uplabel;
                    pnode = pnode->up;
                }
*/
rlnextlabel:
                if (pnode->rl_next) {
                    pnode = pnode->rl_next;
                    goto descentlabel;
                } else {
                    while (pnode->rl_prev)
                           pnode = pnode->rl_prev;
                    if (!pnode->up) {
                        printf("%s %d is out of parse tree"
                               " ...exiting\n",
                               pnode->node->token_name,
                               pnode->node->token_num);
                        return TOKEN_OUT_OF_PARSE_TREE;
                    }
                    pnode = pnode->up;
                    goto uplabel;
                }
descentlabel:
        dnres_pnode = pnode;
        while (1) {
            if (!dnres_pnode->dn) {
                pnode = dnres_pnode;
                goto rlnextlabel;
            } else {
                dnres_pnode = dnres_pnode->dnres_pnode;
            }
        }
}
#endif

/*
parse_node_t* pnode_action_ex_inline (gills_context_t *gills,
              parse_node_t *uprecpnode, parse_node_t *uprecpnode1,
              parse_node_t *rlpnode, parse_node_t *rlpnode1,
              parse_node_t *dnrecpnode, parse_node_t *dnrecpnode1)
{
                                         rlpnode->val = dnrecpnode->val;
                                       rlpnode->val = uprecpnode->val;
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode->val = rlpnode->val;
                                       uprecpnode->recdn = rlpnode->recdn;
                                       uprecpnode->recdn->recup = uprecpnode;
                                       free_parse_rule(gills, rlpnode);
                                       pnode = uprecpnode->recdn;
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
}
*/

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
 //       free_pnode(gills, tkpnode);
        lasttk_pass = 1;
//        return;
    }
    rlpnode = pnode;
    if (lasttk_pass ||
        (crecur_begin_pnode &&
         tkpnode != crecur_begin_pnode))  {
        while (1) {
            if (rlpnode->nonterm && rlpnode->dnres_pnode) {
                rlpnode = rlpnode->dnres_pnode;
            } else {
//                rlpnode->act = 1;
                pnode = rlpnode;
                break;
            }
        }
    }
/*
    rlpnode1 = rlpnode;
    while (rlpnode1->rl_next)
        rlpnode1 = rlpnode1->rl_next;
    while (rlpnode->rl_prev)
        rlpnode = rlpnode->rl_prev;
*/
/*
    if (rlpnode == crecur_begin_pnode)
        rlpnode = rlpnode->rl_next;
    else if (rlpnode->crecur
*/
/*
    if (rlpnode->crecur) {
        if (rlpnode->up != rlpnode->recup_pnode) {
            rlpnode = rlpnode->recup_pnode;
        } else {
            if (rlpnode->right_recur) {
                rlpnode1 = rlpnode;
                while (rlpnode->rl_prev)
                    rlpnode = rlpnode->rl_prev;
                if (rlpnode->crecur)
                    pnode = rlpnode;
                else
                    pnode = tkpnode;
            }
        }
    }
*/
    while (1) {
action_loop_start:
         uppnodereached = 0;
         rlpnode = pnode;
             if (!rlpnode->res)
                 return;
/*
             if ((rlpnode->recur || rlpnode->int_recur ||
                 rlpnode->right_recur) &&
                 !rlpnode->recres)
                 return;
*/
/*
         if (rlpnode->recur || rlpnode->int_recur || rlpnode->right_recur) {
             if (!rlpnode->recres)
                 return;
         }
*/
         if (!rlpnode->nonterm)
             pnode->act = 1;
         
         if (rlpnode->recur || rlpnode->int_recur || rlpnode->right_recur) {
             if (rlpnode->act &&
                 !rlpnode->recact) {
//                 if (rlpnode->right_recur && crecur_begin_pnode) {
                     
                 if (rlpnode->recdn_pnode) {
                     rlpnode = rlpnode->recdn_pnode;
                     pnode = rlpnode;
                     dndir = 1;
                     goto action_crecur_loop_start;
                 } else if (!rlpnode->parse_reclist) {
                     pnode->recact = 1;
                 } else if (!rlpnode->right_recur) {
                     return;
                 } /* else if (rlpnode->right_recur) {
                     fpnode = rlpnode;
                     while (fpnode->rl_prev)
                         fpnode = rlpnode->rl_prev;
                     if (!fpnode->crecur && !rlpnode)
                         return;
                 } */
             }
         }

/*
             if (!rlpnode->crecur) {
                 if (!rlpnode->recur && 
                   !rlpnode->int_recur &&
                   !rlpnode->right_recur) {
                   act_done = rlpnode->act;
                 } else {
                 if (rlpnode->recur || 
                     rlpnode->int_recur ||
                     rlpnode->right_recur) {
                     act_done = rlpnode->act && rlpnode->recact ? 1 : 0;
                 }
*/
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
#if 0
                     if (rlpnode->uprecur &&
                         !rlpnode->rl_next)
                         return;
#endif
 //                    if (rlpnode->rl_next) {
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
//                     rlpnode = rlpnode->rl_next;
                     pnode = rlpnode;
#if 0
                 if (rlpnode->rec_prevup_list ||
                     rlpnode->up_recur_list ||
                     (rlpnode->recur_up_list &&
                      !rlpnode->recur_up_top))
                     return;
#endif

                     if (pnode->nonterm) {
                         rlpnode = pnode;
                         while (1) {
#ifndef GILLS_FAST
                             if (!rlpnode->dnres_pnode ||
                                  rlpnode->dn)
#else
                             if (!rlpnode->dnres_pnode) // ||
 //                                 rlpnode->dn)
#endif
                                 return;
#if 0
                 if (rlpnode->rec_prevup_list ||
                     rlpnode->up_recur_list ||
                     (rlpnode->recur_up_list &&
                      !rlpnode->recur_up_top))
                     return;
#endif

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
 //                    while (rlpnode->rl_prev)
 //                        rlpnode = rlpnode->rl_prev;

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
//                             while (fpnode->rl_prev)
//                                 fpnode = fpnode->rl_prev;
                             if (fpnode == crecur_begin_pnode) {
                                 action_ex(gills, rlpnode->up, rlpnode);
                                 rlpnode->up->act = 1;
                                 rlpnode->up->dnres_pnode = NULL;
                                 free_parse_rule(gills, rlpnode);
 //                                rlpnode->up->dnres_pnode = NULL;
                                 return;
                             }
                         }
                     }
/*
                     if (crecur_begin_pnode &&
                         crecur_begin_pnode == rlpnode) {
                         rlpnode->act = 1;
                         return;
                     }
*/
//                     rlpnode = rlpnode->up;
//                     if (up == gills->toppnode)
//                         return;
                     action_ex(gills, rlpnode->up, rlpnode);
                     pnode = rlpnode->up;
                     free_parse_rule(gills, rlpnode);
                     pnode->dnres_pnode = NULL;
                     if (pnode == gills->toppnode) {
#ifdef REDUCE_ACTION_INLINE
                         return;
#else
                         toplasttklist = pnode->top_lasttk_pnode_list;
                         while (toplasttklist) {
                             toplasttklist = toplasttklist->next;
                             toplasttkpnode = (parse_node_t *)(toplasttklist->data);
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
#endif
                     }
/*
                     if (crecur_begin_pnode) {
                         if (pnode->right_recur &&
                             !pnode->rl_next) {
                             rlpnode = pnode;
                             while (rlpnode->rl_prev)
                                 rlpnode = rlpnode->rl_prev;
                              if (rlpnode == crecur_begin_pnode)
                              return;
                         }
                     }
*/
/*
                     rlpnode1 = rlpnode;
                     while (rlpnode->rl_prev)
                         rlpnode = rlpnode->rl_prev;
*/
                     goto action_loop_start;
                 }
             }
#if 0
                 if (rlpnode->rec_prevup_list ||
                     rlpnode->up_recur_list ||
                     (rlpnode->recur_up_list &&
                      !rlpnode->recur_up_top))
                     return;
#endif
/*
             if (!rlpnode->crecur) {
                 if (rlpnode->rl_next &&
                     rlpnode->rl_next->res == 0)
                     return;
                 )
                 return;
*/
 //            if (!rlpnode->rl_next) {
               if (rlpnode->rule_end) {
                 rlpnode1 = rlpnode;
               rlpnode = rlpnode1->rl_next;
 //                while (rlpnode->rl_prev)
 //                    rlpnode = rlpnode->rl_prev;
             }
action_crecur_loop_start:
             uppnodereached = 0;
             rlpnode1 = rlpnode->rl_prev;
//             while (rlpnode1->rl_next)
 //                rlpnode1 = rlpnode1->rl_next;
#if 0
             if (!rlpnode->res || !rlpnode1->res ||
                  rlpnode->recup_pnode->parse_reclist /* ||
                  rlpnode1->parse_reclist */ )
                 return;
#endif
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
/*
             rlpassocprec = 0;
             if (assoc || prec)
                 rlpassocprec = 1;
*/
//             while (rlpnode->rl_prev)
//                 rlpnode = rlpnode->rl_prev;
             uppnode = rlpnode->up;
             if (rlpnode->crecur && rlpnode->recdn_pnode) {
//                 if (rlpnode->recdn_pnode) {
                     dnrecpnode = rlpnode->recdn_pnode;
//                     if (dnrecpnode) {
                         dncrecur = dnrecpnode->crecur;
                         dnrecpnode1 = dnrecpnode->rl_prev;
 //                        while (dnrecpnode1->rl_next)
 //                            dnrecpnode1 = dnrecpnode1->rl_next;
                         if ( /* ( */ !dnrecpnode->res /* || !dnrecpnode1->res) /* /* ||
                             (dnrecpnode->recup_pnode->parse_reclist ||
                              dnrecpnode1->parse_reclist ) */ )
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
/*                     } else {
                         dnrecpnode = dnrecpnode1 = NULL;
                         dncrecur = 0;
                         dnright_recur = 0;
                         dnassoc = dnprec = 0;
                     }
*/
                 } else {
//                     dnrecpnode = NULL;
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
//                         while (uprecpnode1->rl_next)
 //                            uprecpnode1 = uprecpnode1->rl_next;
                         if (!uprecpnode->res || !uprecpnode1->res /* ||
                             uprecpnode->recup_pnode->parse_reclist */ /* ||
                             uprecpnode1->parse_reclist */)
                             return;
                         upright_recur = 0;
                         upassoc = LEFT_ASSOC;
                         upprec = 0;
                         uppnodereached = 0;
                     } else if (uprecpnode->right_recur) {
                         uprecpnode1 = uprecpnode;
                         uprecpnode = uprecpnode1->rl_next;
 //                        while (uprecpnode->rl_prev)
 //                            uprecpnode = uprecpnode->rl_prev;
                         if (!uprecpnode->res || !uprecpnode1->res /* ||
                              uprecpnode->parse_reclist ||
                              uprecpnode1->parse_reclist */ )
                             return;
                         upassoc = uprecpnode1->assoc;
                         upprec = uprecpnode1->prec;
                         upright_recur = uprecpnode1->right_recur;
                         upcrecur = uprecpnode->crecur;
                         uppnodereached = 1;
                     } else if (uprecpnode->recur &&
                         uprecpnode == rlpnode->up) {
/*
                     upright_recur = uprecpnode1->right_recur;
                     if (upright_recur) {
                         upassoc = uprecpnode1->assoc;
                         upprec = uprecpnode1->prec;
                     } else if (upcrecur) {
                         upassoc = LEFT_ASSOC;
                         upprec = 0;
                     } else {
                         upassoc = 0;
                         upprec = 0;
                     }
                 } else {
*/
                     uppnode = rlpnode->up;
                     uprecpnode = uprecpnode1 = NULL;
                     upcrecur = upright_recur = 0;
                     upassoc = upprec = 0;
                     uppnodereached = 1;
                 }
             }
//              }
                if (rlpnode1->right_recur &&
                    rlpnode1->recdn_pnode) {
                    if (rlpnode1->recdn_pnode) {
                        rtdnrecpnode = rlpnode1->recdn_pnode;
                        rtdnrecpnode1 = rtdnrecpnode->rl_prev;
                        rtdncrecur = rtdnrecpnode->crecur;
 //                       while (rtdnrecpnode1->rl_next)
 //                           rtdnrecpnode1 = rtdnrecpnode1->rl_next;
                        if ( /* ( */!rtdnrecpnode->res /* ||
                            !rtdnrecpnode1->res) */ /* ||
                             (rtdnrecpnode->recup_pnode->parse_reclist ||
                              rtdnrecpnode1->parse_reclist) */ )
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
//           if (uppnodereached) {
               if ((crecur && !right_recur && !dnrecpnode) ||
                   (crecur && right_recur &&
                    !dnrecpnode && !rtdnrecpnode) /* || */
                   /* (!crecur && right_recur &&
                    !rtdnrecpnode) */) {
#ifndef GILLS_FAST
                       if (rlpnode->recup_pnode->parse_reclist)
                           return;
// #endif
                       if (uppnodereached && !upright_recur) {
                       if ((!((!assoc || assoc == LEFT_ASSOC) && !prec)) &&
                           ((crecur && !right_recur && rlpnode->parse_reclist) ||
                           (crecur && right_recur &&
                            rlpnode1->parse_reclist))) {
 //                          ((crecur && !right_recur /* && rlpnode->parse_reclist */ ) ||
 //                          (crecur && right_recur /* &&
 //                           rlpnode1->parse_reclist */ ))) {
                           return;
                       }
#endif
/*
                       if (!uppnode->uprecres &&
                           (assoc || prec))
                           return;
*/
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
                               change_parsereclist_recup_pnode(rlpnode->parse_reclist, rlpnode->up, rlpnode->up);
                                rlpnode->parse_reclist = NULL;
                           } else if (crecur && right_recur){
                               rlpnode->up->parse_reclist = rlpnode1->parse_reclist;
                               if (rlpnode->up->parse_reclist)
                                   rlpnode->up->recres = 0;
                               change_parsereclist_recup_pnode(rlpnode1->parse_reclist, rlpnode->up, rlpnode->up);
                               rlpnode1->parse_reclist = NULL;
                           }
                           free_parse_rule(gills, rlpnode);
                           pnode->recdn_pnode = NULL;
                           if (pnode->parse_reclist)
                               return;
                           dndir = 0;
                           goto action_loop_start;
//                           continue; // break;
/*                       } else {
                           rlpnode->up->val = rlpnode->val;
                           pnode = rlpnode->up;
                           pnode->recact = 1;
                           free_parse_rule(gills, rlpnode);
                           pnode->recdn_pnode = NULL;
                           goto action_loop_start;
//                           continue; // break;
                       }
*/
                       } else {
#ifndef GILLS_FAST
                       if ((crecur && !right_recur && rlpnode->parse_reclist) ||
                           (crecur && right_recur && rlpnode1->parse_reclist)) {
#else
                       if ((crecur && !right_recur /* && rlpnode->parse_reclist */ ) ||
                           (crecur && right_recur /* && rlpnode1->parse_reclist */ )) {
#endif
                            if (dndir)
                               return;
 //                           pnode = rlpnode = uprecpnode1;
                              pnode = rlpnode = uprecpnode;
//                            pnode = rlpnode->recup_pnode;
                            rlpnode = pnode;
                            dndir = 0;
                            goto action_crecur_loop_start;
                       }
 //                          if (assoc > upassoc ||
                           if (( !prec && !upprec && assoc >= upassoc) ||
                               ( /* assoc == upassoc && */
                                prec >= upprec)) {
                               if (upright_recur && uprecpnode1) {
                                   uprecpnode2 = uprecpnode1;
//                                   rlpnode->val = uprecpnode1->val;
                               } else {
                                   uprecpnode2 = uprecpnode;
//                                   rlpnode->val = uprecpnode->val;
                               }
                               if (!rlpnode->act)
                                   action_ex_inline(gills, rlpnode->rl_next, rlpnode);
//                               if (!uprecpnode1->act)
//                                   action_ex_inline(gills, uprecpnode->rl_next, uprecpnode);
                               rlpnode->val = uprecpnode2->val;
                               action_ex(gills, rlpnode, rlpnode);
                               uprecpnode2->val = rlpnode->val;
                               uprecpnode2->recdn_pnode = NULL;
                           if (crecur && !right_recur) {
                               uprecpnode2->parse_reclist = rlpnode->parse_reclist;
                               rlpnode->parse_reclist = NULL;
                               change_parsereclist_recup_pnode(uprecpnode2->parse_reclist, uprecpnode2, uprecpnode2);
                           } else if (crecur && right_recur){
                               uprecpnode2->parse_reclist = rlpnode1->parse_reclist;
                               rlpnode1->parse_reclist = NULL;
                               change_parsereclist_recup_pnode(uprecpnode2->parse_reclist, uprecpnode2, uprecpnode2);
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
//               }
/*
               if (crecur && !right_recur && 
                   dnrecpnode &&
                   dnrecpnode->parse_reclist)
                   return;
*/
               if (crecur && !right_recur &&
                   dnrecpnode && !rtdnrecpnode) {
/*
                       if (rlpnode->recup_pnode->parse_reclist ||
                           (!uppnodereached && rlpnode->recup_pnode->recup_pnode->parse_reclist) ||
                           dnrecpnode->recup_pnode->parse_reclist)
                           return;
*/
                       if ( /* (!assoc && !prec) || */
 //                          (((assoc > dnassoc) ||
 //                          (((!prec && !dnprec && assoc > dnassoc) ||
                           (((!prec && !dnprec && assoc >= dnassoc) ||
                            ( /* assoc == dnassoc && */
                             prec >= dnprec)) &&
                           ((uppnodereached &&
                             !upright_recur) ||
                            (((!prec && !upprec && assoc > upassoc) ||
                            (( /* assoc == upassoc && */
                              prec >= upprec))))))) {
                           if (uppnodereached && !upright_recur) {
                               rlpnode->val = rlpnode->up->val;
                           } else {
//                               if (!uprecpnode->act)
                                   //action_ex_inline(gills, uprecpnode->rl_next, uprecpnode);
                               if (upright_recur && uprecpnode1) {
                                   rlpnode->val = uprecpnode1->val;
                               } else {
                                   rlpnode->val = uprecpnode->val;
                               }
                           }
                           if (!rlpnode->act)
                               action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                           action_ex(gills, rlpnode, rlpnode);
                           if (uppnodereached &&
                               !upright_recur) {
                               rlpnode->up->val = rlpnode->val;
                               rlpnode->up->recdn_pnode = dnrecpnode;
                               rlpnode->up->parse_reclist = rlpnode->parse_reclist;
                               change_parsereclist_recup_pnode(
                               rlpnode->parse_reclist,
                               rlpnode->up, rlpnode->up);
                               dnrecpnode->recup_pnode = rlpnode->up;
                               dnrecpnode->up = rlpnode->up;
                               rlpnode->parse_reclist = NULL;
                               free_parse_rule(gills, rlpnode);
                               
                               pnode = dnrecpnode;
                               rlpnode = pnode;
                               goto action_crecur_loop_start; //continue;
                           } else {
                             if (upright_recur) {
                                 uprecpnode2 = uprecpnode1;
                             } else {
                                 uprecpnode2 = uprecpnode;
                             }
                             uprecpnode2->val = rlpnode->val;
                             uprecpnode2->recdn_pnode = dnrecpnode;
                             dnrecpnode->recup_pnode = uprecpnode2;
                             dnrecpnode->up = upright_recur ? uprecpnode1 : uprecpnode2->up;
                             change_parsereclist_recup_pnode(rlpnode->parse_reclist,
                                uprecpnode2, dnrecpnode->up);
                             uprecpnode2->parse_reclist = rlpnode->parse_reclist;
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
                                      action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                  pnode = dnrecpnode;
                              }
                              rlpnode = pnode;
                              goto action_crecur_loop_start; //continue;
                           }
                       } else {
 //                          if (upassoc > assoc ||
                           if ((!upprec && !prec && upassoc >= assoc) ||
                               ( /* upassoc == assoc && */
                                upprec >= prec)) {
                               dndir = 0;
                               pnode = uprecpnode;
 //                          } else if (dnassoc > assoc ||
                           } else if ( (!dnprec && !prec && dnassoc > assoc) ||
                               ( /* dnassoc == assoc && */
                                dnprec >= prec)) {
//                               if (!pnode->act)
//                                   action_ex_inline(gills, pnode->rl_next, pnode);
                               dndir = 1;
                               if (!rlpnode->act)
                                   action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                               pnode = dnrecpnode;
                           }
                           rlpnode = pnode;
                           goto action_crecur_loop_start;
                       }
                   }

/*
                   if ((!assoc && !prec ) ||
                       ((assoc == LEFT_ASSOC &&
                       dnrecassoc == RIGHT_ASSOC) ||
                      (assoc == dnrecassoc &&
                       prec >= dnrecprec) && 
                       ((uppnodereached) ||
                        ((assoc == LEFT_ASSOC &&
                         uprecassoc == RIGHT_ASSOC) ||
                         (assoc == uprecassoc &&
                          prec >= uprecprec))))) {
                       if (uppnodereached) {
                       rlpnode->val = rlpnode->up->val;
                       action_ex(gills, rlpnode, rlpnode);
                       if (!assoc || assoc == LEFT_ASSOC)
                           rlpnode->up->val = rlpnode->val;
                       else
                           dnrecpnode->val = rlpnode->val;
                       dnrecpnode->recup_pnode = rlpnode->up;
                       rlpnode->up->recdn_pnode = dnrecpnode;
                       pnode = dnrecpnode;
                       free_parse_rule(gills, rlpnode);
                       continue;
                           } else {
                               if (uprecassoc == dnrecassoc ==
                                   assoc) {
                                       if (!uprecright_recur) {
                                       rlpnode->val = uprecpnode->val;
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode->val = rlpnode->val;
                                       uprecpnode->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode->recdn->recup = uprecpnode;
                                       free_parse_rule(gills, rlpnode);
                                       pnode = uprecpnode->recdn;
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } else {
                                       rlpnode->val = uprecpnode1->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode1->val = rlpnode->val;
                                       uprecpnode1->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode1->recdn->recup = uprecpnode1;
                                       free_parse_rule(gills, rlpnode);
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode1->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } 
                               } else if (uprecassoc != dnrecassoc) {
                                   if ((uprecassoc == RIGHT_ASSOC &&
                                        assoc == LEFT_ASSOC) {
                                        rlpnode->val = uprecpnode->val;
                                       if (!uprecright_recur) {
                                       rlpnode->val = uprecpnode->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode->val = rlpnode->val;
                                       uprecpnode->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode->recdn->recup = uprecpnode;
                                       free_parse_rule(gills, rlpnode);
                                       pnode = uprecpnode->recdn;
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } else {
                                       rlpnode->val = uprecpnode1->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode1->val = rlpnode->val;
                                       uprecpnode1->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode1->recdn->recup = uprecpnode1;
                                       free_parse_rule(gills, rlpnode);
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode1->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } 
                                   } else if (rtdnrecassoc == LEFT_ASSOC &&
                                              assoc == RIGHT_ASSOC) {
                                         rlpnode->val = dnrecpnode->val;
                                       if (!uprecright_recur) {
                                       rlpnode->val = uprecpnode->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode->val = rlpnode->val;
                                       uprecpnode->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode->recdn->recup = uprecpnode;
                                       free_parse_rule(gills, rlpnode);
                                       pnode = uprecpnode->recdn;
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } else {
                                       rlpnode->val = uprecpnode1->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode1->val = rlpnode->val;
                                       uprecpnode1->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode1->recdn->recup = uprecpnode1;
                                       free_parse_rule(gills, rlpnode);
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode1->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } 
                                }
                             }
                         }
                   } else {
                       if ((dnrecpnode && dncrecur) &&
                          ((dnrecpnode->recdn && !dnrecpnode->parse_reclist) ||
                           (rtdnrecpnode && rtdncrecur && 
                            ((rtdnrecpnode->recdn && !rtdnrecpnode->parse_reclist) ||
                           (rtdnrecpnode1 && rtdnright_recur &&
                            rtdnrecpnode1->recdn && !rtdnrecpnode1->parse_reclist))))) {
                           pnode = dnrecpnode;
                           continue;
                        } else {
                           return;
                        }
                   }
*/
/*                   if (crecur && right_recur &&
                       rtdnrecpnode &&
                       rtdnrecpnode->parse_reclist)
                       return;
*/
                   if (crecur && right_recur &&
                       !dnrecpnode && rtdnrecpnode) {
/*                       if (rlpnode->recup_pnode->parse_reclist ||
                           (uppnodereached && rlpnode->up->parse_reclist) ||
                           (!uppnodereached && rlpnode->recup_pnode->recup_pnode->parse_reclist) ||
                           rtdnrecpnode->recup_pnode->parse_reclist) 
                           return;
*/
                       if ( /* (!assoc && !prec) || */
 //                          (((assoc > rtdnassoc) ||
 //                          (((!prec && !rtdnprec && assoc > rtdnassoc) ||
                           (((!prec && !rtdnprec && assoc >= rtdnassoc) ||
                            ( /* assoc == rtdnassoc && */
                             prec >= rtdnprec)) &&
                           ((uppnodereached && !upright_recur) ||
 //                           (((!prec && !upprec && assoc > upassoc) ||
                            (((!prec && !upprec && assoc >= upassoc) ||
                            (( /* assoc == upassoc && */
                              prec >= upprec))))))) {
                           
/*
                   if (crecur && right_recur &&
                       !dnrecpnode && rtdnrecpnode) {
                       if ((!assoc && !prec) ||
                           (assoc == LEFT_ASSOC &&
                            rtdnrecassoc == RIGHT_ASSOC) ||
                           ((assoc == rtdnrecassoc &&
                            prec >= rtdnrecprec) && 
                       ((uppnodereached) ||
                        ((assoc == LEFT_ASSOC &&
                         uprecassoc == RIGHT_ASSOC) ||
                         (assoc == uprecassoc &&
                          prec >= uprecprec))))) {
*/
                           if (uppnodereached && !upright_recur) {
                               rlpnode->val = rlpnode->up->val;
                           } else {
//                               if (!uprecpnode->act)
//                                   action_ex_inline(gills, uprecpnode->rl_next, uprecpnode);
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
                               goto action_crecur_loop_start; //continue;
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
 //                            if (upassoc > rtdnassoc ||
                             if ((!upprec && !rtdnprec && upassoc > rtdnassoc) ||
                                 ( /* upassoc == rtdnassoc && */
                                  upprec >= rtdnprec)) {
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
//                               if (!pnode->act)
//                                   action_ex_inline(gills, pnode->rl_next, pnode);
                               if (!rlpnode->act)
                                   action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                               pnode = rtdnrecpnode;
                               rlpnode = pnode;
                               dndir = 1;
                               goto action_crecur_loop_start;
                           } else {
 //                          if (upassoc > assoc ||
                           if ((!upprec && !prec && upassoc > assoc) ||
                               ( /* upassoc == assoc && */
                                upprec >= prec)) {
                               dndir = 0;
                               pnode = uprecpnode;
                           } else if ((!rtdnprec && !prec && rtdnassoc > assoc) ||
                               ( /* rtdnassoc == assoc && */
                                rtdnprec >= prec)) {
                               if (!rlpnode->act)
                                   action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                               dndir = 1;
                               pnode = rtdnrecpnode;
                           }
                           rlpnode = pnode;
                           goto action_crecur_loop_start;
                       }
                       }
                   }
/*
                           if (uppnodereached) {
                           rlpnode->val = rlpnode->up->val;
                           action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                           action_ex(gills, rlpnode, rlpnode);
                           if (!assoc || 
                               assoc == LEFT_ASSOC ||
                               rtdncrecur == 0) {
                               rlpnode->up->val = rlpnode->val;
                           } else {
                               rtdnrecpnode->val = rlpnode->val;
                           }
                            rtdnrecpnode->recup_pnode = 
                            rtdnrecpnode->up = rlpnode->up;
                            rlpnode->up->recdn_pnode = rtdnrecpnode;
                            free_parse_rule(gills, rlpnode);
                            pnode = rtdnrecpnode;
                            continue;
                           } else {
                               if (uprecassoc == dnrecassoc ==
                                   assoc) {
                                       if (!uprecright_recur) {
                                       rlpnode->val = uprecpnode->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode->val = rlpnode->val;
                                       uprecpnode->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode->recdn->recup = uprecpnode;
                                       free_parse_rule(gills, rlpnode);
                                       pnode = uprecpnode->recdn;
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } else {
                                       rlpnode->val = uprecpnode1->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode1->val = rlpnode->val;
                                       uprecpnode1->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode1->recdn->recup = uprecpnode1;
                                       free_parse_rule(gills, rlpnode);
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode1->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } 
                               } else if (uprecassoc != dnrecassoc) {
/*                                   if ((uprecassoc == RIGHT_ASSOC &&
                                        assoc == LEFT_ASSOC) {  */
/*                                   if ((uprecassoc > dnrecassoc) ||
                                       (uprecassoc == dnrecassoc &&
                                        uprecprec > dnrecprec) || 
                                        (uprecassoc == dnrecassoc &&
                                         uprecprec == dnrecprec)) {
                                        rlpnode->val = uprecpnode->val;
                                       if (!uprecright_recur) {
                                       rlpnode->val = uprecpnode->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode->val = rlpnode->val;
                                       uprecpnode->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode->recdn->recup = uprecpnode;
                                       free_parse_rule(gills, rlpnode);
                                       pnode = uprecpnode->recdn;
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } else {
                                       rlpnode->val = uprecpnode1->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode1->val = rlpnode->val;
                                       uprecpnode1->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode1->recdn->recup = uprecpnode1;
                                       free_parse_rule(gills, rlpnode);
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode1->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } 
                                   } else if (rtdnrecassoc == LEFT_ASSOC &&
                                              assoc == RIGHT_ASSOC) {
                                         rlpnode->val = dnrecpnode->val;
                                       if (!uprecright_recur) {
                                       rlpnode->val = uprecpnode->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode->val = rlpnode->val;
                                       uprecpnode->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode->recdn->recup = uprecpnode;
                                       free_parse_rule(gills, rlpnode);
                                       pnode = uprecpnode->recdn;
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } else {
                                       rlpnode->val = uprecpnode1->val;
                                       action_ex_inline(gills, rlpnode->rl_next, rlpnode);
                                       action_ex(gills, rlpnode, rlpnode);
                                       uprecpnode1->val = rlpnode->val;
                                       uprecpnode1->recdn = !right_recur ? rlpnode->recdn : rlpnode1->recdn;
                                       uprecpnode1->recdn->recup = uprecpnode1;
                                       free_parse_rule(gills, rlpnode);
                                       if ((uprecassoc >= dnrecassoc) ||
                                           (uprecassoc == dnrecassoc &&
                                            uprecprec >= dnrecprec)) {
                                       pnode = uprecpnode1->recdn;
                                       } else {
                                           pnode = dnrecpnode;
                                       }
                                       continue;
                                   } 
                        } else {
                            if ((rtdnrecpnode && rtdncrecur) &&
                                ((rtdnrecpnode->recdn && !rtdnrecpnode->parse_reclist) ||
                                 (rtdnrecpnode1 && rtdnright_recur && 
                                  rtdnrecpnode1->recdn && 
                                 !rtdnrecpnode1->parse_reclist))) {
                                pnode = rtdnrecpnode;
                                continue;
                            } else {
                                return;
                            }
                        }
                    }
*/
                    if (!crecur && right_recur) {
#ifndef GILLS_FAST
                        if (rlpnode1->parse_reclist)
                            return;
#endif
//                        if (!rlpnode1->act)
//                            action_ex_inline(gills, rlpnode, rlpnode);
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
//}

#if 0
void set_uppnode_recur_list (list_t *rec_list, list_t *up_list, int recur_up_top)
{
    parse_node_t *recpnode, *uppnode;
    list_t *reclist = rec_list, *uplist = up_list, *reclist1;
    int up_recur = 0, i;

    for (i = 0; i < 2; i++) {
    reclist1 = reclist;         
    while (1) {
        reclist = reclist->next;
        if (reclist == reclist1)
            break;
        recpnode = (parse_node_t *)reclist->data;
//        while (1) {
//            uppnode = (parse_node_t *)uplist->data;
            if (!up_recur) {
                recpnode->recur_up_list = uplist;
                if (recur_up_top)
                    recpnode->recur_up_top = 1;
//                recpnode->recur_up_list = list_add(gills, recpnode->recur_up_list, uppnode, NULL);
            } else {
                recpnode->up_recur_list = uplist;
            }
//                recpnode->up_recur_list = list_add(gills, recpnode->up_recur_list, uppnode);
/*
            uplist = uplist->next;
            if (uplist == up_list)
                break;
*/
//        }
    }
    reclist = up_list;
    uplist = rec_list;
    up_recur = 1;
    }
}
#endif

/*
int check_recur_up_ambiguity (parse_node_t *pnode)
{
    list_t *recurlist;
    int i;
    parse_node_t *recpnode;

    if (!pnode->up_recur_list && !pnode->recur_up_list)
        return 0;

    for (i = 0; i < 2; i++) {
    recurlist = recurlist ? pnode->up_recur_list : pnode->recur_up_list;
    if (!recurlist)
        continue;
    rec_list = recurlist;
    while (1) {
        recpnode = (parse_node_t *)recurlist->data;
        if (recpnode->res) {
            printf("tokens ambiguous %s %d and %s %d ...exiting\n",
                   pnode->token_name, pnode->token_num,
                   recpnode->token_name, recpnode->token_num);
            return AMBIGUITY;
        }
        recurlist = recurlist->next;
        if (recurlist == recur_list)
            break;
    }
    }
    return 0;
}
*/
#if 0
int reduce_parse (gills_context_t *gills,
                  parse_node_t *rpnode, parse_node_t **retp)
{
    parse_node_t *pnode = rpnode, *prevpnode, *recpnode, *parsenxtrecpnode, *rlpnode, *crecpnode, *recuppnode, *recnxtpnode, *uppnode, *retpnode, *resrecpnode, *upuppnode, *recur_up_pnode, *actpnode, *crecur_pnode;
    token_node_t *rnode, *recnode, *upnode, *upnxtnode;
    list_t *rlnode, *rlnxtlnode, *reclist, *parsereclist, *reclnode, *uplnode, *listnode, *up_precur_list, *up_recur_list, *crecuplistnode;
    int recseqnum, crecur_occur = 0;
    rule_node_t *rulnode;

    while (1) {
    prevpnode = pnode;
    while (1) {
        if (pnode->res && !crecur_occur) {
            printf("ambiguous syntax token %s %d ...exiting\n",
                   pnode->token->token_name,
                   pnode->token->token_num);
            exit_parse(gills, -1);
        }
        pnode->res = 1;
        rnode = pnode->node;
        rlnode = pnode->lnode;
            rulnode = (rule_node_t *)(rlnode->data);
            uptknode = rulnode->up;
            if (uptknode && uptknode->rules[rulnode->rule_num] != rlnode->next) {
                rlnxtlnode = rlnode->next;
                
            up_recur_list = NULL;
            ret = generate_parse(gills, (token_node_t *)(rlnxtlnode->data), rlnxtlnode, pnode, gills->tkstack_idx, 0, &up_recur_list, &retpnode);
            if (ret)
                return ret;
            pnode->rl_next = retpnode;
            retpnode->rl_prev = pnode;
                break;
            } else if (uptknode) {
                rlpnode = pnode;
               while  (rlpnode->prev)
                    rlpnode = rlpnode->prev;
                if (/*rlpnode->recur || */ rlpnode->crecur) {
                    rlnxtlnode = NULL;
//                    if (prevpnode != rlpnode)
            uppnode = rlpnode->up;
            if (uppnode->res && uppnode->parserec_pnode) {
                printf("ambiguous tokens %s %d %s %d ...exiting\n",
                       uppnode->parserec_pnode->token->token_name, uppnode->parserec_pnode->token->token_num,
                       pnode->token->token_name, pnode->token->token_num);
                exit_parse(gills, -1);
            } else if (!uppnode->res) {
                uppnode->res = 1;
                uppnode->parserec_pnode = pnode;
                free_parsereclist_lnode(gills, pnode);
            }
            ret = generate_parse(gills, (token_node_t *)(rlpnode->data), rlpnode->list_node, rlpnode, gills->tkstack_idx, 1, &up_recur_list, &retpnode);
            if (ret)
                return ret;
            crecur_pnode = rlpnode;
            if (up_precur_list) {
                set_uppnode_recur_list(rlpnode->parse_reclist, up_precur_list);
            }
#ifdef ACTION_REDUCE_INLINE
            fpnode = rlpnode;
            uppnode = rlpnode->crecurup_pnode ? rlpnode->crecurup_pnode : rlpnode->recurup_pnode;
            while (fpnode->rl_next)
                fpnode = fpnode->rl_next;
            ret = action_ex_inline(gills, fpnode, 0);
#endif
#if 0
            if (ret)
// #endif
            {
                rlpnode->recdn_pnode = retpnode;
                retpnode->recup_pnode = rlpnode;
                retpnode->up = uppnode;
            }
#ifdef ACTION_REDUCE_INLINE
            else {
                recup_pnode->recdn_pnode = retpnode;
                retpnode->recup_pnode = recup_pnode;
           }
#endif
#endif
                
            
            uppnode = rlpnode->up;
            while (1) {
                if (!uppnode)
                    return 0;
                fpnode = uppnode;
                while (fpnode->rl_prev)
                       fpnode = fpnode->rl_prev;
                rlnode = uppnode->lnode;
                rnode = (rule_node_t *)(rlnode->data);
                if (rlnode->next != fpnode->lnode)
                    break;
                uppnode = fpnode->up;
            }
            rlpnode = uppnode;
            rlnode = rlnode->next;
            ret = generate_parse(gills, ((rule_node_t *)(rlnode->data))->tknode, rlnode, NULL, gills->tkstack_idx, 1, NULL, &retpnode);
            if (ret)
                return ret;
            rlpnode->rl_next = retpnode;
            retpnode->rl_prev = rlpnode;
            set_uppnode_recur_list(crecur_pnode->parse_reclist, listnode);
/*
            if (up_precur_list) {
                listnode = NULL;
                listnode = list_add(gills, listnode, retpnode, NULL);
                set_uppnode_recur_list(up_precur_list, listnode);
                listnode = list_del_head(listnode);
                retpnode->up_precur_list = up_precur_list;
                up_precur_list = NULL;
            }
*/
        } else {
                    pnode = pnode->up;
                    if (pnode->res) {
                        printf("ambiguous syntax at token %s %d ...exiting\n",
                               pnode->token->token_name,
                               pnode->token->token_num);
                        exit_parse(gills, -1);
                    }
                    pnode->res = 1;
                    continue;
                }
            } else if (!uptknode) {
                if (up_precur_list)
                    pnode->up_precur_list = pnode;
                *retp= pnode;
                return 0;
            }
        }
    }
        if (!rlnxtlnode) {
            rlpnode = pnode;
        if (pnode->res == 1 &&
            (pnode->recur || pnode->right_recur)) {
            check_recur_up_ambiguity(pnode);
            pnode->rec_seq_num += 1;
#ifdef REDUCE_INLINE_ACTION
            if (pnode->recur)
                action_ex(gills, pnode, pnode);
#endif
            reclist = pnode->recur_list;
            listnode = NULL;
            crecuplistnode = NULL;
            while (reclist) {
                 recpnode = (parse_node_t *)reclist->data;
                 reclnode = recpnode->lnode;
                 if (!reclnode->next) {
                     //error
                 }
                 crecpnode = get_pnode(gills);
                 crecpnode->node = (token_node_t *)reclnode->data;
                 if (up_precur_list)
                     crecpnode->up_precur_list = up_precur_list;
                 crecpnode->lnode = reclnode;
                 crecpnode->recup_pnode = pnode;
                 pnode->recdn_pnode = crecpnode;

#ifdef REDUCE_ACTION_INLINE
                 actpnode = pnode;
                 while (actpnode->rl_next)
                     actpnode = actpnode->rl_next;
                 action_ex_inline(gills, actpnode, 0);
                 crecpnode->val = pnode->val;
#endif

#ifdef REDUCE_ACTION_INLINE
                 
                 if (pnode->assoc == LEFT_ASSOC) {
                     recpprevpnode = pnode->recup_pnode;
                     lastpnode = recpprevpnode;
//                     pnode->val = recprevpnode->val;
//                     action_ex(gills, pnode, pnode);
//                     recpprevnode = recprevpnode;
                     recprevpnode = pnode;
                     while (1) {
                         if ((recpprevpnode->assoc == TOP_ASSOC) ||
                             (recpprevpnode->assoc > recprevpnode->assoc)
#ifdef REDUCE_ACTION_ASSOC_NOT_STRICT
                             || recpprevpnode->assoc == recpprevpnode->assoc)
#endif
                          ) {
#if 0
                         if ((recprevpnode->assoc == LEFT_ASSOC &&
                              recpprevpnode->assoc == RIGHT_ASSOC) ||
                              ((recprevpnode->assoc == LEFT_ASSOC &&
                               recpprevpnode->assoc == LEFT_ASSOC) &&
                              recpprevpnode->prec > recprevpnode->prec) ||
                              (recprevpnode->assoc == NO_ASSOC &&
                               recpprevpnode->assoc == ) {
#endif
                             action_ex(gills, recpprevpnode, recpprevpnode);
                           } else {  
                             break;
                           }
                               if (recpprevpnode->right_recur)

                               recprevpnode = recpprevpnode;
                               recpprevpnode = recpprevpnode->recup_pnode;
                     }
                         if (recprevpnode->assoc == RIGHT_ASSOC &&
                             recpprevpnode->assoc == LEFT_ASSOC) {
                             action_ex(gills, recpprevpnode, recpprevpnode);
                             valpnode = recprevpnode;
                             while (1) {
                             if (recpprevpnode->recup_pnode !=
                                 recpprevpnode->up) {
                                 recprevpnode->recup_pnode = recpprevpnode->recup_pnode;
                                 recpprevpnode->recup_pnode->recdn_pnode = recprevpnode;
                                 recprevpnode->val = recpprevpnode->val;
                                 recpprevpnode = recpprevpnode->recup_pnode;
                                 free_pnode(gills, recpprevpnode);
                              } else {
                                  break;
                              }
                              if (recpre
                         if ((recprevpnode->assoc == RIGHT_ASSOC) ||
              //                recprev
                             (recprevpnode->prec && 
                              recpprevpnode->prec &&
                             recpprevpnode->prec <= recprevpnode->prec)) {
                             recpprevpnode = recprevpnode == pnode->up ? NULL : recprevpnode->recup_pnode;
                             if (recpprevpnode) {
                                 lastpnode = oppnode->rl_next;
                                 recprevpnode->val = lastpnode->val;
                                 action_ex(gills, recprevpnode, recprevpnode);
                             if (recpprevpnode) {
                                 recprevpnode = recpprevpnode;
                             } else {
                                 break;
                             }
                         } else if (recprevpnode->assoc == LEFT_ASSOC) {
                             recpprevpnode = recprevpnode;
                             recpprevpnode->val = recprevpnode->val;
                             free_pnode(gills, recprevpnode);
                             if (recprevpnode->up == recpprevpnode)
                                 break;
                             recprevpnode = recpprevpnode;
                             recpprevpnode = recprevpnode->recup_pnode;
                         }
                     }
                 } /* else if (opnode->assoc == RIGHT_ASSOC) { */
#endif                     

                 crecpnode->crecur = 1;
                 ret = generate_parse(gills, (token_node_t *)(reclnode->data), reclnode, crecpnode, gills->tkstack_idx, &recnxtpnode);
                 if (ret)
                     return ret;
/*
                 crecpnode->rl_next = recnxtpnode;
                 recnxtpnode->rl_prev = crecpnode;
*/
                 crecpnode->rec_seq_num = rlpnode->up->rec_seq_num;
//                 recnxtpnode->rec_seq_num = rlpnode->up->rec_seq_num;
                 listnode = list_add(gills, listnode, crecpnode, &crecuplistnode);
//                 crecpnode->upparse_rec_list = listnode;
//                 crecpnode->upreclist_lnode = crecuplistnode;
                 crecpnode->rl_next = recnxtpnode;
//                 recnxtpnode->rl_prev = crecpnode;
//                 crecpnode->up = rlpnode->up;
                 crecpnode->up = rlpnode;
//                 recnxtpnode->up = rlpnode->up;
                 reclist = reclist->next;
                 if (reclist == pnode->recur_list)
                     break;
            }
            if (up_precur_list)
                set_uppnode_recur_list(up_precur_list, listnode);
            up_precur_list = listnode;
            pnode->parse_reclist = list_add(gills, pnode->parse_reclist, listnode, NULL);
            pnode->res = 1;
        } else if (rlpnode->crecur) {
            check_recur_up_ambiguity(pnode);
                   recseqnum = rlpnode->up->rec_seq_num;
                   parsereclist = rlpnode->up->parse_reclist;
                   while (1) {
                       recseqnum -= 1;
                       if (!recseqnum)
                           break;
                       parsereclist = parsereclist->next;
                   }
                   listnode = parsereclist;
                   while (1) {
                       crecpnode = (parse_node_t *)listnode;
                       if (crecpnode != rlpnode && crecpnode->res) {
                           printf("ambiguity tokens %s %d and %s %d ...exiting\n",
                           crecpnode->token_name, crecpnode->token_num,
                           pnode->token_name, pnode->token_num);
                           return AMBIGUITY;
                       }
                       listnode = listnode->next;
                       if (listnode == parsereclist)
                           break;
                   }
                   uppnode = rlpnode->up;
               listnode = parsereclist->data;
               if (/*!rlpnode->recur_up_list && */ listnode == listnode->next) {
                   rlpnode->res = 1;
/*
#ifdef REDUCE_ACTION_INLINE
                   action_ex(gills, rlpnode, rlpnode);
                   free_parse_nodes(gills, rlpnode);
              }

#endif
*/
               if (rlpnode->crecur && !rlpnode->right_recur &&
                   rlpnode->up->right_recur) {
                   if (rlpnode->up->right_recur_end) {
                       if (rlpnode->assoc == RIGHT_ASSOC &&
                           rlpnode->prec < rlpnode->up->prec) {
                           rlpnode->recdn_pnode = NULL;
#ifdef REDUCE_ACTION_INLINE
                           fpnode = rlpnode;
                           while (fpnode->rl_next)
                               fpnode = fpnode->rl_next;
                           action_ex_inline(gills, fpnode, 1);
#endif
                           pnode = rlpnode->up;
                   }
               }
	           recseqnum = uppnode->rec_seq_num;
               recuppnode = uppnode;
               reclist = recuppnode->recur_list;
               recuppnode->rec_seq_num += 1;
               while (1) {
                   recpnode = (parse_node_t *)reclist->data;
                   reclnode = recpnode->lnode;
                   crecpnode = get_pnode(gills);
                   crecpnode->node = (token_node_t *)reclnode->data;
                   crecpnode->lnode = reclnode;

#ifdef REDUCE_ACTION_INLINE
                   crecpnode->recup_pnode = uppnode;
                   uppnode->recdn_pnode = crecpnode;
#else
                   crecpnode->recup_pnode = rlpnode;
                   rlpnode->recdn_pnode = crecpnode;
#endif
                   crecpnode->crecur = 1;
                   crecpnode->rec_seq_num = recuppnode->rec_seq_num;
                   ret = generate_parse(gills, (token_node_t *)(reclnode->next->data), reclnode->next, crecpnode, gills->tkstack_idx, &recnxtpnode);
                   if (ret)
                       return ret;
                   crecpnode->rl_next = recnxtpnode;
                   recnxtpnode->rec_seq_num = recuppnode->rec_seq_num;
                   crecpnode->up = uppnode;
                   if (up_precur_list)
                       crecpnode->up_precur_list = up_precur_list;
                   recnxtpnode->up = rlpnode->up;
                   listnode = list_add(gills, listnode, crecpnode, NULL);
                   recnxtpnode->recup_pnode = rlpnode;
                   recnxtpnode->rl_prev = crecpnode;
                   reclist = reclist->next;
                   if (reclist == pnode->up->recur_list)
                       break;
               }
               if (up_precur_list)
                   set_uppnode_recur_list(up_precur_list, listnode);
               up_precur_list = listnode;
           }
/*
             else if (!pnode->recur && !pnode->crecur &&
               pnode->res) {
               printf("ambiguity %s %d ...exiting\n",
                      pnode->node->token_name,
                      pnode->node->token_num);
               return AMBIGUITY;
           }
*/
               pnode->res = 1;
               uppnode = pnode->up;
               if (!uppnode->up && uppnode->node == gills->top_token) {
                   *retp = uppnode;
                   return 0;
               } else if (!uppnode->up && uppnode->node != gills->top_token) {
                   printf("token %s %d out of parse tree ...exiting\n", pnode->node->token_name, pnode->node->token_num);
                   return TOKEN_OUT_OF_PARSE_TREE;
               }
               uppnode->dnrnum = pnode->rnum;
               uppnode->seq_num = pnode->seq_num;
               if (!uppnode->recur && !uppnode->crecur)
                   uppnode->dnres_pnode = pnode;
               upnode  = uppnode->node;
               uplnode = uppnode->lnode;
               if (uppnode->recur /* || uppnode->crecur */) {
                   pnode = uppnode;
               } /* else  {
                   if (uppnode) {
                       pnode = uppnode;
                   } else {
                       if (pnode->node == gills->top_token) {
                           return 0;
                       } else {
                           printf("token %s %d out of parse tree ...exiting\n",
                           pnode->node->token_name,
                           pnode->node->token_num);
                           return TOKEN_OUT_OF_PARSE_TREE;
                       }
                   }
               } */
           }
        } else {
            ret = generate_parse(gills, (token_node_t *)(rlnxtlnode->data), rlnxtlnode, NULL, gills->tkstack_idx, &retpnode);
            if (ret)
                return ret;
            if (up_precur_list) {
                listnode = NULL;
                listnode = list_add(gills, listnode, retpnode, NULL);
                set_uppnode_recur_list(up_precur_list, listnode);
                listnode = list_del_head(gills, listnode);
                retpnode->up_precur_list = up_precur_list;
                up_precur_list = NULL;
            }
            pnode->rl_next = retpnode;
            retpnode->rl_prev = pnode;
            break;
        }
    }
    *retp = retpnode;
    return 0;
}
#endif
/*
void reduce_recur_parse (gills_context_t *gills,
                         parse_node_t *recpnode)
{
    parse_node_t *recup_pnode, *recdn_pnode = recpnode;

    recup_pnode = recdn_pnode->recup_pnode;
    while (1) {
        recup_pnode->val = NULL;
        action_ex(gills, recup_pnode, recup_pnode);
        recdn_pnode->val = recup_pnode;
        action_ex(gills, recdn_pnode, recdn_pnode);
        recup_pnode->val = recdn_pnode);
        if (recup_pnode == recdn_pnode->up)
            break;
        recdn_pnode = recup_pnode;
        recup_pnode = recup_pnode->recup_pnode;
     }
}
*/

void free_parsereclist_lnode (gills_context_t *gills, parse_node_t *pnode)
{
#ifndef GILLS_FAST
    parse_node_t *up_pnode, *uprecurpnode;
    list_t *parsereclist;
    list_t *llnode, *lnode;

    llnode = (list_t *)(pnode->parserec_llnode->data);
//    up_pnode = pnode->recup_pnode;
//    parsereclist = up_pnode->parse_reclist;
//    llnode = (list_t *)(llnode->data);
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
 //       pnode->parserec_llnode = NULL;
//        pnode->parserec_lnode = NULL;
        pnode->parserec_llnode = NULL;
#if 0
        if (pnode->recur_up_list) {
            uprecurpnode = (parse_node_t *)(pnode->recur_up_list->next->data);
            if (uprecurpnode->up_recur_list) {
//                free_list(uprecurpnode->up_recur_list);
                uprecurpnode->up_recur_list = NULL;
            }
            free_list(pnode->recur_up_list);
            pnode->recur_up_list = NULL;
        }
#endif
    }
/* 
    up_pnode->parse_reclist = 
                    list_del_node(gills, up_pnode->parse_reclist,
                    pnode->parserec_lnode);
*/
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
	//	if (!nxtrlpnode) {
            if (end)
                break;
	//	}
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
//                        if (prevpnode->parse_reclist ||
//                            prevpnode->recdn_pnode) {
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
/*                            if (!updir) {
                            free_pnode(gills, prevpnode);
                            fpnode->rl_next = NULL;
                            pnode = fpnode;
                            }
*/                    }
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
        /* pnode->lasttk_toppnode->res = */
        pnode->lasttk_toppnode->topres = 0;
        toplasttk_list = pnode->lasttk_toppnode->top_lasttk_pnode_list;
        pnode->lasttk_toppnode->top_lasttk_pnode_list = 
               list_del_node(gills, toplasttk_list,
 //                            pnode->updn_lnode);
                               pnode->toplist_lnode);
        if (!pnode->lasttk_toppnode->top_lasttk_pnode_list &&
             !pnode->lasttk_toppnode->dn &&
             !pnode->lasttk_toppnode->dnres_pnode) {
            free_pnode(gills, pnode->lasttk_toppnode);
            gills->toppnode = NULL;
            gills->top_free = 1;
            /* if (free_flag) */ free_pnode(gills, pnode);
            return;
        }
//        return;
        free_pnode(gills, pnode);
        if (!top_res)
            return;
        if (!fpnode->token)
            return;
        pnode = fpnode;
        goto rlnextfreelabel;
//        if (toplasttk_list)
//            return;
        
    }

rlnextfreelabel: 
    nextpnode = pnode;
    fpnode = pnode;
    while (!nextpnode->rl_next->rule_start)
        nextpnode = nextpnode->rl_next;
 //   while (nextpnode->rl_next)
 //       nextpnode = nextpnode->rl_next;
    pnode = nextpnode;
    next = 1;
    if (lasttk_top_floating) {
        if (fpnode->recur)
            goto rldnfreelabel;
    }
    if (pnode->recur || pnode->right_recur ||
        pnode->int_recur) {
//        recpnode_pass += 1;
//        pnode->recur_free = 1;
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
#if 0
        if ( /* pnode->recdn_pnode && */
            pnode->parse_reclist) {
            return;
        } else 
#endif
if (pnode->recdn_pnode /* &&
                 !pnode->parse_reclist */) {
            fpnode = pnode;
            while (1) {
/*
                if (!pnode->recdn_pnode) 
                    break;
*/
//                if (fpnode->parse_reclist)
//                    return;
                if (!fpnode->recdn_pnode) {
                    if (fpnode->parse_reclist)
                        return;
                    break;
                }
                fpnode = fpnode->recdn_pnode;
            }
            pnode = fpnode;
//            if (fpnode != pnode)
//                recpnode_pass += 1; 
            goto /* recurfreelabel; */ rlnextfreelabel;
        } else if (!pnode->recdn_pnode &&
                   pnode->parse_reclist) {
            return;
        } else if (!pnode->recdn_pnode &&
                   !pnode->parse_reclist) // &&
              if (pnode->dnres_pnode) {
                  goto rldnfreelabel;
              } else {
                  goto rlnextfreelabel;
              }
//        }
crecurfreelabel:
//            if (pnode->recup_pnode->recdn_pnode != pnode) {
//                free_parsereclist_lnode(gills, pnode);
                fpnode = pnode->recup_pnode;
                fpuppnode = pnode->up == pnode->recup_pnode ?
                pnode->up : NULL;
                if (pnode->parse_reclist ||
                    pnode->recdn_pnode)
                    return;
                if (fpnode->recdn_pnode &&
                    fpnode->recdn_pnode == pnode)
                    fpnode->recdn_pnode = NULL;
/*                else
                    free_parsereclist_lnode(gills, pnode);
*/
/*
                if (pnode->recur_free ||
                    pnode->uprecur_pnode->recur_fl_lasttk_pnode)
                    fpnode->recur_free = 1;
*/
#if 0
                recur_up_top = pnode->recur_up_top;
                recuruplist = pnode->recur_up_list; /* ? pnode->recur_up_list->next : NULL; */
#endif
/*
                if (pnode->recur_up_list) {
                    uprecurpnode = (parse_node_t *)(pnode->recur_up_list->next->data);
                    prevlistuppnode = uprecurpnode->rl_prev;
                } else {
                    uprecurpnode = prevlistuppnode = NULL;
                }
*/
                free_pnode(gills, pnode);
                pnode = fpnode;
                
#if 0
                if (!recpnode_pass && recuruplist/* !fpnode->recur_free */) {
                    if (pnode->parse_reclist)
                        return;
                    if (fpuppnode) {
                    if (!fpuppnode->recdn_pnode) {
                        fpuppnode->recres = fpuppnode->uprecres = 1;
                        if (fpuppnode->act)
                            fpuppnode->recact = 1;
#if 0
                        if (fpuppnode->up) {
                            if (fpuppnode->up->dnres_pnode) {
                                printf("ambiguous path at %s %d with "
                                "%s %d and %s %d ...exiting\n",
                                fpuppnode->up->token->token_name,
                                fpuppnode->up->token->token_num,
                                
#endif
#if 0
                     ret = generate_parse(gills, fpuppnode, /* (token_node_t *)(reclnode->next->data) */ fpuppnode->token, fpuppnode->rule_node, 0, NULL, NULL, NULL);
                     if (ret) {
                         printf("generate_parse %d failed ...exiting\n", ret);
//                         return ret;
                         exit_parse(gills, -1);
                     }
#endif                        
                    }
                        return;
                    } else {
                        if (!pnode->recdn_pnode)
                            pnode->up->recres = pnode->up->uprecres = 1;
                        return;
                    }
                 } else if (recpnode_pass || (!recuruplist && !recur_up_top)) {
#endif
                 if (recpnode_pass) { // || (!recuruplist && !recur_up_top)) {
 //                      pnode = fpnode;
 //                      pnode->recur_free = 1;
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
                     if (fpnode->parse_reclist || fpnode->recdn_pnode) // &&
 //                        fpnode->recres)
                         return;
                     while (1) {
                         if (fpnode->crecur) {
                             fpnode = fpnode->up;
                         } else if (fpnode->right_recur) {
                               fpnode = fpnode->rl_next;
 //                            while (fpnode->rl_prev)
 //                                fpnode = fpnode->rl_prev;
                         } else if (fpnode->recur || fpnode->int_recur) {
                             break;
                         }
 //                      if (pnode->parse_reclist ||
 //                         (pnode->recur && pnode->dnres_pnode)) ||
                     }
                     fpnode->recres = 1;
                     return;
                 }

#if 0
                if (!pnode->parse_reclist && !pnode->recdn_pnode /* && !pnode->res */) {
                    if (pnode->recur || pnode->right_recur ||
                        pnode->int_recur) { /* &&
                        recpnode_pass) { */
                        if (!recpnode_pass) {
//                            if (!pnode->parse_reclist) {

                                if (pnode->act)
                                    pnode->recact = 1;
                                pnode->recres = pnode->uprecres = 1;
//                            }
                            return;
                        }
//                        if (recpnode_pass) {
//                        recpnode_pass -= 1;
                        goto rldnfreelabel; /* rlprevfreelabel */;
//                        } else if (!recpnode_pass) {
                                 
//                        return;
                    }
//                    }
                    if (pnode->crecur /* && recpnode_pass */) {
//                        recpnode_pass -= 1;
//                        fpnode = pnode;
                        fpuppnode = fpnode->up;
                        if (!recpnode_pass) {
                            fuppnode->uprecres = 1;
                            return;
#if 0
                        while (1) {
                            if (fpnode == fpuppnode) {
                                fpnode->uprecres = 1;
                                return;
                            }
                            if (!fpnode->parse_reclist /* &&
                                fpnode->recup_pnode */ ) {
                                    fpnode = fpnode->recup_pnode;
                                    continue;
                            } else {
                                return;
                            }
                         }
#endif
                         } else {
                             goto rlnextfreelabel;
//                        goto crecurfreelabel;
                    }
//                    } else if (!recpnode_pass) {
//                        return;
//                    }
/*
                    fpnode = pnode->recup_pnode;
                    free_pnode(gills, pnode);
                    pnode = fpnode;
                    if (pnode->recur && !pnode->crecur) {
                       if (!pnode->parserec_pnode &&
                            pnode->parse_reclist) {
                            goto rlnextfreelabel;
                       } else if (pnode->crecur) {
                            return; //break;
                       }
                   } else if (pnode->crecur) {
                       goto crecurfreelabel;
//                       pnode->recdn_pnode = NULL;
//                       return; // goto rlnextfreelabel;
                   }
*/
               } //else {
               } else {
                   return; //break;
               }
/*            } else if (pnode->recup_pnode != pnode->up) {
                fpnode = pnode->recup_pnode;
                free_pnode(gills, pnode);
                fpnode->recdn_pnode = NULL;
                if (fpnode->parse_reclist)
                    return; //break;
                pnode = fpnode;
                if (pnode->recur || pnode->right_recur ||
                    pnode->int_recur)
                    goto rldnfreelabel;
                if (pnode->crecur)
                    goto crecurfreelabel;
//                goto rlnextfreelabel;
            } else {
                fpnode = pnode->up;
                free_pnode(gills, pnode);
                pnode = fpnode;
                if (pnode->parse_reclist)
                    return; //break;
                else
                    goto rlprevfreelabel;
            }
*/
#endif
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
 //                  return;
               }
           }

           lasttk_top_floating = 0;
           if (fpnode->res &&
               !fpnode->dnres_pnode) {
               if (fpnode->recres) {
                   if (fpnode->dn) {
                       pnode = fpnode->up;
//                       fpnode->recur_free = 0;
                       goto rlupresnulllabel;
                   } else {
                       goto rlnextfreelabel;
                   }
               } else {
                   pnode = fpnode->up;
 //                  fpnode->recur_free = 0;
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
//            if (prevpnode->rl_prev) {
              if (!prevpnode->rule_start) {
                pnode = prevpnode->lasttktotoprule_pnode;
                fpnode = prevpnode->rl_prev;
                fpnode->rl_next = prevpnode->rl_next;
                prevpnode->rl_next->rl_prev = fpnode;
                free_pnode(gills, prevpnode);
                gills->toppnode = NULL;
 //               fpnode->rl_next = NULL;
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

 //       if (prevpnode->rl_prev) {
        if (!prevpnode->rule_start) {
            updir = 0;
            fpnode = prevpnode->rl_prev;
/*        } else if (prevpnode->crecur) {
            pnode = prevpnode;
            goto crecurfreelabel; */
        } else {
            updir = 1;
            fpnode = prevpnode->up;
//            pnode = prevpnode;
//            goto rlupfreelabel;
        }

//        while (1) {
            if (fpnode->crecur) {
//                if (free_flag)
                    free_pnode(gills, prevpnode);
                    fpnode->rl_next = NULL;
//                else
//                    prevpnode->res = 0;
                pnode = fpnode;
                goto crecurfreelabel;
            }    
            if (!prevpnode->dn /* && !prevpnode->rec_prevup_list */) {
//                if (prevpnode->rl_prev) {
 //                   if (prevpnode->rl_prev &&
                    if (!prevpnode->rule_start &&
                        (fpnode->recur || fpnode->int_recur ||
                         fpnode->right_recur)) {
//                         fpnode->recur_free = 1;
                         if (!fpnode->parse_reclist &&
                             !fpnode->recdn_pnode)
                            recpnode_npass = 1;
                    }
/*
                    if (prevpnode->rl_prev &&
                        (fpnode->recur || fpnode->int_recur ||
                         fpnode->right_recur)) {
//                         fpnode->recur_free = 1;
                        recpnode_pass += 1;
                    }
*/
/*
                    if (prevpnode->crecur) {
                        pnode = prevpnode;
                        goto crecurfreelabel;
                    }
*/
#if 0
                    if (prevpnode->uprecur) {
                        if (prevpnode->rec_prevup_list) {
                            return;
                        } else {
                                if (prevpnode->rl_prev) {
                                    updir = 0;
                                    fpnode = prevpnode->rl_prev;
                                } else {
                                    updir = 1;
                                    fpnode = prevpnode->up;
                                }
                            if (!updir) {
                                free_parse_recpnode(gills, prevpnode, &recpnode_npass,
                    &recpnode_pass, updir);
                                fpnode->rl_next = NULL;
                                free_pnode(gills, prevpnode);
                                pnode = fpnode;
                            }
                            if (updir)
                                goto rlupfreelabel;
                            else
//                                goto rlprevfreelabel;
                                goto rlnextfreelabel;
                        }
                    }
                    if (prevpnode->recurup) {
/*
                        if (!prevpnode->up_recur_list) {
                            printf("parse failed with input unexpected token for expected tokens ...exiting\n");
                            exit_parse(gills, -1);
                        }
*/
                        if (/* prevpnode->rl_prev && */
                            prevpnode !=
                            prevpnode->rl_prev->rl_next) {
                            uprecur_prevpnode = prevpnode->rl_prev;
                                uprecur_prevpnode->rec_prevup_list = list_del_node(gills, uprecur_prevpnode->rec_prevup_list,
                                prevpnode->recprevup_lnode);
/*
                                if (!uprecur_prevpnode->rec_prevup_list) {
                                    free_pnode(gills, prevpnode);
                                    prevpnode = uprecur_prevpnode;
                                    goto rlprevfreelabel;
                            } else {
*/
                                free_parse_recpnode(gills, prevpnode, &recpnode_npass,
                    &recpnode_pass, updir);
//                                fpnode->rl_next = NULL;
                                free_pnode(gills, prevpnode);
//                                fpnode->rl_next = NULL;
                                if (uprecur_prevpnode->rl_next ||
                                    uprecur_prevpnode->rec_prevup_list) {
                                    return;
                                } else {
                                    pnode = uprecur_prevpnode;
                                    goto rlnextfreelabel;
                                }
//                            }
                         } else {
                                 prevpnode->rl_prev->rl_next = NULL;
//                                 free_pnode(gills, prevpnode);
                                 fpnode->rl_next = NULL;
                                     pnode = prevpnode->rl_prev;
                                free_parse_recpnode(gills, prevpnode, &recpnode_npass,
                    &recpnode_pass, updir);
                                     free_pnode(gills, prevpnode);
                                 if ( /* prevpnode->rl_prev */ fpnode->rec_prevup_list == NULL) {
/*
                                     pnode = prevpnode->rl_prev;
                                free_parse_recpnode(gills, prevpnode, &recpnode_npass,
                    &recpnode_pass, updir);
                                     free_pnode(gills, prevpnode);
*/
                                     goto rlnextfreelabel;
                                 } else {
/*
                                free_parse_recpnode(gills, prevpnode, &recpnode_npass,
                    &recpnode_pass, updir);
                                     free_pnode(gills, prevpnode);
*/
                                     return;
                                 }
/*
                                 prevpnode = fpnode;
                                 prevpnode->rl_next = NULL;
                                 if (!updir)
                                     continue;
                                 goto rlupfreelabel;
                             }
*/
                         }
                    }
#endif
#if 0
                    if (prevpnode->recur ||
                        prevpnode->int_recur ||
                        prevpnode->right_recur) {
//                        if (prevpnode->parse_reclist ||
//                            prevpnode->recdn_pnode) {
                            if (recpnode_pass)
                                recpnode_pass -= 1;
/*
                            free_pnode(gills, prevpnode);
                            fpnode->rl_next = NULL;
                            pnode = fpnode;
                            goto rlnextfreelabel;
*/
//                            goto recurfreelabel;
//                        } else {
                            if (!prevpnode->int_recur &&
                                !prevpnode->right_recur &&
                                !prevpnode->crecur &&
                                 prevpnode->recur &&
                                 prevpnode->recur_list)
                               free_recur_list(gills, prevpnode);
                            if (!updir) {
                            free_pnode(gills, prevpnode);
                            fpnode->rl_next = NULL;
                            pnode = fpnode;
//                            } else {
//                                pnode = prevpnode;
                            }
                            if (!updir)
//	                                goto rlprevfreelabel; //continue;
                                goto rlnextfreelabel;
                            else
                               goto rlupfreelabel;
//                        }
                    }
#endif
/*
                    if (prevpnode->crecur) {
                        pnode = prevpnode;
                        goto crecurfreelabel;
                    }
*/
//            }
//                    free_parse_recpnode(gills, prevpnode, &recpnode_pass, updir);
//                    free_pnode(gills, prevpnode);
//                    pnode = fpnode;
                            if (!updir) {
                                fpnode->rl_next = prevpnode->rl_next;
                                prevpnode->rl_next->rl_prev = fpnode;
                                free_parse_recpnode(gills, prevpnode, &recpnode_npass,
                    &recpnode_pass, updir);
 //                               fpnode->rl_next = NULL;
                                free_pnode(gills, prevpnode);
                                pnode = fpnode;
                            }
                            if (updir)
                                goto rlupfreelabel;
                            else
//                                goto rlprevfreelabel;
                                goto rlnextfreelabel;

/*
                    if (!updir) {
                        pnode->rl_next = NULL;
//                    goto rlprevfreelabel;
                        goto rlnextfreelabel;
                    }
*/
            } 
#if 0
else {
                if (!pnode->recur &&
                    !pnode->right_recur &&
                    !pnode->int_recur)
                    goto rldnfreelabel;
                if (pnode->recur || pnode->right_recur ||
                    pnode->int_recur) {
                      recpnode_pass += 1;
//                    if (pnode->parse_reclist ||
//                        pnode->recdn_pnode)
                      if (pnode->parse_reclist &&
                           pnode->recdn_pnode)
                        goto crecurfreelabel;
                      else
                        goto recurfreelabel;
//                    else {
                        if (pnode->recur_free)
                            goto rldnfreelabel;
                        else
                            return;
                    }
                 }
            }
#endif
//       }
/*
            recuppnode = pnode->recup_pnode;
            recuppnode->recdn_pnode = NULL;
#ifdef REDUCE_ACTION_INLINE
            reduce_recur_parse(recuppnode);
#endif
*/
//            free_pnode(gills, pnode);
//            return;
//}

#if 0
/*            
            goto crecurfreelabel;
                recuppnode = pnode->recup_pnode;
                recuppnode->recdn_pnode = NULL;
                free(pnode);
                return 0;
                if (recuppnode->crecur) {
                    pnode = recuppnode;
                    if (recfree_idx) {
                        goto crecurfreelabel;
                    } else {
                        goto rlnextstacklabel;
                    }
                } else if (recuppnode->recur) {
                    uppnode = pnode->up;
                    if (!uppnode && pnode->node->token_num != 0)
                        return -1;
                    if (recfree_idx)
                        recfree_idx -= 1;
                    pnode = uppnode;
                    if (!pnode->rl_prev)
                        goto rlupfreelabel;
                    else
                        goto rlprevfreelabel;
                }
*/
        } else if (!pnode->dn) {
//                uppnode = pnode->up;
/*
                if (!uppnode && pnode->node->token_num == 0)
                    return -1;
                else if (!uppnode && pnode->node->token_num != 0)
                    return -2;
                    uppnode->dn = list_del_node(gills, uppnode->dn, pnode->updn_lnode);
                    free_pnode(gills, pnode);
                    pnode = uppnode;
*/
                if (!pnode->dn && !pnode->recur) {
                    if (!pnode->rl_prev)
                        goto rlupfreelabel;
                    else
                        goto rlprevfreelabel;
                } else if (!pnode->dn && pnode->recur) {
                    goto recurfreelabel;
                } else if (pnode->dn) {
                    goto rlnextstacklabel;
                }
            }
#endif
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
                     
/*
                fpnode = uppnode;
                if (!fpnode->rl_next &&
                    !fpnode->rl_prev &&
                    ((!fpnode->token->recur) ||
                     (!fpnode->recdn_pnode &&
                      !fpnode->parse_reclist))) {
nterm : nterm1 nterm2(res tree) nontop nterm3
nontop : a
a : b
b : c
d : e
f : g
g : nonterm1 nonterm2
top : a
a : b
b : c
d : e
f : g
g : nonterm1 nonterm2

                     fpnode->res = 0;
                     if (fpnode->up) {
                         pnode = fpnode->up;
                         goto rlupfreelabel;
                     } else {
                         tk_num = pnode->token->token_num;
                         if (fpnode->rl_prev) {
                             goto rlprevfreelabel;
                         } else if (tk_num == 0) {
                             gills->retfree = -1;
                                 free_pnode(gills, pnode);
                             return;
                         } else if (tk_num != 0) {
                             gills->retfree = -2;
                                 free_pnode(gills, pnode);
                             return;
                         }
                      }
                }
*/
/*
                if (!uppnode && pnode->rl_prev) {
//                    goto rlprevfreelabel;
                    goto rlnextfreelabel;
                }
                if (!uppnode && pnode->token->token_num == 0) {
                    gills->retfree = -1;
                    free_pnode(gills, pnode);
                    gills->toppnode = NULL;
                    return; // -1;
                } else if (!uppnode && pnode->token->token_num != 0) {
                    gills->retfree = -2;
                    free_pnode(gills, pnode);
                    return; // -2;
                }
//                    fpnode = pnode;
//                    uppnode->dn = list_del_node(gills, uppnode->dn, pnode->updn_lnode);
//                    free_pnode(gills, pnode);
//                    pnode = uppnode;
*/
                if (uppnode->res || uppnode->dnres_pnode) {
                    if (pnode == uppnode->dnres_pnode) {
                        uppnode->dnres_pnode = NULL;
                        pnode->res = 0;
                        free_parse_recpnode(gills, pnode,
                        &recpnode_npass,
                        &recpnode_pass, updir);
                        /* if (free_flag) */ free_pnode(gills, pnode);
                        if (uppnode->dn) {
//                            while (1) {
                                
//                            while (1) {
//                                if (pnode->rl_next)
//                                    return;
//                                if (pnode
                           pnode = uppnode;
//                           return;
/*
                           if (!pnode->rl_prev &&
                               !pnode->rl_next) {
                               free_flag = 0;
                               goto rlupfreelabel;
                           }
                           goto rlnextfreelabel;
//                           return; //break;
*/
//                        } /* else if (pnode->crecur) {
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
//                                  if (fpnode->rl_next || fpnode->rl_prev) {
                                  if (!fpnode->rule_start || !fpnode->rule_end) {
                                      fpnode->res = fpnode->recres = 0;
                                      if (fpnode->dn)
                                          return;
                                      else
                                          break;
                                  } else if (fpnode->recur &&
                                            fpnode->token->crecur &&
                                           !fpnode->act &&
                                           !fpnode->dnres_pnode)
                                      fpnode->res = 0;
                                  else if (fpnode->recur && 
                                           !fpnode->token->crecur &&
                                           !fpnode->dnres_pnode)
                                      fpnode->res = 0;
                                  else /* if (!fpnode->recur) */
                                      fpnode->res = 0;

//                                  if (!fpnode->dnres_pnode->recur) {
                                  if (!free_flag && fpnode->dnres_pnode) {
                                  fpnode->dn = list_add(gills, fpnode->dn,
                                               fpnode->dnres_pnode, &lnode);
                                  fpnode->dnres_pnode->updn_lnode = lnode;
                                  fpnode->dnres_pnode = NULL;
                                  }
  //                                }
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
                        if (free_flag) free_pnode(gills, pnode);
                                  fpnode = fpuppnode; // fpnode->up;
                        if (topfreed) 
                            return;
 //                             }
/*
else {
                                   fpnode->res = 0;
                                  fpnode->dn = list_add(gills, fpnode->dn,
                                               fpnode->dnres_pnode, &lnode);
                                  fpnode->dnres_pnode->updn_lnode = lnode;
                                  fpnode->dnres_pnode = NULL;
                                   if (fpnode->dn) {
                                       return;
                                   } else {
                                       pnode = fpnode;
                                       goto rlnextfreelabel;
                                   }
                                   break;
                              }
*/
 //                         }
                          }
                          return;
                       } else {
                            pnode = uppnode;
/*
                           if (!pnode->rl_prev &&
                               !pnode->rl_next)
                               goto rlupfreelabel;
*/
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
/*
                if (uppnode->dn) {
                    return; //break;
                } else {
                    if (uppnode->recur)
                        recpnode_pass -= 1;
                    free_pnode(gills, pnode);
                    pnode = uppnode;
                    goto rlnextfreelabel;
                }
*/
//                if (pnode->recur && !pnode->res)
//                    return; // 0;
#if 0
                if (pnode->dn == pnode->dn->next &&
                    pnode->res) {
#ifdef ACTION_REDUCE_INLINE
                    tpnode = pnode->dn_respnode;
                    while (tpnode->rl_next)
                        tpnode = tpnode->rl_next;
                    action_ex_inline(gills, tpnode, 0); 
#else
                    return 0;
#endif

                }
#endif
//        }
#endif
    return;
}
//}
//#endif

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
//    gills->tkstack_idx += 1;
    return 0;
}

int parse (gills_context_t *gills, token_node_t *starttoken)
{
    int tk, tkread = 0, ret, ambiguity, top_reached, i, nonempty_tkcount, loop;
    parse_node_t *pnode, *tkpnode, *tkuppnode, *ltoppnode, *retpnode, *listpnode, *crecpnode, *crecuppnode;
    token_node_t *tknode;
    list_t *tklist, *tknelist, *listnode, *prevlistnode, *toplasttk_list, *listmesh_node, *nextop_list;
    GILLSYYSTYPE *valptr;
    struct timespec timespecstart, timespecend;

//reparselabel:
//    gills->tkstack_idx++;
//    tk = get_token(gills, gills->scan_active_idx);
/*
    ret = clock_gettime(CLOCK_REALTIME, &timespecstart);
    if (ret) {
        perror("clock_gettime failed: ");
        exit(-1);
    }
*/
reparse_genlabel:
    gills->retfree = 0;
    ret = generate_parse(gills, NULL, starttoken, NULL, 0, NULL, &retpnode, NULL);
//    gills->tkstack_idx += 1;
#ifdef GILLS_MEM_TKSTACK_UNLIM
    gills->prevop_tkstack_listmesh_list = gills->nextop_tkstack_listmesh_list;
    gills->nextop_tkstack_listmesh_list = NULL;
#endif
    gills->toppnode = retpnode;
    if (tkread) {
        tkread = 0;
        goto reparsenontklabel;
    }
/*
    ret = clock_gettime(CLOCK_REALTIME, &timespecstart);
    if (ret) {
        perror("clock_gettime failed: ");
        exit(-1);
    }
*/
//    while (1) {
reparselabel:
    tk = get_token(gills, gills->scan_active_idx);
    if (tk == GILLS_SCANNER_ERROR) {
        printf("scanner error ...exiting\n");
        exit(-1);
    }
//    tk = yylex(gills->yylval, gills->yyscan)

reparsenontklabel:
    gills->retfree = 0;
//    if (tk == EOF)
//        goto free_pnode_label;

//reparselabel:
//    gills->topreach = 0;
/*
    gills->tkstack_idx += 1;
    tklist = gills->tkstack[gills->tkstack_idx - 1]->next;
    while (tklist) {
        tkpnode = (parse_node_t *)tklist->data;
        tknode = tkpnode->node;
        if (tknode->token_num == tk) {
#ifdef TOKEN_MEM_INTERNAL
            valptr = (void *)calloc(1, sizeof(GILLSYYSTYPE));
            if (valptr) {
                printf("valptr calloc failed ...exiting\n");
                exit(-1);
            }
            memcpy(valptr, gills->scan_funcs_val[gills->scan_active_idx]->str),
                   gills->scan_val_size[gills->scan_active_idx]);
            tkpnode->val = valptr;
#else
            tkpnode->val = gills->scan_funcs_val[gills->scan_active_idx];
#endif 
           tkpnode->res = 1;
           ret = reduce_parse(gills, tkpnode);
           if (ret) {
               printf("error ...exiting\n");
               exit_parse(gills, -1);
           }
        tklist = tklist->next;
        if (tklist == gills->tkstack[gills->tkstack_idx - 1]) {
            if (gills->tkstack[gills->tkstack_idx]) {
                gills->tkstack_idx += 1;
                tklist = gills->tkstack[gills->tkstack_idx - 1]->next;
            } else {
                break;
            }
        }
    }
*/
//free_pnode_label:
/*
    if (!gills->topreach && tk != EOF) {
        i = gills->tkstack_idx; // == 0 ? 0 : gills->tkstack_idx - 1;
    } else {
        i = 0;
    }
*/
    gills->prevop_tkstack_update = 0;
#ifndef GILLS_MEM_TKSTACK_UNLIM
    tklist = gills->tkstack[gills->tkstack_idx];
    gills->prevop_tkstack_lnode = gills->tkstack[gills->tkstack_idx] ?
                                  gills->tkstack[gills->tkstack_idx]->next :
                                  NULL;
 //   gills->prevop_tkstack_update = 0;
#else
    if (gills->tkstack_idx) {
        gills->prevop_tkstack_listmesh_list = gills->nextop_tkstack_listmesh_list;
    }
    listmesh_node = gills->prevop_tkstack_listmesh_list;
    gills->prevop_tkstack_lnode = listmesh_node ? listmesh_node->next : NULL;
#endif
    while (1/* tklist */ ) {
/*
        tklist = tklist->next;
        if (tklist == gills->tkstack[gills->tkstack_idx])
            break;
*/
#ifndef GILLS_MEM_TKSTACK_UNLIM
 //       if (!gills->tkstack[gills->tkstack_idx])
 //           break;
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == gills->tkstack[gills->tkstack_idx])
            break;
#else
 //       if (!listmesh_node)
 //           break;
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == listmesh_node)
            break;
#endif
/*
        tkpnode = (parse_node_t *)tklist->data;
        tknode = tkpnode->token;
*/
        tkpnode = (parse_node_t *)(gills->prevop_tkstack_lnode->data);
        if (tkpnode->token->token_num == gills->empty_tk_num) {
            tkpnode->res = 1;
            tkpnode->val = NULL;
           ret = reduce_parse(gills, tkpnode);
           if (ret) {
               printf("error ...exiting\n");
               exit_parse(gills, -1);
           }
           if (!gills->prevop_tkstack_update) {
               gills->prevop_tkstack_lnode = gills->prevop_tkstack_lnode->next;
           } else {
               gills->prevop_tkstack_update = 0;
           }
       } else {
           gills->prevop_tkstack_lnode = gills->prevop_tkstack_lnode->next;
       }
    }
 //       gills->prevop_tkstack_lnode = NULL;
        gills->prevop_tkstack_update = 0;

//             listnode = gills->tkstack[gills->tkstack_idx];
        gills->top_free = 0;
 //       gills->prevop_tkstack_lnode = gills->tkstack[gills->tkstack_idx] ?
 //                                     gills->tkstack[gills->tkstack_idx]->next : NULL;
#ifndef GILLS_MEM_TKSTACK_UNLIM
    tklist = gills->tkstack[gills->tkstack_idx];
    gills->prevop_tkstack_lnode = gills->tkstack[gills->tkstack_idx] ?
                                  gills->tkstack[gills->tkstack_idx]->next :
                                  NULL;
  //  gills->prevop_tkstack_update = 0;
#else
    listmesh_node = gills->prevop_tkstack_listmesh_list;
    gills->prevop_tkstack_lnode = listmesh_node ? listmesh_node->next : NULL;
#endif
        while (1) {
/*
            if (!gills->tkstack[gills->tkstack_idx])
                break;
            if (gills->prevop_tkstack_lnode == gills->tkstack[gills->tkstack_idx])
                break;
*/
#ifndef GILLS_MEM_TKSTACK_UNLIM
 //       if (!gills->tkstack[gills->tkstack_idx])
 //           break;
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == gills->tkstack[gills->tkstack_idx])
            break;
#else
 //       if (!listmesh_node)
 //           break;
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == listmesh_node)
            break;
#endif
/*
            prevlistnode = listnode;
            listnode = listnode->next;
            if (listnode == gills->tkstack[gills->tkstack_idx]) {
//                free_list(listnode);
//                gills->tkstack[i] = NULL;
                break;
            }
            pnode = (parse_node_t *)listnode->data;
*/
            pnode = (parse_node_t *)(gills->prevop_tkstack_lnode->data);
//            if (!gills->topreach) {
                if (pnode->token->token_num == gills->empty_tk_num ||
                    pnode->token->token_num == tk) {
                    gills->prevop_tkstack_lnode = gills->prevop_tkstack_lnode->next;
                    continue;
                }
                if (pnode->token->token_num != tk) {
                    /* ret = */free_parse_nodes(gills, pnode);
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
/*
                    if (!gills->tkstack[gills->tkstack_idx])
                        break;
                    listnode = prevlistnode;
*/
//                    if (ret)
//                        gills->retfree = ret;
                }
           /* } else {
#ifndef REDUCE_ACTION_INLINE
                free_parse_nodes(gills, pnode);
#endif
            } */
        }
//    }
/*
      if (!gills->topreach && !gills->tkstack[gills->tkstack_idx]) {
                        if (tk != -1) {
                            printf("parse failed with input unexpected token for expected tokens ...exiting\n");
                        } else {
                            printf("parse failed with unexpected end of input tokens(EOF) ...exiting\n");
                        }
                        exit_parse(gills, -1);
      }             
*/

 //   if (!gills->topreach && gills->tkstack[gills->tkstack_idx] == NULL) {
    if (!gills->topreach &&
#ifndef GILLS_MEM_TKSTACK_UNLIM
        !gills->tkstack[gills->tkstack_idx]
#else
        !gills->prevop_tkstack_listmesh_list
#endif
// !gills->prevop_tkstack_lnode) {
      ) {
           printf("unexpected token %d parse terminated ...exiting\n", tk);
           exit_parse(gills, -1);
       }

#ifndef GILLS_MEM_TKSTACK_UNLIM
    gills->tkstack_idx += 1;
#else
    if (gills->tkstack_idx) {
        gills->prevop_tkstack_listmesh_list = gills->nextop_tkstack_listmesh_list;
        gills->nextop_tkstack_listmesh_list = NULL;
    }
    gills->tkstack_idx += 1;
#endif

#if 0
#ifndef GILLS_MEM_TKSTACK_UNLIM
    gills->tkstack_idx += 1;
    if (!gills->tkstack[gills->tkstack_idx - 1] && gills->topreach) {
        tkread = 1;
        goto topreachlabel;
     }
#else
    if (!gills->prevop_tkstack_lnode && gills->topreach) {
        tkread = 1;
        goto topreachlabel;
    }
#endif
#endif

    gills->prevop_tkstack_update = 0;
#ifndef GILLS_MEM_TKSTACK_UNLIM
    tklist = gills->tkstack[gills->tkstack_idx];
    gills->prevop_tkstack_lnode = gills->tkstack[gills->tkstack_idx - 1] ?
                                  gills->tkstack[gills->tkstack_idx - 1]->next :
                                  NULL;
  //  gills->prevop_tkstack_update = 0;
#else
    listmesh_node = gills->prevop_tkstack_listmesh_list;
    gills->prevop_tkstack_lnode = listmesh_node ? listmesh_node->next : NULL;
#endif
    while (1 /* tklist */ ) {
/*
        tklist = tklist->next;
        if (tklist == gills->tkstack[gills->tkstack_idx - 1])
            break;
        tkpnode = (parse_node_t *)tklist->data;
        tknode = tkpnode->token;
*/
/*
        if (!gills->tkstack[gills->tkstack_idx - 1])
            break;
        if (gills->prevop_tkstack_lnode == gills->tkstack[gills->tkstack_idx - 1])
            break;
*/

#ifndef GILLS_MEM_TKSTACK_UNLIM
 //       if (!gills->tkstack[gills->tkstack_idx])
 //           break;
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == gills->tkstack[gills->tkstack_idx - 1])
            break;
#else
 //       if (!listmesh_node)
 //           break;
        if (!gills->prevop_tkstack_lnode)
              break;
        if (gills->prevop_tkstack_lnode == listmesh_node)
            break;
#endif
        tkpnode = (parse_node_t *)(gills->prevop_tkstack_lnode->data);
        if (tkpnode->token->token_num == tk) {
           tkpnode->res = 1;
 //          if (gills->prevop_tkstack_lnode ==
 //              gills->prevop_tkstack_lnode->next->next)
 //              gills->prevop_tkstack_list_res = 1;
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
//               toppnodelist = tkpnode->lasttk_toppnode->toplist_lnode;
               ltoppnode = tkpnode->lasttk_toppnode;
               if (!ltoppnode->res) {
                   pnode->lasttk_toppnode->top_lasttk_pnode_list = 
                             list_del_node(gills, toplasttk_list,
                             pnode->updn_lnode);
/*
                   if (!ltoppnode->top_lasttk_pnode_list &&
                       !ltoppnode->dn && !ltoppnode->dnres_pnode) {
                            printf("parse failed with unexpected end of input tokens(EOF) ...exiting\n");
                            exit_parse(gills, -1);
                   }
*/
               }

               while (1) {
 //                  if (!ltoppnode->rl_prev)
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
/*
               if (!tkpnode->rl_prev->rl_next) {
                   tkpnode->rl_prev->rl_next = tkpnode;
                   continue;
               } else {
                   printf("top node input multiple resolved ...exiting\n"); //not to expect
                   exit_parse(gills, -1);
               }
*/
 //              gills->prevop_tkstack_lnode = gills->prevop_tkstack_lnode->next;
           } else {
#ifdef TOKEN_MEM_INTERNAL

            valptr = (GILLSYYSTYPE *)calloc(1, sizeof(GILLSYYSTYPE));
            if (!valptr) {
                printf("valptr calloc failed ...exiting\n");
                exit(-1);
            }
            strcpy(valptr->str, ((YYSTYPE *)(gills->scan_funcs_val[gills->scan_active_idx]))->str);

/*
            valptr->str[0] = ((YYSTYPE *)(gills->scan_funcs_val[gills->scan_active_idx]))->str[0];
            valptr->str[1] = ((YYSTYPE *)(gills->scan_funcs_val[gills->scan_active_idx]))->str[1];
*/
            tkpnode->val = (void *)valptr;
/*
            valptr = (void *)calloc(1, gills->scan_val_size[gills->scan_active_idx]);
            memcpy(valptr, gills->scan_funcs_val[gills->scan_active_idx],
                   gills->scan_val_size[gills->scan_active_idx]);
            tkpnode->val = valptr;
*/
#else
            tkpnode->val = gills->scan_funcs_val[gills->scan_active_idx];
#endif 
           ret = reduce_parse(gills, tkpnode);
           if (ret) {
               printf("error ...exiting\n");
               exit_parse(gills, -1);
           }
           }
/*
           if (gills->prevop_tkstack_update) {
               gills->prevop_tkstack_update = 0;
           } else {
               gills->prevop_tkstack_lnode = gills->prevop_tkstack_lnode->next;
           }
*/
        }
/*        tklist = tklist->next;
        if (tklist == gills->tkstack[gills->tkstack_idx - 1]) {
            if (gills->tkstack[gills->tkstack_idx]) {
                gills->tkstack_idx += 1;
                tklist = gills->tkstack[gills->tkstack_idx - 1]->next;
             } else {
                break;
            }
        } */
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

    if (gills->prevop_tkstack_lnode) {
        if (gills->prevop_tkstack_lnode == 
            gills->prevop_tkstack_lnode->next->next) {
 //           tkpnode = (parse_node_t *)(gills->prevop_tkstack_lnode->next->data);
              tkpnode = (parse_node_t *)(gills->prevop_tkstack_lnode->next->data);
              pnode = (parse_node_t *)(gills->prevact_tkstack_list->next->data);
            if (tkpnode->res) {
                if (tkpnode->token->token_num == gills->last_tk_num) {
                    if (!nextop_list) {
 //                       gills->topres = 1;
#ifndef GILLS_MEM_TKSTACK_UNLIM

#ifndef REDUCE_ACTION_INLINE
                        gills->action = 1;
                        action_ex_inline(gills, (parse_node_t *)(gills->tkstack[0]->next->data), NULL);
                        gills->action = 0;
                        
#else
 //                       if (gills->prevact_tkstack_list->next->next ==
 //                           gills->prevact_tkstack_list) {
                            gills->action = 1;
                            action_ex_inline(gills, pnode, NULL);
                            gills->action = 0;
 //                           free_pnode(gills, gills->toppnode);
 //                       }
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
#if 0
#ifndef REDUCE_ACTION_INLINE
    top_reached = 1;
#ifndef GILLS_MEM_TKSTACK_UNLIM
    for (i = 0; i <= gills->tkstack_idx; i++) {
         tklist = gills->tkstack[i] ? gills->tkstack[i]->next : gills->tkstack[i];
#else
    listmesh_node = gills->tkstack_listmesh ? gills->tkstack_listmesh->next : NULL;
    while (1) {
        if (!listmesh_node || listmesh_node == gills->tkstack_listmesh)
            break;
        tklist = (list_t *)(listmesh_node->data);
        
#endif
// #define REDUCE_ACTION_INLINE   1
//#if 0 
//REDUCE_ACTION_INLINE
         if ( /* ((i == gills->tkstack_idx - 1) ||
             (i == gills->tkstack_idx)) && */
             !tklist) {
             if (tk == EOF) {
                 printf("parse failed with unexpected end of input tokens(EOF) ...exiting\n");
                 exit_parse(gills, -1);
             } else {
                  printf("parse failed with input unexpected token for expected tokens ...exiting\n");
                 exit_parse(gills, -1);
             }
         }
/*
         if (tklist) {
             top_reached = 0;
             break;
         }
*/
//#endif
//else
//         if ( /* !tklist || */ tklist != tklist->next->next) {
             nonempty_tkcount = 0;
             tknelist = NULL;
             while (1) {
                 tkpnode = (parse_node_t *)(tklist->data);
                 if (tkpnode->token->token_num != gills->empty_tk_num) {
                     if (!nonempty_tkcount) {
                         tknelist = tklist;
                         nonempty_tkcount += 1;
                     } else {
                        top_reached = 0;
                        break;
                     }
                }
                tklist = tklist->next;
                if (tklist == gills->tkstack[i])
                    break;
            }
            if (!top_reached)
                break;
 //       }
         tklist = tknelist;
         tkpnode = (parse_node_t *)(tklist->data);
         if (!tkpnode->res) {
             top_reached = 0;
             break;
         }
//         if (!tklist) {
//             if ( /* (i + 1 == gills->tkstack_idx) || */
//                 (i == gills->tkstack_idx)) {
                 if (!tklist/* gills->topreach && !gills->tkstack[gills->tkstack_idx] */) {
//                     tkread = 1;
//                     break;
//                 } else {
                     if ( i < gills->tkstack_idx) {
                         printf("unexpected token %d ...exiting\n", tk);
                        exit_parse(gills, -1);
                     }
                  else {
//             } /* else {
                 printf("unexpected end of expected token list ...exiting\n");
                 exit_parse(gills, -1);
             }
         }
         if (tkpnode->token->token_num == gills->last_tk_num) {
             break;
         }
#if 0
         if ((i == gills->tkstack_idx) &&
             (tkpnode->token->token_num != gills->last_tk_num) /* ||
              ((tkpnode->rl_prev) &&
             (tkpnode->rl_prev->rl_next != tkpnode)) */) {
              top_reached = 0;
              break; //continue;
         }
#endif
//#endif
    }
#endif
/*
    if (top_reached == 0) {
        gills->topreach = 0;
    } else {
        gills->topreach = 1;
    }
*/
//    if (i == gills->tkstack_idx + 1) {
//        gills->topreach = 1;
//    }
//    for (; i <= gills->tkstack_idx; i++) {
/*
        if (!gills->topreach && !gills->tkstack[gills->tkstack_idx - 1]) {
            printf("error null terminal token list generated from last parse ...exiting\n");
            exit_parse(gills, -1);
        }
*/
//        else
/*
    if (ambiguity) {
    }
*/
#endif
    if (top_reached) {
/*
    ret = clock_gettime(CLOCK_REALTIME, &timespecend);
    if (ret) {
        perror("clock_gettime end failed: ");
        exit(-1);
    }

    printf("\nstart timespec : %d %d \n", timespecstart.tv_sec, timespecstart.tv_nsec);
    printf("end timespec : %d %d \n", timespecend.tv_sec, timespecend.tv_nsec);
    printf("diff : %d sec %d nsec\n", timespecend.tv_sec - timespecstart.tv_sec,
            timespecend.tv_nsec - timespecstart.tv_nsec);
*/
    }

#if 0
#ifndef REDUCE_ACTION_INLINE
//    if (top_reached /* && !ambiguity */) {
    if (gills->topreach && gills->prevop_tkstack_list_res == 1) {
/*
    ret = clock_gettime(CLOCK_REALTIME, &timespecstart);
    if (ret) {
        perror("clock_gettime failed: ");
        exit(-1);
    }
*/
//        action_parse(gills, gills->toppnode);
        action_ex_inline(gills, (parse_node_t *)(gills->tkstack[0]->next->data), NULL);
/*
    ret = clock_gettime(CLOCK_REALTIME, &timespecend);
    if (ret) {
        perror("clock_gettime end failed: ");
        exit(-1);
    }
    printf("\nstart timespec : %d %d \n", timespecstart.tv_sec, timespecstart.tv_nsec);
    printf("end timespec : %d %d \n", timespecend.tv_sec, timespecend.tv_nsec);
    printf("diff : %d sec %d nsec\n", timespecend.tv_sec - timespecstart.tv_sec,
            timespecend.tv_nsec - timespecstart.tv_nsec);
*/
    }

#endif
#endif
#if 0
#ifdef REDUCE_ACTION_INLINE
 //   for (i = 0; i <= gills->tkstack_idx; i++) {
#ifndef GILLS_MEM_TKSTACK_UNLIM
    for (i = 0; i <= gills->tkstack_idx; i++) {
         listnode = gills->tkstack[i];
         tklist = gills->tkstack[i] ? gills->tkstack[i]->next : gills->tkstack[i];
#else
    listmesh_node = gills->tkstack_listmesh ? gills->tkstack_listmesh->next : NULL;
    loop = 1;
    while (loop) {
        if (!listmesh_node || listmesh_node == gills->tkstack_listmesh)
            break;
        listnode = (list_t *)(listmesh_node->data);
        while (1) {
#endif
         if (!listnode || !listnode->next ||
             listnode->next->next != listnode)
             continue;
//         if (listnode->act)
//             continue;
         listpnode = (parse_node_t *)listnode->next->data;
         if (!listpnode->res) {
             loop = 0;
             break;
         }
 //            continue;
//         if (listpnode->act)
//             continue;
         if (listpnode->act)
             continue;
/*
         if (listpnode->gen_recur_pnode &&
             listpnode->gen_recur_pnode->crecur &&
             listpnode->gen_recur_pnode->assoc == RIGHT_ASSOC) {
                 if (!listpnode->gen_recur_pnode->rec_to_reduce)
                     continue;
         }
*/
         if (listpnode->token->token_num == gills->last_tk_num) {
//             free_parse_nodes(gills, listpnode);
             toplasttk_list = listpnode->lasttk_toppnode->top_lasttk_pnode_list;
             listpnode->lasttk_toppnode->top_lasttk_pnode_list = 
               list_del_node(gills, toplasttk_list,
                             listpnode->updn_lnode);
//            /* if (free_flag) */ free_pnode(gills, pnode);

             if (listpnode->lasttktotoprule_pnode->token)
                 free_parse_nodes(gills, listpnode->lasttktotoprule_pnode);
             if (listpnode->token)
                 free_pnode(gills, listpnode);
             if (gills->toppnode)
                 free_pnode(gills, gills->toppnode);
             gills->toppnode = NULL;
             return 0;
//             continue;
         }
         action_ex_inline(gills, listpnode, 0);
         if (ret) {
             return -1;
         }
    }
#ifdef GILLS_MEM_TKSTACK_UNLIM
    }
#endif
#endif
#endif
#if 0
#ifdef REDUCE_ACTION_INLINE
    top_reached = 1;
    for (i = 0; i <= gills->tkstack_idx; i++) {
         tklist = gills->tkstack[i] ? gills->tkstack[i]->next : gills->tkstack[i];
         if (tklist) {
             top_reached = 0;
             break;
         }
    }
#endif
#endif
check_reparse:
    top_reached = (gills->topreach && gills->prevop_tkstack_list_res == 1);
    if (top_reached && tk != EOF) {
        printf("parse failed with input unexpected token for expected tokens (EOF) ...exiting\n");
        exit_parse(gills, -1);
    }
    if ((!top_reached ||
         (gills->toppnode && !gills->toppnode->res)) && tk == EOF) {
/*
        if (gills->toppnode) {
            free_pnode(gills, gills->toppnode);
            gills->toppnode = NULL;
        }
*/
                 printf("parse failed with unexpected end of input tokens(EOF) ...exiting\n");
                        exit_parse(gills, -1);
/*
        if (gills->retfree == -1) {
            printf("unexpected eof ...exiting\n");
            return UNEXPECTED_EOF;
        }
        if (gills->retfree == -2) {
            printf("unexpected token %d ...exiting\n");
            return UNEXPECTED_TOKEN;
        }
*/
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
        gills->pnodemem_idx = 0;
        gills->pnodeptrs_start = 0;
        gills->pnodeptrs_end = 0;
        goto reparse_genlabel;
    }

    if (!top_reached) {
        goto reparselabel;
    }
//    if (gills->topreach && !ambiguity && tk != EOF)
//        goto reparse;
    if (gills->topreach && tk == EOF)
        return 0;
//    }
}

