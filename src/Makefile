SANIT = -O3 -std=c11
OS=$(shell uname)
ifeq ($(OS), Linux)
	LIBS=-lcheck -lpthread -lrt -lm -lsubunit
else
	LIBS=-lcheck
endif

all: test gcov_report

matrix.a: matrix.o
	ar rcs matrix.a matrix.o
	ranlib matrix.a

matrix.o: matrix.c
	gcc -c matrix.c

test: matrix.a
	gcc $(SANIT) --coverage test_matrix.c matrix.a $(LIBS)
	./a.out

gcov_report:
	gcc --coverage matrix.c -c -o matrix_coverage.o
	gcc test_matrix.c -c -o test_coverage.o
	gcc matrix_coverage.o test_coverage.o $(LIBS) -lgcov 
	./a.out
	gcovr --html --html-details -o report.html
	open report.html

style_check:
	clang-format -style=google -i *.c
	clang-format -style=google -i *.h

clean:
	rm -rf *.o *.a *.gcno *.gcda *.out *.html *.css
