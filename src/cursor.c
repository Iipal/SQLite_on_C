#include <libsqlighter.h>

cursor_t *new_cursor_start(table_t *table) {
  cursor_t *cursor = cursor_find(table, 0);

  void *   node      = pager_get_page(table->pager, cursor->page_num);
  uint32_t num_cells = *leaf_node_num_cells(node);

  cursor->end_of_table = !num_cells;

  return cursor;
}

cursor_t *cursor_find(table_t *table, uint32_t key) {
  uint32_t root_page_num = table->root_page_num;
  void *   root_node     = pager_get_page(table->pager, root_page_num);

  if (NODE_LEAF == leaf_node_get_type(root_node)) {
    return leaf_node_find(table, root_page_num, key);
  } else {
    return node_internal_find(table, root_page_num, key);
  }
}

void *cursor_value(cursor_t *cursor) {
  const uint32_t page_num = cursor->page_num;
  void *         page     = pager_get_page(cursor->table->pager, page_num);

  return leaf_node_value(page, cursor->cell_num);
}

void cursor_move(cursor_t *cursor) {
  uint32_t page_num = cursor->page_num;
  void *   node     = pager_get_page(cursor->table->pager, page_num);

  ++cursor->cell_num;
  if (cursor->cell_num >= (*leaf_node_num_cells(node))) {
    uint32_t next_page_num = *leaf_node_next_leaf(node);
    if (!next_page_num) {
      cursor->end_of_table = true;
    } else {
      cursor->page_num = next_page_num;
      cursor->cell_num = 0;
    }
  }
}
