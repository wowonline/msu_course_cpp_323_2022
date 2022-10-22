#include "graph.hpp"
#include "graph_generator.hpp"
#include "printing.hpp"
#include <fstream>

void write_to_file(const std::string& str, const std::string& filename) {
  std::ofstream out(filename);
  out << str << std::endl;
}

int handle_depth_input() {
  int input;
  while (std::cin >> input) {
    if (input >= 0) {
      return input;
    }

    std::cout << "Incorrect depth" << std::endl;
  }

  std::runtime_error("Bad input depth!");

  return input;
}

int handle_new_vertices_count_input() {
  int input;
  while (std::cin >> input) {
    if (input >= 0) {
      return input;
    }

    std::cout << "Incorrect new_vertices_count" << std::endl;
  }

  std::runtime_error("Bad input new_vertices_count!");

  return input;
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
