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
  for (int vertex_id = 0; vertex_id < graph.vertices().size(); ++vertex_id) {
    std::cout << vertex_id << ": " << graph.vertex_depths().at(vertex_id)
              << std::endl;
  }
  // const auto graph_json = printing::json::print_graph(graph);
  // std::cout << graph_json << std::endl;
  // write_to_file(graph_json, "graph.json");

  return 0;
}
