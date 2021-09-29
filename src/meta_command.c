#include <sqlite_on_c.h>

static inline void print_constants() {
  printf("Constants:\n"
         " ROW_SIZE: %lu\n"
         " COMMON_NODE_HEADER_SIZE: %lu\n"
         " LEAF_NODE_HEADER_SIZE: %lu\n"
         " LEAF_NODE_CELL_SIZE: %lu\n"
         " LEAF_NODE_SPACE_FOR_CELLS: %lu\n"
         " LEAF_NODE_MAX_CELLS: %lu\n",
         ROW_SIZE,
         COMMON_NODE_HEADER_SIZE,
         LEAF_NODE_HEADER_SIZE,
         LEAF_NODE_CELL_SIZE,
         LEAF_NODE_SPACE_FOR_CELLS,
         LEAF_NODE_MAX_CELLS);
}

meta_command_result_t do_meta_command(input_buffer_t *ib, table_t *table) {
  if (!strcmp(ib->buffer, META_COMMAND_STR_EXIT)) {
    db_close(table);
    exit(EXIT_SUCCESS);
  } else if (!strcmp(ib->buffer, META_COMMAND_STR_CONSTANTS)) {
    print_constants();
    return META_COMMAND_SUCCESS;
  } else if (!strcmp(ib->buffer, META_COMMAND_STR_BTREE)) {
    printf("Tree:\n");
    print_tree(table->pager, 0, 0);
    return META_COMMAND_SUCCESS;
  } else {
    return META_COMMAND_UNRECOGNIZED;
  }
}
