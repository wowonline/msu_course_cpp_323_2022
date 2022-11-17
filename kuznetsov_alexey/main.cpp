#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printing.hpp"
#include <iostream>
#include <string>

void write_to_file(const std::string& graph_json,
                   const std::string& file_name) {
  std::ofstream file(file_name);
  file << graph_json;
}

Graph::Depth handle_depth_input() {
  Graph::Depth depth;
  std::cin >> depth;
  return depth;
}

int handle_new_vertices_count_input() {
  int count_vertices;
  std::cin >> count_vertices;
  return count_vertices;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();

  auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(std::move(params));
  const auto graph = generator.generate();

  const auto graph_json = printing::json::print_graph(graph);
  write_to_file(graph_json, "graph.json");

  return 0;
}
