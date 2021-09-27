#include <libsqlighter.h>

input_buffer_t *input_buffer_new(void) {
  input_buffer_t *out = calloc(1, sizeof(*out));

  return out;
}
