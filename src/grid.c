#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define is_bit_set(bitmap, bitindex) ((bitmap & (1 << (bitindex - 1))) != 0)
#define set_bit(bitmap, bitindex) (bitmap |= (1 << (bitindex - 1)))

void backtrack(uint_fast8_t *partial_candidate, bool *solved);
void generate_next_candidate(uint_fast8_t *candidate, int_fast8_t working_index, uint_fast8_t next_value);
uint64_t difftime_in_micros(struct timeval *start_time, struct timeval *stop_time);
void copy(uint_fast8_t *new_sudoku, uint_fast8_t *old_sudoku);
bool is_valid(uint_fast8_t *sudoku);
bool has_dupes(uint_fast8_t *v);
bool is_complete(uint_fast8_t *sudoku);
int_fast8_t find_best_index(uint_fast8_t *sudoku);
uint_fast8_t *row(uint_fast8_t *sudoku, uint_fast8_t row_index);
uint_fast8_t *column(uint_fast8_t *sudoku, uint_fast8_t column_index, uint_fast8_t *column);
uint_fast8_t *quadrant(uint_fast8_t *sudoku, uint_fast8_t quadrant_index, uint_fast8_t *quadrant);

//
// PUBLIC METHODS
//

void print(uint_fast8_t *sudoku) {
  printf("-------------------------\n");
  for (uint_fast8_t row = 0; row < 9; ++row) {
    printf("|");
    for (uint_fast8_t column = 0; column < 9; ++column) {
      uint_fast8_t number = sudoku[9 * row + column];
      printf(" %c", number == 0 ? ' ' : (uint_fast8_t)number + '0');
      if ((column + 1) % 3 == 0) {
        printf(" |");
      }
    }
    printf("\n");
    if ((row + 1) % 3 == 0) {
      printf("-------------------------\n");
    }
  }
}

void solve(uint_fast8_t *sudoku) {
  struct timeval start_time, stop_time;
  gettimeofday(&start_time, NULL);

  bool solved = false;
  backtrack(sudoku, &solved);

  gettimeofday(&stop_time, NULL);
  printf("Solved in %" PRIu64 " microseconds.\n", difftime_in_micros(&start_time, &stop_time));
}

//
// BACKTRACKING ALGORITHM
//

void backtrack(uint_fast8_t *partial_candidate, bool *solved) {
  if (!is_valid(partial_candidate)) {
    return;
  } else if (is_complete(partial_candidate)) {
    printf("Complete!\n");
    *solved = true;
    print(partial_candidate);
    return;
  }

  int_fast8_t working_index = find_best_index(partial_candidate);
  if (working_index < 0) {
    return;
  }

  uint_fast8_t candidate[81];
  copy(candidate, partial_candidate);
  for (uint_fast8_t next_value = 1; next_value < 10; ++next_value) {
    generate_next_candidate(candidate, working_index, next_value);
    backtrack(candidate, solved);
    if (*solved) {
      return;
    }
  }
}

void generate_next_candidate(uint_fast8_t *candidate, int_fast8_t working_index, uint_fast8_t next_value) {
  candidate[working_index] = next_value;
}

//
// TOOLS
//

uint64_t difftime_in_micros(struct timeval *start_time, struct timeval *stop_time) {
  return 1000000L * stop_time->tv_sec + stop_time->tv_usec - 1000000L * start_time->tv_sec - start_time->tv_usec;
}

void copy(uint_fast8_t *new_sudoku, uint_fast8_t *old_sudoku) {
  for (uint_fast8_t i = 0; i < 81; ++i) {
    new_sudoku[i] = old_sudoku[i];
  }
}

bool is_valid(uint_fast8_t *sudoku) {
  uint_fast8_t temp[9];
  for (uint_fast8_t index = 0; index < 9; ++index) {
    if (has_dupes(row(sudoku, index))) {
      return false;
    };
    if (has_dupes(column(sudoku, index, temp))) {
      return false;
    };
    if (has_dupes(quadrant(sudoku, index, temp))) {
      return false;
    };
  }
  return true;
}

bool has_dupes(uint_fast8_t *v) {
  uint_fast16_t bitmap = 0;
  for (uint_fast8_t i = 0; i < 9; ++i) {
    if (v[i] != 0) {
      if (is_bit_set(bitmap, v[i])) {
        return true;
      } else {
        set_bit(bitmap, v[i]);
      }
    }
  }
  return false;
}

bool is_complete(uint_fast8_t *sudoku) {
  uint_fast8_t filled_squares = 0;
  for (uint_fast8_t row = 0; row < 9; ++row) {
    for (uint_fast8_t column = 0; column < 9; ++column) {
      if (sudoku[9 * row + column] != 0) {
        ++filled_squares;
      }
    }
  }
  return (filled_squares == 81);
}

int_fast8_t find_best_index(uint_fast8_t *sudoku) {
  for (uint_fast8_t row = 0; row < 9; ++row) {
    for (uint_fast8_t column = 0; column < 9; ++column) {
      if (sudoku[9 * row + column] == 0) {
        return 9 * row + column;
      }
    }
  }
  return -1;
}

uint_fast8_t *row(uint_fast8_t *sudoku, uint_fast8_t row_index) {
  return sudoku + 9 * row_index;
}

uint_fast8_t *column(uint_fast8_t *sudoku, uint_fast8_t column_index, uint_fast8_t *column) {
  for (uint_fast8_t index = 0; index < 9; ++index) {
    column[index] = sudoku[9 * index + column_index];
  }
  return column;
}

uint_fast8_t *quadrant(uint_fast8_t *sudoku, uint_fast8_t quadrant_index, uint_fast8_t *quadrant) {
  for (uint_fast8_t index = 0; index < 3; ++index) {
    quadrant[3 * index] = (sudoku[9 * (3 * ((quadrant_index / 3)) + index) + 3 * (quadrant_index % 3)]);
    quadrant[3 * index + 1] = (sudoku[9 * (3 * ((quadrant_index / 3)) + index) + 3 * (quadrant_index % 3) + 1]);
    quadrant[3 * index + 2] = (sudoku[9 * (3 * ((quadrant_index / 3)) + index) + 3 * (quadrant_index % 3) + 2]);
  }
  return quadrant;
}
