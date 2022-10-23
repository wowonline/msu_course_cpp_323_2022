#include <fstream>
#include <iostream>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "printing.hpp"

void write_to_file(const std::string& output_string,
                   const std::string& file_name) {
  std::ofstream output(file_name);
  if (output.is_open()) {
    output << output_string;
  } else {
    std::cout << "Unable to open file";
  }
}

int handle_depth_input() {
  int depth;
  while (std::cin >> depth) {
    if (depth >= 0) {
      return depth;
    }
    std::cout << "Invalid depth. Please, try again." << std::endl;
  }

  throw std::runtime_error("Invalid input");
}

int handle_new_vertices_count_input() {
  int new_vertices_count;
  while (std::cin >> new_vertices_count) {
    if (new_vertices_count >= 0) {
      return new_vertices_count;
    }
    std::cout << "Invalid new vertices count. Please, try again." << std::endl;
  }

  throw std::runtime_error("Invalid input");
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();

  auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(std::move(params));
  const auto graph = generator.generate();

  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
