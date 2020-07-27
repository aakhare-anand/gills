#ifndef __GILLS_INTERNAL_H__
#define __GILLS_INTERNAL_H__

list_t* list_add(gills_context_t *gills, list_t *list, void *data, list_t **listnode);
void free_list(list_t *list);
list_t* list_del_node (list_t *list, list_t *node);
list_t* list_del_head (list_t *list);
int read_parse(gills_context_t *gills,
               token_read_t *tkread, int tokens_num,
               char **tkleft_assoc, int tkleftassoc_num,
               char **tkright_assoc, int tkrtassoc_num,
               char **tkprec, int tkprec_num);

#endif    /* __GILLS_INTERNAL_H__ */
