#include <libsqlighter.h>

cursor_t *new_cursor_start(table_t *table) {
  cursor_t *cursor;
  assert(cursor = calloc(1, sizeof(*cursor)));

  *cursor = (cursor_t){table, 0, !table->count_rows};
  return cursor;
}

cursor_t *new_cursor_end(table_t *table) {
  cursor_t *cursor;
  assert(cursor = calloc(1, sizeof(*cursor)));

  *cursor = (cursor_t){table, table->count_rows, true};
  return cursor;
}

void *cursor_value(cursor_t *cursor) {
  const uint32_t row_num     = cursor->row_num;
  const uint32_t page_num    = row_num / ROWS_PER_PAGE;
  void *         page        = get_page(cursor->table->pager, page_num);
  const uint32_t row_offset  = row_num % ROWS_PER_PAGE;
  const uint32_t byte_offset = row_offset * ROW_SIZE;

  return page + byte_offset;
}

void cursor_move(cursor_t *cursor) {
  cursor->end_of_table = !!(++cursor->row_num >= cursor->table->count_rows);
}
