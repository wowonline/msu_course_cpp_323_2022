CC = clang++
FLAGS = -std=c++17 -Werror 

PROGS.O = graph.o logger.o graph_printing.o graph_generator.o graph_json_printing.o main.o
PROGS.CPP = graph.cpp logger.cpp graph_printing.cpp graph_generator.cpp graph_json_printing.cpp main.cpp
PROGS.HPP = graph.hpp logger.hpp graph_printing.hpp graph_generator.hpp graph_json_printing.hpp config.hpp
RES = main
GRAPH = graph.json

all: $(PROGS.O)
	$(CC) $(PROGS.O) -o $(RES) $(FLAGS) -std=c++17

%.o : %.cpp
	$(CC) -c $< $(FLAGS)

run:
	./$(RES)

clean:
	rm -f $(PROGS.O)

delete: clean
	rm -f $(RES)

erase: delete
	rm -f $(GRAPH)

ready:
	rm -rf $(PROGS.O) $(RES)

clang-format:
	clang-format -i -style=Chromium $(PROGS.HPP) $(PROGS.CPP)

