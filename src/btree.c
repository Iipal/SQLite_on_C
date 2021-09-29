#include <libsqlighter.h>

void node_new_root(table_t *table, uint32_t right_child_page_num) {
  void *   root                = pager_get_page(table->pager, table->root_page_num);
  void *   right_child         = pager_get_page(table->pager, right_child_page_num);
  uint32_t left_child_page_num = pager_get_unused_page_num(table->pager);
  void *   left_child          = pager_get_page(table->pager, left_child_page_num);

  memcpy(left_child, root, PAGE_SIZE);
  node_set_root(left_child, false);

  leaf_node_init(root);
  node_set_root(root, true);
  *node_internal_num_keys(root) = 1;
  *node_internal_child(root, 0) = left_child_page_num;

  uint32_t left_child_max_key      = node_get_max_key(left_child);
  *node_internal_key(root, 0)      = left_child_max_key;
  *node_internal_right_child(root) = right_child_page_num;
  *node_parent(left_child)         = table->root_page_num;
  *node_parent(right_child)        = table->root_page_num;
}

void leaf_node_init(void *node) {
  leaf_node_set_type(node, NODE_LEAF);
  node_set_root(node, false);
  *leaf_node_num_cells(node) = 0;
  *leaf_node_next_leaf(node) = 0;
}

void node_set_root(void *node, bool is_root) {
  uint8_t value                         = is_root;
  *((uint8_t *)(node + IS_ROOT_OFFSET)) = value;
}

uint32_t *node_parent(void *node) { return node + PARENT_POINTER_OFFSET; }

uint32_t node_get_max_key(void *node) {
  switch (leaf_node_get_type(node)) {
  case NODE_INTERNAL:
    return *node_internal_key(node, *node_internal_num_keys(node) - 1);

  case NODE_LEAF:
    return *leaf_node_key(node, *leaf_node_num_cells(node) - 1);
  }
}

void node_internal_init(void *node) {
  leaf_node_set_type(node, NODE_INTERNAL);
  node_set_root(node, false);
  *node_internal_num_keys(node) = 0;
}

uint32_t *node_internal_num_keys(void *node) {
  return node + INTERNAL_NODE_NUM_KEYS_OFFSET;
}

uint32_t *node_internal_right_child(void *node) {
  return node + INTERNAL_NODE_RIGHT_CHILD_OFFSET;
}

uint32_t *node_internal_child(void *node, uint32_t child_num) {
  uint32_t num_keys = *node_internal_num_keys(node);

  if (child_num > num_keys) {
    errx(EXIT_FAILURE, "Tried to access child_num %d > num_keys %d", child_num, num_keys);
  } else if (child_num == num_keys) {
    return node_internal_right_child(node);
  } else {
    return node_internal_cell(node, child_num);
  }
}

uint32_t *node_internal_cell(void *node, uint32_t cell_num) {
  return node + INTERNAL_NODE_HEADER_SIZE + cell_num * INTERNAL_NODE_CELL_SIZE;
}

uint32_t *node_internal_key(void *node, uint32_t key_num) {
  return (void *)node_internal_cell(node, key_num) + INTERNAL_NODE_CHILD_SIZE;
}

void node_internal_update_key(void *node, uint32_t old_key, uint32_t new_key) {
  uint32_t old_child_index = node_internal_find_child(node, old_key);

  *node_internal_key(node, old_child_index) = new_key;
}

uint32_t node_internal_find_child(void *node, uint32_t key) {
  uint32_t num_keys = *node_internal_num_keys(node);

  uint32_t min_index = 0;
  uint32_t max_index = num_keys;

  while (min_index != max_index) {
    uint32_t index        = (min_index + max_index) / 2;
    uint32_t key_to_right = *node_internal_key(node, index);
    if (key_to_right >= key) {
      max_index = index;
    } else {
      min_index = index + 1;
    }
  }

  return min_index;
}

