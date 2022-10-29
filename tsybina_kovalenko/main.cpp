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

GraphGenerator::Params ask_params() {
  Graph::Depth depth;
  bool is_depth_correct = false;
  do {
    std::cout << "Enter graph depth: " << std::flush;
    std::cin >> depth;
    is_depth_correct = depth >= 0;
    if (!is_depth_correct) {
      std::cout << "Alas, depth can't be negative :(\n";
    }
  } while (!is_depth_correct);

  int new_vertices_count;
  bool is_new_vertices_count_correct = false;
  do {
    std::cout << "Enter new vertices count for each vertex: " << std::flush;
    std::cin >> new_vertices_count;
    is_new_vertices_count_correct = new_vertices_count >= 0;
    if (!is_new_vertices_count_correct) {
      std::cout << "Alas, new vertices count can't be negative :(\n";
    }
  } while (!is_new_vertices_count_correct);

  return {depth, new_vertices_count};
}

int main() {
  auto params = ask_params();

  const auto graph = GraphGenerator(std::move(params)).generate();
  for (int vertex_id = 0; vertex_id < graph.vertices().size(); ++vertex_id) {
    std::cout << vertex_id << ": " << graph.vertex_depths().at(vertex_id)
              << std::endl;
  }
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
