#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_LENGTH(x) (sizeof(x) / sizeof(x[0]))

/*
 * | i 		| use
 * | ------ | ---
 * | 0 		| temporary storage to operate on
 * | 1,2,3 	| storage for contents
 */
int SLOT_ARRAY[] = {0, 0, 0, 0};

enum Operator { SET = 0, LOAD, ADD, SUB, MULT, DIV, STORE, END = 99 };
const char *const operators[] = {
    [SET] = "SET",   [LOAD] = "LOAD", [ADD] = "ADD",     [SUB] = "SUB",
    [MULT] = "MULT", [DIV] = "DIV",   [STORE] = "STORE", [END] = "END",
};

void print_slots();
void reg_log(const int operator, const int slot);
void die(const char *text, const int line);
void execute_operation(const int operator, const int slot, const int set_value,
                       const int line);
void parse_file(const char *text);

/**
 * Prints the resulting array
 */
void print_slots() {
  printf("{");
  int length = (int)ARR_LENGTH(SLOT_ARRAY);
  for (int i = 0; i < length; i++) {
    printf("%d%s", SLOT_ARRAY[i], i == length - 1 ? "" : ",");
  }
  printf("}\n");
}

/**
 * prints the operator and the slot, after that prints the slots
 * @param operator int, see num operator
 * @param slot int, see SLOT_ARRAY
 */
void reg_log(const int operator, const int slot) {
  printf("%5s %2d: ", operators[operator], slot);
  print_slots();
}

/**
 * prints error and exits with failure status
 * @param *text
 */
void die(const char *text, const int line) {
  printf("Error: %s%c\n", text, line != -1 ? (char)line : ' ');
  exit(EXIT_FAILURE);
}

void execute_operation(const int operator, const int slot, const int set_value,
                       const int line) {
  switch (operator) {
  case END:
    reg_log(operator, slot);
    exit(EXIT_SUCCESS);
  case SET:
    if (slot == 0)
      die("provided 'set_value' can not be assigned to 'SLOT0', assigning to "
          "SLOT0 is not allowed!",
          line);
    if (set_value != 0 && SLOT_ARRAY[slot] == 0)
      SLOT_ARRAY[slot] = set_value;
    break;
  case LOAD:
    SLOT_ARRAY[0] = SLOT_ARRAY[slot];
    break;
  case ADD:
    SLOT_ARRAY[0] += SLOT_ARRAY[slot];
    break;
  case SUB:
    SLOT_ARRAY[0] -= SLOT_ARRAY[slot];
    break;
  case MULT:
    SLOT_ARRAY[0] *= SLOT_ARRAY[slot];
    break;
  case DIV:
    SLOT_ARRAY[0] /= SLOT_ARRAY[slot];
    break;
  case STORE:
    SLOT_ARRAY[slot] = SLOT_ARRAY[0];
    SLOT_ARRAY[0] = 0;
    break;
  default:
    die("selected operator invalid or unavailable", line);
  }
  reg_log(operator, slot);
}

void parse_file(const char *text) {
  FILE *file = fopen(text, "r");
  if (file == NULL) {
    die("file could not be opened", -1);
  }
  // TODO: read each line
  // TODO: split line into OPERATOR SLOT
  // TODO: execute OPERATOR on SLOT
  fclose(file);
}

int main(int argc, char **argv) {
  char filename[255];

  if (argc == 1) {
    die("missing file", -1);
  }

  strcpy(filename, argv[1]);

  // check if filename contains substring .regmach
  if (!strstr(filename, ".regmach")) {
    die("fileformat not supported, use .regmach", -1);
  }

  parse_file(filename);
  return EXIT_SUCCESS;
}
