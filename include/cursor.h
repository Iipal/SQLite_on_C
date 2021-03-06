#ifndef __SQLC_CURSOR_H__
#define __SQLC_CURSOR_H__

typedef struct {
  table_t *table;
  uint32_t page_num;
  uint32_t cell_num;
  bool     end_of_table;
} cursor_t;

cursor_t *new_cursor_start(table_t *table);

cursor_t *cursor_find(table_t *table, uint32_t key);
void *    cursor_value(cursor_t *cursor);
void      cursor_move(cursor_t *cursor);

#endif /* __SQLC_CURSOR_H__ */
