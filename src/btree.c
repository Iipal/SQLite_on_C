#include <libsqlighter.h>

void leaf_node_init(void *node) { *leaf_node_num_cells(node) = 0; }

uint32_t *leaf_node_num_cells(void *node) { return node + LEAF_NODE_NUM_CELLS_OFFSET; }

void *leaf_node_cell(void *node, uint32_t cell_num) {
  return node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

uint32_t *leaf_node_key(void *node, uint32_t cell_num) {
  return leaf_node_cell(node, cell_num);
}

void *leaf_node_value(void *node, uint32_t cell_num) {
  return leaf_node_cell(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

void leaf_node_insert(cursor_t *cursor, uint32_t key, row_t *value) {
  void *   node      = pager_get_page(cursor->table->pager, cursor->page_num);
  uint32_t num_cells = *leaf_node_num_cells(node);

  if (LEAF_NODE_MAX_CELLS <= num_cells) {
    err(EXIT_FAILURE, "Need to implement splitting a leaf node.\n");
  }

  if (num_cells > cursor->cell_num) {
    for (uint32_t i = num_cells; cursor->cell_num < i; --i) {
      memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i - 1), LEAF_NODE_CELL_SIZE);
    }
  }

  *(leaf_node_num_cells(node)) += 1;
  *(leaf_node_key(node, cursor->cell_num)) = key;

  serialize_row(value, leaf_node_value(node, cursor->cell_num));
}

void print_leaf_node(void *node) {
  uint32_t num_cells = *leaf_node_num_cells(node);
  printf("leaf (size %d)\n", num_cells);
  for (uint32_t i = 0; num_cells > i; ++i) {
    uint32_t key = *leaf_node_key(node, i);
    printf(" - %d: %d\n", i, key);
  }
}