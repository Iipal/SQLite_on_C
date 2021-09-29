#ifndef __LIBSQLIGHTER_PAGER_H__
#define __LIBSQLIGHTER_PAGER_H__

#define PAGE_SIZE       4096
#define TABLE_MAX_PAGES 100

typedef struct {
  void *   pages[TABLE_MAX_PAGES];
  int      fd;
  uint32_t num_pages;
  uint32_t file_lenght;
} pager_t;

pager_t *pager_open(const char *filename);
void *   pager_get_page(pager_t *pager, uint32_t page_num);
uint32_t pager_get_unused_page_num(pager_t *pager);
void     pager_flush(pager_t *pager, uint32_t page_num);

#endif /* __LIBSQLIGHTER_PAGER_H__ */