cursor_t *node_internal_find(table_t *table, uint32_t page_num, uint32_t key) {
  void *node = pager_get_page(table->pager, page_num);

  uint32_t child_index = node_internal_find_child(node, key);
  uint32_t child_num   = *node_internal_child(node, child_index);
  void *   child       = pager_get_page(table->pager, child_num);

  switch (leaf_node_get_type(child)) {
  case NODE_LEAF:
    return leaf_node_find(table, child_num, key);

  case NODE_INTERNAL:
    return node_internal_find(table, child_num, key);
  }
}

void node_internal_insert(table_t *table,
                          uint32_t parent_page_num,
                          uint32_t child_page_num) {
  void *   parent        = pager_get_page(table->pager, parent_page_num);
  void *   child         = pager_get_page(table->pager, child_page_num);
  uint32_t child_max_key = node_get_max_key(child);
  uint32_t index         = node_internal_find_child(parent, child_max_key);

  uint32_t original_num_keys      = *node_internal_num_keys(parent);
  *node_internal_num_keys(parent) = original_num_keys + 1;

  if (INTERNAL_NODE_MAX_CELLS <= original_num_keys) {
    errx(EXIT_FAILURE, "Need to implement splitting internal node.");
  }

  uint32_t right_child_page_num = *node_internal_right_child(parent);
  void *   right_child          = pager_get_page(table->pager, right_child_page_num);

  if (child_max_key > node_get_max_key(right_child)) {
    *node_internal_child(parent, original_num_keys) = right_child_page_num;
    *node_internal_key(parent, original_num_keys)   = node_get_max_key(right_child);
    *node_internal_right_child(parent)              = child_page_num;
  } else {
    for (uint32_t i = original_num_keys; i > index; --i) {
      void *dst = node_internal_cell(parent, i);
      void *src = node_internal_cell(parent, i - 1);
      memcpy(dst, src, INTERNAL_NODE_CELL_SIZE);
    }

    *node_internal_child(parent, index) = child_page_num;
    *node_internal_key(parent, index)   = child_max_key;
  }
}

bool is_node_root(void *node) {
  uint8_t value = *((uint8_t *)(node + IS_ROOT_OFFSET));
  return (bool)value;
}

uint32_t *leaf_node_num_cells(void *node) { return node + LEAF_NODE_NUM_CELLS_OFFSET; }

