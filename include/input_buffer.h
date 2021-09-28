#ifndef __LIBSQLIGHTER_INPUTBUFFER_H__
#define __LIBSQLIGHTER_INPUTBUFFER_H__

typedef struct {
  char *  buffer;
  size_t  buffer_lenght;
  ssize_t input_lenght;
} input_buffer_t;

input_buffer_t *input_buffer_new(void);

#endif /* __LIBSQLIGHTER_INPUTBUFFER_H__ */
