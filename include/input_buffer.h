#ifndef __LIBSQLIGHTER_INPUTBUFFER_H__
#define __LIBSQLIGHTER_INPUTBUFFER_H__

#ifndef __LIBSQLIGHTER_H__
#  error "include only libsqlighter.h"
#endif

#include <sys/types.h>

typedef struct {
  char *  buffer;
  size_t  buffer_lenght;
  ssize_t input_lenght;
} input_buffer_t;

input_buffer_t * input_buffer_new(void);

#endif /* __LIBSQLIGHTER_INPUTBUFFER_H__ */
