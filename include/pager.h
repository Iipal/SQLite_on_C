#ifndef __LIBSQLIGHTER_PAGER_H__
#define __LIBSQLIGHTER_PAGER_H__

#define PAGE_SIZE       4096
#define TABLE_MAX_PAGES 100
#define ROWS_PER_PAGE   (PAGE_SIZE / ROW_SIZE)
#define TABLE_MAX_ROWS  (ROWS_PER_PAGE * TABLE_MAX_PAGES)

typedef struct {
  void *   pages[TABLE_MAX_PAGES];
  int      fd;
  uint32_t file_lenght;
} pager_t;

pager_t *pager_open(const char *filename);
void *   pager_get_page(pager_t *pager, uint32_t page_num);
void     pager_flush(pager_t *pager, uint32_t page_num, uint32_t size);

#endif /* __LIBSQLIGHTER_PAGER_H__ */
