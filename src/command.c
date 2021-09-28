#include <libsqlighter.h>

static inline prepare_result_t prepare_insert(input_buffer_t *ib, state_t *state) {
  state->type = STATE_INSERT;

  char *keyword   = strtok(ib->buffer, " ");
  char *id_string = strtok(NULL, " ");
  char *username  = strtok(NULL, " ");
  char *email     = strtok(NULL, " ");

  if (!id_string || !username || !email) {
    return PREPARE_SYNTAX_ERROR;
  }
  if (COLUMN_USERNAME_SIZE < strlen(username) || COLUMN_EMAIL_SIZE < strlen(email)) {
    return PREPARE_STR_TOO_LONG;
  }

  int id = atoi(id_string);
  if (0 > id) {
    return PREPARE_NEGATIVE_ID;
  }

  state->current.id = id;
  strcpy(state->current.username, username);
  strcpy(state->current.email, email);

  (void)keyword;
  return PREPARE_SUCCESS;
}

prepare_result_t prepare_state(input_buffer_t *ib, state_t *state) {
  if (!strncmp(ib->buffer, STATE_INSERT_STR, STATE_INSERT_STR_LENGTH)) {
    return prepare_insert(ib, state);
  }
  if (!strncmp(ib->buffer, STATE_SELECT_STR, STATE_SELECT_STR_LENGTH)) {
    state->type = STATE_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATE;
}

static inline execute_result_t execute_insert(table_t *table, state_t *state) {
  void *node = pager_get_page(table->pager, table->root_page_num);
  if (LEAF_NODE_MAX_CELLS <= *leaf_node_num_cells(node)) {
    return EXECUTE_TABLE_FULL;
  }

  row_t *   r      = &(state->current);
  cursor_t *cursor = new_cursor_end(table);

  leaf_node_insert(cursor, r->id, r);

  free(cursor);

  return EXECUTE_SUCCESS;
}
static inline execute_result_t execute_select(table_t *table, state_t *state) {
  (void)state;
  cursor_t *cursor = new_cursor_start(table);
  row_t     r;

  while (!(cursor->end_of_table)) {
    deserialize_row(cursor_value(cursor), &r);
    print_row(&r);
    cursor_move(cursor);
  }

  free(cursor);

  return EXECUTE_SUCCESS;
}

execute_result_t execute_state(table_t *table, state_t *state) {
  switch (state->type) {
  case STATE_INSERT:
    return execute_insert(table, state);
  case STATE_SELECT:
    return execute_select(table, state);

  default:
    printf("Unknown state '%d'\n", state->type);
    break;
  }
}
