#ifndef __GILLS_INTERNAL_H__
#define __GILLS_INTERNAL_H__

list_t* list_add(gills_context_t *gills, list_t *list, void *data, list_t **listnode);
void free_list(list_t *list);
list_t* list_del_node (gills_context_t *gills, list_t *list, list_t *node);
list_t* list_del_head (gills_context_t *gills, list_t *list);
void list_mesh_del_node(gills_context_t *gills,
                         list_t **llist, list_t *llist_node, list_t **list,
                         list_t *list_node);
void list_mesh_add(gills_context_t *gills,
                    list_t **llist, list_t **list,
                    void *data, list_t **llnode, list_t **lnode);
int read_parse(gills_context_t *gills,
               token_read_t *tkread, int tokens_num,
               char **tkleft_assoc, int tkleftassoc_num,
               char **tkright_assoc, int tkrtassoc_num,
               char **tkprec, int tkprec_num);

mem_pool_t* mem_pool_alloc(gills_context_t *gills,int node_size, int nodes_num,
                           int segments_num);
void* mem_pool_get_node(gills_context_t *gills, mem_pool_t *mempool);
void mem_pool_node_free(gills_context_t *gills,
                     mem_pool_t *mempool, void *node);

#endif    /* __GILLS_INTERNAL_H__ */
