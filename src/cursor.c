#include <libsqlighter.h>

cursor_t *new_cursor_start(table_t *table) {
  cursor_t *cursor;
  assert(cursor = calloc(1, sizeof(*cursor)));

  void *   root_node = pager_get_page(table->pager, table->root_page_num);
  uint32_t num_cells = *leaf_node_num_cells(root_node);

  *cursor = (cursor_t){table, table->root_page_num, 0, !num_cells};

  return cursor;
}

cursor_t *new_cursor_end(table_t *table) {
  cursor_t *cursor;
  assert(cursor = calloc(1, sizeof(*cursor)));

  void *   root_node = pager_get_page(table->pager, table->root_page_num);
  uint32_t num_cells = *leaf_node_num_cells(root_node);

  *cursor = (cursor_t){table, table->root_page_num, num_cells, true};
  return cursor;
}

void *cursor_value(cursor_t *cursor) {
  const uint32_t page_num = cursor->page_num;
  void *         page     = pager_get_page(cursor->table->pager, page_num);

  return leaf_node_value(page, cursor->cell_num);
}

void cursor_move(cursor_t *cursor) {
  uint32_t page_num = cursor->page_num;
  void *   node     = pager_get_page(cursor->table->pager, page_num);

  cursor->end_of_table = (++cursor->cell_num >= (*leaf_node_num_cells(node)));
}
