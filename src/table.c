#include <libsqlighter.h>

table_t *db_open(const char *filename) {
  pager_t *pager = pager_open(filename);
  uint32_t rows  = pager->file_lenght / ROW_SIZE;

  table_t *out;
  assert((out = calloc(1, sizeof(*out))));

  out->pager      = pager;
  out->count_rows = rows;

  return out;
}

void db_close(table_t *table) {
  pager_t *      pager          = table->pager;
  const uint32_t num_full_pages = table->count_rows / ROWS_PER_PAGE;

  for (uint32_t i = 0; num_full_pages > i; ++i) {
    if (pager->pages[i]) {
      pager_flush(pager, i, PAGE_SIZE);
      free(pager->pages[i]);
      pager->pages[i] = NULL;
    }
  }

  uint32_t num_add_rows = table->count_rows % ROW_SIZE;
  if (0 < num_add_rows) {
    uint32_t page_num = num_full_pages;
    if (pager->pages[page_num]) {
      pager_flush(pager, page_num, num_add_rows * ROW_SIZE);
      free(pager->pages[page_num]);
      pager->pages[page_num] = NULL;
    }
  }

  int res = close(pager->fd);
  if (0 > res) {
    err(EXIT_FAILURE, "Error closing db-file");
  }

  for (uint32_t i = 0; TABLE_MAX_PAGES > i; ++i) {
    void *page = pager->pages[i];
    if (page) {
      free(page);
      pager->pages[i] = NULL;
    }
  }

  free(pager);
  free(table);
}

void *get_table_row_slot(table_t *table, uint32_t row_num) {
  const uint32_t page_num    = row_num / ROWS_PER_PAGE;
  void *         page        = pager_get_page(table->pager, page_num);
  const uint32_t row_offset  = row_num % ROWS_PER_PAGE;
  const uint32_t byte_offset = row_offset * ROW_SIZE;

  return page + byte_offset;
}
