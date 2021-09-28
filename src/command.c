#include <libsqlighter.h>

prepare_result_t prepare_insert(input_buffer_t *ib, state_t *state) {
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

  state->cursor.id = id;
  strcpy(state->cursor.username, username);
  strcpy(state->cursor.email, email);

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

execute_result_t execute_insert(table_t *table, state_t *state) {
  if (TABLE_MAX_ROWS <= table->count_rows) {
    return EXECUTE_TABLE_FULL;
  }

  row_t *r = &(state->cursor);
  serialize_row(r, get_table_row_slot(table, table->count_rows));
  ++table->count_rows;

  return EXECUTE_SUCCESS;
}
execute_result_t execute_select(table_t *table, state_t *state) {
  (void)state;
  row_t r;

  for (uint32_t i = 0; table->count_rows > i; ++i) {
    deserialize_row(get_table_row_slot(table, i), &r);
    print_row(&r);
  }

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
