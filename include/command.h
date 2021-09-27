#ifndef __LIBSQLIGHTER_COMMAND_H__
#define __LIBSQLIGHTER_COMMAND_H__

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATE,
} prepare_result_t;

typedef enum {
#define STATE_STR_GET_LENGTH(_sstr) (sizeof(_sstr) - sizeof(char))

  STATE_INSERT,
#define STATE_INSERT_STR        "insert"
#define STATE_INSERT_STR_LENGTH STATE_STR_GET_LENGTH(STATE_INSERT_STR)

  STATE_SELECT,
#define STATE_SELECT_STR        "select"
#define STATE_SELECT_STR_LENGTH STATE_STR_GET_LENGTH(STATE_SELECT_STR)

} state_type_t;

typedef struct {
  state_type_t type;
} state_t;

prepare_result_t prepare_state(input_buffer_t *ib, state_t *state);
void             execute_state(state_t *state);

#endif /* __LIBSQLIGHTER_COMMAND_H__ */
