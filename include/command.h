#ifndef __SQLC_COMMAND_H__
#define __SQLC_COMMAND_H__

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_SYNTAX_ERROR,
  PREPARE_NEGATIVE_ID,
  PREPARE_STR_TOO_LONG,
  PREPARE_UNRECOGNIZED_STATE,
} prepare_result_t;

typedef enum {
  EXECUTE_SUCCESS,
  EXECUTE_DUPLICATE_KEY,
  EXECUTE_TABLE_FULL
} execute_result_t;

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
  row_t        current;
} state_t;

prepare_result_t prepare_state(input_buffer_t *ib, state_t *state);
execute_result_t execute_state(table_t *table, state_t *state);

#endif /* __SQLC_COMMAND_H__ */
