#include <libsqlighter.h>

prepare_result_t prepare_state(input_buffer_t *ib, state_t *state) {
  if (!strncmp(ib->buffer, STATE_INSERT_STR, STATE_INSERT_STR_LENGTH)) {
    state->type = STATE_INSERT;
    return PREPARE_SUCCESS;
  }
  if (!strncmp(ib->buffer, STATE_SELECT_STR, STATE_SELECT_STR_LENGTH)) {
    state->type = STATE_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATE;
}

void execute_state(state_t *state) {
  switch (state->type) {
  case STATE_INSERT:
    printf("INSERT\n");
    break;
  case STATE_SELECT:
    printf("SELECT\n");
    break;

  default:
    printf("Unknown state '%d'\n", state->type);
    break;
  }
}
