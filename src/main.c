#include <libsqlighter.h>

void print_prompt() { printf("db > "); }

void read_input(input_buffer_t * ib) {
  ssize_t readb = getline(&(ib->buffer), &(ib->buffer_lenght), stdin);

  if (0 >= readb) {
    err(EXIT_FAILURE, "Error reading input");
  }

  ib->input_lenght      = readb - 1;
  ib->buffer[readb - 1] = 0;
}

void close_input_buffer(input_buffer_t * ib) {
  if (ib) {
    if (ib->buffer) {
      free(ib->buffer);
    }
    free(ib);
  }
}

int main(int argc, char * argv[]) {
  (void)argc;
  (void)argv;

  input_buffer_t * ib = input_buffer_new();

  while (true) {
    print_prompt();
    read_input(ib);

    if (!strcmp(ib->buffer, ".exit")) {
      close_input_buffer(ib);
      break;
    } else {
      printf("Unrecognized command '%s'\n", ib->buffer);
    }
  }
}
