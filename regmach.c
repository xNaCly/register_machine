#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_LENGTH(x) (sizeof(x) / sizeof(x[0]))

int SLOT_ARRAY[] = {0, 0, 0, 0, 0, 0, 0};
enum Operator { SET = 0, LOADI, LOAD, ADD, SUB, MULT, DIV, STORE, END = 99 };
const char *const operators[] = {
    [SET] = "SET",     [LOADI] = "LOADI",[LOAD] = "LOAD",
    [ADD] = "ADD",     [SUB] = "SUB",
    [MULT] = "MULT",   [DIV] = "DIV",
    [STORE] = "STORE", [END] = "END",
};

void print_slots();
void reg_log(const int operator, const int slot);
void die(const char *text, const int line);
void execute_operation(const int operator, const int slot, const int line);
int parse_operator(const char *text);
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
 * prints the operator and the slot, after that prints the slots @param operator
 * int, see num operator @param slot int, see SLOT_ARRAY
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
  printf("Error: %s. %s%d\n", text, line != -1 ? "Line: " : "",
         line != -1 ? line + 1 : ' ');
  exit(EXIT_FAILURE);
}

void execute_operation(const int operator, const int slot, const int line) {
  switch (operator) {
  case END:
    reg_log(operator, slot);
    exit(EXIT_SUCCESS);
  case LOADI:
    SLOT_ARRAY[0] = slot;
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

int parse_operator(const char *text) {
  if (strcmp(text, "END") == 0) {
    return END;
  } else if (strcmp(text, "SET") == 0) {
    return SET;
  } else if (strcmp(text, "LOADI") == 0) {
    return LOADI;
  } else if (strcmp(text, "LOAD") == 0) {
    return LOAD;
  } else if (strcmp(text, "ADD") == 0) {
    return ADD;
  } else if (strcmp(text, "SUB") == 0) {
    return SUB;
  } else if (strcmp(text, "MULT") == 0) {
    return MULT;
  } else if (strcmp(text, "DIV") == 0) {
    return DIV;
  } else if (strcmp(text, "STORE") == 0) {
    return STORE;
  } else if (strcmp(text, "END") == 0) {
    return END;
  } else {
    return -1;
  }
}

void parse_file(const char *text) {
  FILE *file = fopen(text, "r");
  char line[100];
  int cur_line = 1;

  if (file == NULL) {
    die("file could not be opened", -1);
  }

  while (fgets(line, sizeof(line), file)) {
    char operator[100];
    int slot = 0;

    fscanf(file, "%s %d", operator, & slot);

    execute_operation(parse_operator(operator), slot, cur_line);
    cur_line++;
  }

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
