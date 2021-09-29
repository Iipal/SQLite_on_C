#include <sqlite_on_c.h>

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
  if (2 != argc) {
    errx(EXIT_FAILURE, "Must supply a database filename.");
  }
  const char *db_filename = argv[1];
  table_t *restrict table = db_open(db_filename);

  input_buffer_t *restrict ib = input_buffer_new();

  while (true) {
    print_prompt();
    read_input(ib);

    if (ib->buffer[0] == '.') {
      switch (do_meta_command(ib, table)) {
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
    case PREPARE_NEGATIVE_ID:
      printf("ID must be positive.\n");
      continue;
    case PREPARE_STR_TOO_LONG:
      printf("String is too long.\n");
      continue;

    case (PREPARE_UNRECOGNIZED_STATE):
    default:
      printf("Unrecognized keyword at start of '%s'\n", ib->buffer);
      continue;
    }

    switch (execute_state(table, &state)) {
    case EXECUTE_DUPLICATE_KEY:
      printf("Error: Duplicate key.\n");
      break;

    case EXECUTE_TABLE_FULL:
      printf("Error: Table full.\n");
      break;

    case EXECUTE_SUCCESS:
    default:
      break;
    }
  }
}
