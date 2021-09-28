#ifndef __LIBSQLIGHTER_TABLE_H__
#define __LIBSQLIGHTER_TABLE_H__

typedef struct {
  pager_t *pager;
  uint32_t count_rows;
} table_t;

table_t *db_open(const char *filename);
void     db_close(table_t *table);

void *get_table_row_slot(table_t *table, uint32_t row_num);

#endif /* __LIBSQLIGHTER_TABLE_H__ */
