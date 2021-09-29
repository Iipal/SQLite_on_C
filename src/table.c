#include <libsqlighter.h>

table_t *db_open(const char *filename) {
  pager_t *pager = pager_open(filename);
  table_t *out;
  assert((out = calloc(1, sizeof(*out))));

  out->pager         = pager;
  out->root_page_num = 0;

  if (!pager->num_pages) {
    void *root_node = pager_get_page(pager, 0);
    leaf_node_init(root_node);
    node_set_root(root_node, true);
  }

  return out;
}

void db_close(table_t *table) {
  pager_t *pager = table->pager;

  for (uint32_t i = 0; pager->num_pages > i; ++i) {
    if (pager->pages[i]) {
      pager_flush(pager, i);
      free(pager->pages[i]);
      pager->pages[i] = NULL;
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
