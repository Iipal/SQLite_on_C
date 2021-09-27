#include <libsqlighter.h>

meta_command_result_t do_meta_command(input_buffer_t *ib) {
  if (!strcmp(ib->buffer, META_COMMAND_EXIT)) {
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED;
  }
}
