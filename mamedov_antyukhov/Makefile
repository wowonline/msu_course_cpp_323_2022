CC = clang++
FLAGS = -std=c++17 -Werror 
PROGS.O = graph.o Logger.o graph_printing.o main.o graph_generator.o graph_json_printing.o
PROGS.CPP = graph.cpp Logger.cpp graph_printing.cpp graph_generator.cpp graph_json_printing.cpp main.cpp
PROGS.HPP = graph.hpp Logger.hpp graph_printing.hpp graph_generator.hpp graph_json_printing.hpp config.hpp
RES = main
GRAPH = graph.json

all: $(PROGS.O)
	$(CC) $(PROGS.O) -o $(RES) $(FLAGS)

%.o : %.cpp
	$(CC) -c $< $(CFLAGS)

run:
	./$(RES)

clean:
	rm -f $(PROGS.O)

delete: clean
	rm -f $(RES)

erase: delete
	rm -f $(GRAPH)

ready:
	rm -rf $(PROGS.O)

clang-format:
	clang-format -i -style=Chromium $(PROGS.CPP) $(PROGS.HPP)

