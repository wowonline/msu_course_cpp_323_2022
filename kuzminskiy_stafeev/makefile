CC=clang++

LFLAGS = -std=c++17 -Werror
CFLAGS = -std=c++17 -Werror
OBJ = classes.o printing.o main.o

all: prog

prog: $(OBJ)
	${CC} $(OBJ) $(LFLAGS) -o $@

.cpp.o:
	${CC} -g ${CFLAGS} -c $< -o $@

main.o: classes.hpp printing.hpp

clean:
	rm -rf *.o prog