void *leaf_node_cell(void *node, uint32_t cell_num) {
  return node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

node_type_t leaf_node_get_type(void *node) {
  uint8_t value = *((uint8_t *)(node + NODE_TYPE_OFFSET));
  return (node_type_t)value;
}

void leaf_node_set_type(void *node, node_type_t type) {
  uint8_t value                           = type;
  *((uint8_t *)(node + NODE_TYPE_OFFSET)) = value;
}

uint32_t *leaf_node_key(void *node, uint32_t cell_num) {
  return leaf_node_cell(node, cell_num);
}

void *leaf_node_value(void *node, uint32_t cell_num) {
  return leaf_node_cell(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

void leaf_node_insert(cursor_t *cursor, uint32_t key, row_t *value) {
  void *   node      = pager_get_page(cursor->table->pager, cursor->page_num);
  uint32_t num_cells = *leaf_node_num_cells(node);

  if (LEAF_NODE_MAX_CELLS <= num_cells) {
    leaf_node_split_and_insert(cursor, key, value);
    return;
  }

  if (num_cells > cursor->cell_num) {
    for (uint32_t i = num_cells; cursor->cell_num < i; --i) {
      memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i - 1), LEAF_NODE_CELL_SIZE);
    }
  }

  *(leaf_node_num_cells(node)) += 1;
  *(leaf_node_key(node, cursor->cell_num)) = key;

  serialize_row(value, leaf_node_value(node, cursor->cell_num));
}

void leaf_node_split_and_insert(cursor_t *cursor, uint32_t key, row_t *value) {
  void *   old_node     = pager_get_page(cursor->table->pager, cursor->page_num);
  uint32_t old_max      = node_get_max_key(old_node);
  uint32_t new_page_num = pager_get_unused_page_num(cursor->table->pager);
  void *   new_node     = pager_get_page(cursor->table->pager, new_page_num);

  leaf_node_init(new_node);
  *node_parent(new_node)         = *node_parent(old_node);
  *leaf_node_next_leaf(new_node) = *leaf_node_next_leaf(old_node);
  *leaf_node_next_leaf(old_node) = new_page_num;

  for (int32_t i = LEAF_NODE_MAX_CELLS; 0 <= i; --i) {
    void *dst_node;
    if ((int32_t)LEAF_NODE_LEFT_SPLIT_COUNT >= i) {
      dst_node = new_node;
    } else {
      dst_node = old_node;
    }

    uint32_t index_within_node = i % LEAF_NODE_LEFT_SPLIT_COUNT;
    void *   dst               = leaf_node_cell(dst_node, index_within_node);

    if (i == (int32_t)cursor->cell_num) {
      serialize_row(value, leaf_node_value(dst_node, index_within_node));
      *leaf_node_key(dst_node, index_within_node) = key;
    } else if (i > (int32_t)cursor->cell_num) {
      memcpy(dst, leaf_node_cell(old_node, i - 1), LEAF_NODE_CELL_SIZE);
    } else {
      memcpy(dst, leaf_node_cell(old_node, i), LEAF_NODE_CELL_SIZE);
    }
  }

  *(leaf_node_num_cells(old_node)) = LEAF_NODE_LEFT_SPLIT_COUNT;
  *(leaf_node_num_cells(new_node)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

  if (is_node_root(old_node)) {
    return node_new_root(cursor->table, new_page_num);
  } else {
    uint32_t parent_page_num = *node_parent(old_node);
    uint32_t new_max         = node_get_max_key(old_node);
    void *   parent          = pager_get_page(cursor->table->pager, parent_page_num);

    node_internal_update_key(parent, old_max, new_max);
    node_internal_insert(cursor->table, parent_page_num, new_page_num);
    return;
  }

  (void)key;
}

cursor_t *leaf_node_find(table_t *table, uint32_t page_num, uint32_t key) {
  void *   node      = pager_get_page(table->pager, page_num);
  uint32_t num_cells = *leaf_node_num_cells(node);

  cursor_t *cursor;
  assert(cursor = calloc(1, sizeof(*cursor)));

  cursor->table    = table;
  cursor->page_num = page_num;

  uint32_t min_index          = 0;
  uint32_t one_past_max_index = num_cells;

  while (one_past_max_index != min_index) {
    uint32_t index        = (min_index + one_past_max_index) / 2;
    uint32_t key_at_index = *leaf_node_key(node, index);

    if (key_at_index == key) {
      cursor->cell_num = index;
      return cursor;
    }

    if (key_at_index > key) {
      one_past_max_index = index;
    } else {
      min_index = index + 1;
    }
  }

  cursor->cell_num = min_index;
  return cursor;
}

uint32_t *leaf_node_next_leaf(void *node) { return node + LEAF_NODE_NEXT_LEAF_OFFSET; }

static inline void print_tree_indent(uint32_t lvl) {
  for (uint32_t i = 0; lvl > i; ++i) {
    printf(" ");
  }
}

void print_tree(pager_t *pager, uint32_t page_num, uint32_t indent_lvl) {
  void *   node = pager_get_page(pager, page_num);
  uint32_t num_keys;
  uint32_t child;

  switch (leaf_node_get_type(node)) {
  case NODE_LEAF:
    num_keys = *leaf_node_num_cells(node);

    print_tree_indent(indent_lvl);
    printf("- leaf (size %d)\n", num_keys);
    for (uint32_t i = 0; num_keys > i; ++i) {
      print_tree_indent(indent_lvl + 1);
      printf("- %d\n", *leaf_node_key(node, i));
    }

    break;

  case NODE_INTERNAL:
    num_keys = *node_internal_num_keys(node);

    print_tree_indent(indent_lvl);
    printf("- internal (size %d)\n", num_keys);
    for (uint32_t i = 0; num_keys > i; ++i) {
      child = *node_internal_child(node, i);

      print_tree(pager, child, indent_lvl + 1);
      print_tree_indent(indent_lvl + 1);
      printf("- key %d\n", *node_internal_key(node, i));
    }

    child = *node_internal_right_child(node);
    print_tree(pager, child, indent_lvl + 1);
    break;
  }
}
