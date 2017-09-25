all:
	cc -std=c99 -O3 -march=native -o solver src/solver.c src/grid.c

debug:
	cc -std=c99 -O0 -g -Wall -Wextra -pedantic -o solver src/solver.c src/grid.c

gprof:
	cc -std=c99 -pg -o solver src/solver.c src/grid.c
	./solver samples/06-worldhardest.txt
	gprof solver gmon.out > prof_output

