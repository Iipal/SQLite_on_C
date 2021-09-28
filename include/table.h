#ifndef __LIBSQLIGHTER_TABLE_H__
#define __LIBSQLIGHTER_TABLE_H__

#define PAGE_SIZE       4096
#define TABLE_MAX_PAGES 100
#define ROWS_PER_PAGE   (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_ROWS  (ROWS_PER_PAGE * TABLE_MAX_PAGES)

typedef struct {
  uint32_t count_rows;
  void *   pages[TABLE_MAX_PAGES];
} table_t;

table_t *new_table(void);
void     free_table(table_t *table);

void *get_table_row_slot(table_t *table, uint32_t row_num);

#endif /* __LIBSQLIGHTER_TABLE_H__ */
