#ifndef __SQLC_META_COMMAND_H__
#define __SQLC_META_COMMAND_H__

#define META_COMMAND_STR_EXIT      ".exit"
#define META_COMMAND_STR_CONSTANTS ".constants"
#define META_COMMAND_STR_BTREE     ".btree"

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED,
} meta_command_result_t;

meta_command_result_t do_meta_command(input_buffer_t *ib, table_t *table);

#endif /* __SQLC_META_COMMAND_H__ */
