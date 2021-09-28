#include <libsqlighter.h>

void print_prompt() { printf("db > "); }

void read_input(input_buffer_t *ib) {
  ssize_t readb = getline(&(ib->buffer), &(ib->buffer_lenght), stdin);

  if (0 >= readb) {
    err(EXIT_FAILURE, "Error reading input");
  }

  ib->input_lenght      = readb - 1;
  ib->buffer[readb - 1] = 0;
}

void close_input_buffer(input_buffer_t *ib) {
  if (ib) {
    if (ib->buffer) {
      free(ib->buffer);
    }
    free(ib);
  }
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  input_buffer_t *ib    = input_buffer_new();
  table_t *       table = new_table();

  while (true) {
    print_prompt();
    read_input(ib);

    if (ib->buffer[0] == '.') {
      switch (do_meta_command(ib)) {
      case META_COMMAND_SUCCESS:
        continue;
      case META_COMMAND_UNRECOGNIZED:
        printf("Unrecognized command '%s'\n", ib->buffer);
        continue;
      }
    }

    state_t state;
    switch (prepare_state(ib, &state)) {
    case PREPARE_SUCCESS:
      break;
    case PREPARE_SYNTAX_ERROR:
      printf("Syntax error. Could not parse statement.\n");
      continue;

    case (PREPARE_UNRECOGNIZED_STATE):
    default:
      printf("Unrecognized keyword at start of '%s'\n", ib->buffer);
      continue;
    }

    switch (execute_state(table, &state)) {
    case EXECUTE_TABLE_FULL:
      printf("Error: Table full.\n");
      break;

    case EXECUTE_SUCCESS:
    default:
      break;
    }
  }
}
