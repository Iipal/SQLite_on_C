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
#define COMMON_NODE_HEADER_SIZE (NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE)

#define NODE_TYPE_OFFSET      0
#define IS_ROOT_OFFSET        NODE_TYPE_SIZE
#define PARENT_POINTER_OFFSET (IS_ROOT_OFFSET + IS_ROOT_SIZE)

/*
 * Leaf Node Header Layout
 */
#define LEAF_NODE_NUM_CELLS_SIZE   sizeof(uint32_t)
#define LEAF_NODE_NUM_CELLS_OFFSET COMMON_NODE_HEADER_SIZE
#define LEAF_NODE_NEXT_LEAF_SIZE   sizeof(uint32_t)
#define LEAF_NODE_NEXT_LEAF_OFFSET (LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE)
#define LEAF_NODE_HEADER_SIZE \
  (COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE + LEAF_NODE_NEXT_LEAF_SIZE)

/*
 * Leaf Node Body Layout
 */
#define LEAF_NODE_KEY_SIZE   sizeof(uint32_t)
#define LEAF_NODE_VALUE_SIZE ROW_SIZE
#define LEAF_NODE_CELL_SIZE  (LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE)

#define LEAF_NODE_KEY_OFFSET      0
#define LEAF_NODE_VALUE_OFFSET    (LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE)
#define LEAF_NODE_SPACE_FOR_CELLS (PAGE_SIZE - LEAF_NODE_HEADER_SIZE)
#define LEAF_NODE_MAX_CELLS       (LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE)

#define LEAF_NODE_RIGHT_SPLIT_COUNT ((LEAF_NODE_MAX_CELLS + 1) / 2)
#define LEAF_NODE_LEFT_SPLIT_COUNT \
  ((LEAF_NODE_MAX_CELLS + 1) - LEAF_NODE_RIGHT_SPLIT_COUNT)

/*
 * Internal Node Header Layout
 */
#define INTERNAL_NODE_NUM_KEYS_SIZE    sizeof(uint32_t)
#define INTERNAL_NODE_NUM_KEYS_OFFSET  COMMON_NODE_HEADER_SIZE
#define INTERNAL_NODE_RIGHT_CHILD_SIZE sizeof(uint32_t)
#define INTERNAL_NODE_RIGHT_CHILD_OFFSET \
  (INTERNAL_NODE_NUM_KEYS_OFFSET + INTERNAL_NODE_NUM_KEYS_SIZE)
#define INTERNAL_NODE_HEADER_SIZE \
  (COMMON_NODE_HEADER_SIZE + INTERNAL_NODE_NUM_KEYS_SIZE + INTERNAL_NODE_RIGHT_CHILD_SIZE)

/*
 * Internal Node Body Layout
 */
#define INTERNAL_NODE_KEY_SIZE   sizeof(uint32_t)
#define INTERNAL_NODE_CHILD_SIZE sizeof(uint32_t)
#define INTERNAL_NODE_CELL_SIZE  (INTERNAL_NODE_CHILD_SIZE + INTERNAL_NODE_KEY_SIZE)

void node_new_root(table_t *table, uint32_t right_child_page_num);
void node_set_root(void *node, bool is_root);

uint32_t node_get_max_key(void *node);

void      node_internal_init(void *node);
uint32_t *node_internal_num_keys(void *node);
uint32_t *node_internal_right_child(void *node);
uint32_t *node_internal_child(void *node, uint32_t child_num);
uint32_t *node_internal_cell(void *node, uint32_t cell_num);
uint32_t *node_internal_key(void *node, uint32_t key_num);
cursor_t *node_internal_find(table_t *table, uint32_t s, uint32_t key);

bool is_node_root(void *node);

void        leaf_node_init(void *node);
uint32_t *  leaf_node_num_cells(void *node);
void *      leaf_node_cell(void *node, uint32_t cell_num);
node_type_t leaf_node_get_type(void *node);
void        leaf_node_set_type(void *node, node_type_t type);
uint32_t *  leaf_node_key(void *node, uint32_t cell_num);
void *      leaf_node_value(void *node, uint32_t cell_num);
void        leaf_node_insert(cursor_t *cursor, uint32_t key, row_t *value);
void        leaf_node_split_and_insert(cursor_t *cursor, uint32_t key, row_t *value);
cursor_t *  leaf_node_find(table_t *table, uint32_t page_num, uint32_t key);
uint32_t *  leaf_node_next_leaf(void *node);

void print_tree(pager_t *pager, uint32_t page_num, uint32_t indent_lvl);

#endif /* __LIBSQLIGHTER_BTREE_H__ */
