#include <fstream>
#include <iostream>

#include "graph.hpp"
#include "graph_generator.hpp"
#include "printing.hpp"

void write_to_file(const std::string& data, const std::string& file_path) {
  std::ofstream out(file_path, std::ios::out | std::ios::trunc);
  out << data << "\n";
  out.close();
}

int main() {
  const auto graph = GraphGenerator(GraphGenerator::Params(0, 0)).generate();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
