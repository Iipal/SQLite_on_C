#ifndef __LIBSQLIGHTER_BTREE_H__
#define __LIBSQLIGHTER_BTREE_H__

typedef enum {
  NODE_INTERNAL,
  NODE_LEAF,
} node_type_t;

/*
 * Common Node Header Layout
 */

#define NODE_TYPE_SIZE          sizeof(uint8_t)
#define IS_ROOT_SIZE            sizeof(uint8_t)
#define PARENT_POINTER_SIZE     sizeof(uint32_t)
#define COMMON_NODE_HEADER_SIZE NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE

#define NODE_TYPE_OFFSET      0
#define IS_ROOT_OFFSET        NODE_TYPE_SIZE
#define PARENT_POINTER_OFFSET IS_ROOT_OFFSET + IS_ROOT_SIZE

/*
 * Leaf Node Header Layout
 */
#define LEAF_NODE_NUM_CELLS_SIZE   sizeof(uint32_t)
#define LEAF_NODE_NUM_CELLS_OFFSET COMMON_NODE_HEADER_SIZE
#define LEAF_NODE_HEADER_SIZE      COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE

/*
 * Leaf Node Body Layout
 */
#define LEAF_NODE_KEY_SIZE   sizeof(uint32_t)
#define LEAF_NODE_VALUE_SIZE ROW_SIZE
#define LEAF_NODE_CELL_SIZE  LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE

#define LEAF_NODE_KEY_OFFSET      0
#define LEAF_NODE_VALUE_OFFSET    LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE
#define LEAF_NODE_SPACE_FOR_CELLS PAGE_SIZE - LEAF_NODE_HEADER_SIZE
#define LEAF_NODE_MAX_CELLS       LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE

void      leaf_node_init(void *node);
uint32_t *leaf_node_num_cells(void *node);
void *    leaf_node_cell(void *node, uint32_t cell_num);
uint32_t *leaf_node_key(void *node, uint32_t cell_num);
void *    leaf_node_value(void *node, uint32_t cell_num);
void      leaf_node_insert(cursor_t *cursor, uint32_t key, row_t *value);

void print_leaf_node(void *node);

#endif /* __LIBSQLIGHTER_BTREE_H__ */