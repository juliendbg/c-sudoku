#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define is_bit_set(bitmap, bitindex)  ((bitmap & (1 << (bitindex - 1))) != 0)
#define set_bit(bitmap, bitindex)     (bitmap |= (1 << (bitindex - 1)))

void backtrack(int *partial_candidate, bool *solved);
void generate_next_candidate(int *candidate, int working_index, int next_value);
unsigned long difftime_in_micros(struct timeval *start_time, struct timeval *stop_time);
void print_vector(int *vector, int length);
void copy(int *new_sudoku, int *old_sudoku);
bool is_valid(int *sudoku);
bool has_dupes(int * v);
bool is_complete(int *sudoku);
int find_best_index(int *sudoku);
int *row(int *sudoku, int row_index, int *row);
int *column(int *sudoku, int column_index, int *column);
int *quadrant(int *sudoku, int quadrant_index, int *quadrant);

//
// PUBLIC METHODS
//

void print(int *sudoku) {
  printf("-------------------------\n");
  for (int row = 0 ; row < 9 ; ++row) {
    printf("|");
    for(int column = 0 ; column < 9 ; ++column) {
      int number = sudoku[9*row+column];
      printf(" %c", number == 0 ? ' ' :  (char) number+'0');
      if ((column+1) % 3 == 0) { printf(" |"); }
    }
    printf("\n");
    if ((row+1) % 3 == 0) {
      printf("-------------------------\n");
    }
  }
}

void solve(int *sudoku) {
  struct timeval start_time, stop_time;
  gettimeofday(&start_time, NULL);

  bool solved = false;
  backtrack(sudoku, &solved);

  gettimeofday(&stop_time, NULL);
  printf("Solved in %lu microseconds.\n", difftime_in_micros(&start_time, &stop_time));
}

//
// BACKTRACKING ALGORITHM
//

void backtrack(int *partial_candidate, bool *solved) {
  if (!is_valid(partial_candidate)) {
    return;
  } else if (is_complete(partial_candidate)) {
    printf("Complete!\n");
    *solved = true;
    print(partial_candidate);
    return;
  }

  int working_index = find_best_index(partial_candidate);
  if (working_index < 0) {
    return;
  }

  int candidate[81];
  copy(candidate, partial_candidate);
  for (int next_value = 1 ; next_value < 10 ; ++next_value) {
    generate_next_candidate(candidate, working_index, next_value);
    backtrack(candidate, solved);
    if (*solved) {return;}
  }
}

void generate_next_candidate(int *candidate, int working_index, int next_value) {
  candidate[working_index] = next_value;
}

//
// TOOLS
//

unsigned long difftime_in_micros(struct timeval *start_time, struct timeval *stop_time) {
  return 1000000L*stop_time->tv_sec + stop_time->tv_usec
       - 1000000L*start_time->tv_sec - start_time->tv_usec;
}

void print_vector(int *vector, int length) {
  for(int i = 0; i < length; ++i) {
    printf("%d ", vector[i]);
  }
  printf("\n");
}

void copy(int *new_sudoku, int *old_sudoku) {
  for (int i = 0 ; i < 81 ; ++i) {
    new_sudoku[i] = old_sudoku[i];
  }
}

bool is_valid(int *sudoku) {
  int temp[9];
  for (int index = 0 ; index < 9 ; ++index) {
    if (has_dupes(row(sudoku, index, temp))) { return false; };
    if (has_dupes(column(sudoku, index, temp))) { return false; };
    if (has_dupes(quadrant(sudoku, index, temp))) { return false; };
  }
  return true;
}

bool has_dupes(int *v) {
  int bitmap = 0;
  for (int i = 0 ; i < 9 ; ++i) {
    if (v[i] != 0) {
      if (is_bit_set(bitmap,v[i])) {
        return true;
      } else {
        set_bit(bitmap, v[i]);
      }
    }
  }
  return false;
}

bool is_complete(int *sudoku) {
  int filled_squares = 0;
  for (int row = 0 ; row < 9 ; ++row) {
    for(int column = 0 ; column < 9 ; ++column) {
      if (sudoku[9*row+column] != 0) {
        ++filled_squares;
      }
    }
  }
  return (filled_squares == 81);
}

int find_best_index(int *sudoku) {
  for (int row = 0 ; row < 9 ; ++row) {
    for(int column = 0 ; column < 9 ; ++column) {
      if (sudoku[9*row+column] == 0) {
        return 9*row+column;
      }
    }
  }
  return -1;
}

int *row(int *sudoku, int row_index, int *row) {
  for (int index = 0 ; index < 9 ; ++index) {
    row[index] = sudoku[9*row_index+index];
  }
  return row;
}

int *column(int *sudoku, int column_index, int *column) {
  for (int index = 0 ; index < 9 ; ++index) {
    column[index] = sudoku[9*index+column_index];
  }
  return column;
}

int *quadrant(int *sudoku, int quadrant_index, int *quadrant) {
  for (int index = 0 ; index < 3 ; ++index) {
    quadrant[3*index] = (sudoku[9*(3*((quadrant_index/3))+index)+3*(quadrant_index%3)]);
    quadrant[3*index+1] = (sudoku[9*(3*((quadrant_index/3))+index)+3*(quadrant_index%3)+1]);
    quadrant[3*index+2] = (sudoku[9*(3*((quadrant_index/3))+index)+3*(quadrant_index%3)+2]);
  }
  return quadrant;
}

