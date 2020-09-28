#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__

#include "gills.h"
#include "gills_internal.h"

mem_segment_t* mem_segment_alloc (gills_context_t *gills, mem_pool_t *mempool)
{
     mem_segment_t *seg;

     if (mempool->segments_num == mempool->segments_max_num ||
         mempool->segments_max_num != -1) {
         return NULL;
     }

     seg = (mem_segment_t *)calloc(1, sizeof(mem_segment_t));
     if (!seg) {
         printf("calloc failed ...exiting\n");
         return NULL;
     }
     seg->mem = (void *)calloc(mempool->nodes_max_num,
                                       (mempool->node_size + SEG_LNODE_SIZE));
     if (!seg->mem) {
         printf("calloc failed ...exiting\n");
         free(seg);
         return NULL;
     }

     seg->nodes_ptrs = (void *)calloc(mempool->nodes_num,
                                      sizeof(list_t *));
     if (!seg->nodes_ptrs) {
         printf("calloc failed ...exiting\n");
         free(seg);
         return NULL;
     }
    mempool->segments_list = list_add(gills, mempool->segments_list,
                                      (void *)seg, &seg->seglist_lnode);
    mempool->segments_num += 1;
    mempool->nodes_num += mempool->nodes_max_num;

     return seg;
}


mem_pool_t* mem_pool_alloc (gills_context_t *gills,
                            int node_size, int nodes_max_num,
                            int segs_max_num)
{
    mem_pool_t *mempool;
    mem_segment_t *memseg;

    mempool = (mem_pool_t *)calloc(1, sizeof(mem_pool_t));
    if (!mempool) {
        printf("calloc failed ...exiting\n");
        return NULL;
    }
    mempool->node_size = node_size;
    mempool->nodes_max_num = nodes_max_num;
    mempool->segments_max_num = segs_max_num;

    memseg = mem_segment_alloc(gills, mempool);
    if (!memseg)
        return NULL;
    return mempool;
}

void* mem_pool_segment_get_node (gills_context_t *gills,
                                 mem_pool_t *mempool,
                                 mem_segment_t *memseg,
                                 list_t *seg_lnode)
{
    char *ptr;

    if (memseg->nodes_in_use_num == mempool->nodes_max_num) {
        return NULL;
    }

    if (memseg->mem_idx < mempool->nodes_max_num) {
        ptr = (char *)(memseg->mem + memseg->mem_idx * 
                       (mempool->node_size + sizeof(SEG_LNODE_SIZE)));
        memseg->nodes_in_use_num += 1;
        memseg->mem_idx += 1;
        return (void *)ptr;
    }

    if (memseg->nodesptrs_start != memseg->nodesptrs_end) {
        ptr = memseg->nodes_ptrs[memseg->nodesptrs_start];
        memseg->nodes_in_use_num += 1;
        memseg->nodesptrs_start = (memseg->nodesptrs_start == 
                                  (mempool->nodes_max_num - 1)) ?
                                  (memseg->nodesptrs_start + 1) :
                                   0;
        ptr = (char *)seg_lnode;
        ptr += sizeof(SEG_LNODE_SIZE);
        return (void *)ptr;
    }

    return NULL;
}

void* mem_pool_get_node (gills_context_t *gills,
                     mem_pool_t *mempool)
{
    list_t *listnode;
    mem_segment_t *memseg;
    void *retnode;

    if (mempool->last_used_segment) {
        memseg = mempool->last_used_segment;
        retnode = (void *)mem_pool_segment_get_node(gills, mempool, memseg,
                              mempool->last_used_segment_lnode);
        if (retnode)
            return retnode;
    }
    if (mempool->nodes_in_use_num == mempool->segments_num * mempool->nodes_num) {
        return NULL;
    }
    listnode = mempool->segments_list->next;
    while (listnode != mempool->segments_list) {
        memseg = (mem_segment_t *)(listnode->data);
        if (memseg->nodes_in_use_num == mempool->nodes_max_num) {
            listnode = listnode->next;
        } else {
            retnode = mem_pool_segment_get_node(gills, mempool, memseg, listnode);
            mempool->last_used_segment = memseg;
            mempool->last_used_segment_lnode = listnode;
            return retnode;
        }
    }
    return NULL;
}

void mem_pool_node_free (gills_context_t *gills,
                     mem_pool_t *mempool, void *node)
{
    char *ptr;
    list_t *seg_lnode;
    mem_segment_t *memseg;

    ptr = (char *)node - sizeof(SEG_LNODE_SIZE);
    seg_lnode = (list_t *)ptr;

    memseg = (mem_segment_t *)(seg_lnode->data);
    if (memseg->nodesptrs_end != (mempool->nodes_max_num - 1)) {
        memseg->nodesptrs_end += 1;
    } else {
        memseg->nodesptrs_end = 0;
    }

    memseg->nodes_ptrs[memseg->nodesptrs_start] = (void *)ptr;
}

#endif    /* __MEM_POOL_H__ */


