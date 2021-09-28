#ifndef __LIBSQLIGHTER_CURSOR_H__
#define __LIBSQLIGHTER_CURSOR_H__

typedef struct {
  table_t *table;
  uint32_t row_num;
  bool     end_of_table;
} cursor_t;

cursor_t *new_cursor_start(table_t *table);
cursor_t *new_cursor_end(table_t *table);

void *cursor_value(cursor_t *cursor);
void  cursor_move(cursor_t *cursor);

#endif /* __LIBSQLIGHTER_CURSOR_H__ */
