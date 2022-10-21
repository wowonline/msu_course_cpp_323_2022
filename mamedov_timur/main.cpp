#include <fstream>
#include <iostream>
#include <string>
#include "graph.hpp"
#include "printing.hpp"

Graph generate_graph() {
  int depth = 4;
  int new_vertices_count = 3;

  auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(std::move(params));
  const auto graph = generator.generate();

  return graph;
}

void write_to_file(const std::string& graph_str, const std::string& file_name) {
  std::ofstream fout;
  fout.open(file_name);
  fout << graph_str;
  fout.close();
}

int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  write_to_file(graph_json, "graph.json");

  return 0;
}
