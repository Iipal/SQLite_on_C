#include <libsqlighter.h>

table_t *new_table(void) {
  table_t *out = calloc(1, sizeof(*out));
  return out;
}

void free_table(table_t *table) {
  if (table) {
    bzero(table->pages, PAGE_SIZE * TABLE_MAX_PAGES);
    free(table);
  }
}

void *get_table_row_slot(table_t *table, uint32_t row_num) {
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void *   page     = table->pages[page_num];

  if (!page) {
    page = table->pages[page_num] = calloc(1, PAGE_SIZE);
  }

  uint32_t row_offset  = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;

  return page + byte_offset;
}
