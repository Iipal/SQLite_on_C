#ifndef __SQLC_TABLE_H__
#define __SQLC_TABLE_H__

typedef struct {
  pager_t *pager;
  uint32_t root_page_num;
} table_t;

table_t *db_open(const char *filename);
void     db_close(table_t *table);

#endif /* __SQLC_TABLE_H__ */
