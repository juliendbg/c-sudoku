# c-sudoku

This program is a fast sudoku solver implemented in C using a backtracking algorithm.

## Build

Use the provided Makefile, no dependencies are required.

```
make
```

## Usage

Some sample grids are provided in the `samples/` folder:

```
./solver samples/06-worldhardest.txt
```

## Is it any good?

Yes.

It solves the World's Hardest Sudoku ([source](https://gizmodo.com/can-you-solve-the-10-hardest-logic-puzzles-ever-created-1064112665)) in less than 100 milliseconds .

```
$ sysctl -a |grep cpu.brand_string
machdep.cpu.brand_string: Intel(R) Core(TM) i7-4870HQ CPU @ 2.50GHz

$ ./solver samples/06-worldhardest.txt
-------------------------
| 8     |       |       |
|     3 | 6     |       |
|   7   |   9   | 2     |
-------------------------
|   5   |     7 |       |
|       |   4 5 | 7     |
|       | 1     |   3   |
-------------------------
|     1 |       |   6 8 |
|     8 | 5     |   1   |
|   9   |       | 4     |
-------------------------
Complete!
-------------------------
| 8 1 2 | 7 5 3 | 6 4 9 |
| 9 4 3 | 6 8 2 | 1 7 5 |
| 6 7 5 | 4 9 1 | 2 8 3 |
-------------------------
| 1 5 4 | 2 3 7 | 8 9 6 |
| 3 6 9 | 8 4 5 | 7 2 1 |
| 2 8 7 | 1 6 9 | 5 3 4 |
-------------------------
| 5 2 1 | 9 7 4 | 3 6 8 |
| 4 3 8 | 5 2 6 | 9 1 7 |
| 7 9 6 | 3 1 8 | 4 5 2 |
-------------------------
Solved in 51781 microseconds.
```
