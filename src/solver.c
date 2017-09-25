#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "grid.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
    return 1;
  }

  char *filename = argv[1];

  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  uint8_t sudoku[81];
  int8_t ch;

  uint8_t index = 0;
  while ((ch = fgetc(fp)) != EOF) {
    if (ch != '\n' && index < 81) {
      int8_t number = ch - '0';
      if (number >= 0 && number <= 9) {
        sudoku[index++] = number;
      }
    }
  }

  if (index != 81) {
    perror("Invalid sudoku file.\n");
    exit(EXIT_FAILURE);
  }

  fclose(fp);

  print(sudoku);
  solve(sudoku);

  return 0;
}
