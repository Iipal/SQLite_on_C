#ifndef __LIBSQLIGHTER_META_COMMAND_H__
#define __LIBSQLIGHTER_META_COMMAND_H__

#define META_COMMAND_EXIT ".exit"

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED,
} meta_command_result_t;

meta_command_result_t do_meta_command(input_buffer_t *ib);

#endif /* __LIBSQLIGHTER_META_COMMAND_H__ */
