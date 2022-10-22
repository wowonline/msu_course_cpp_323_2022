CC=clang++

LFLAGS = -std=c++17 -Werror
CFLAGS = -std=c++17 -Werror
OBJ = graph.o graph_generator.o printing.o main.o

all: prog

prog: $(OBJ)
	${CC} $(OBJ) $(LFLAGS) -o $@

.cpp.o:
	${CC} -g ${CFLAGS} -c $< -o $@

main.o: printing.hpp graph.hpp graph_generator.hpp

clean:
	rm -rf *.o prog
