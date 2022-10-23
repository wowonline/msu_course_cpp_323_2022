#include <fstream>
#include <iostream>
#include <string>
#include "graph.hpp"
#include "printing.hpp"

int handle_depth_input() {
  int output;
  while (std::cin >> output) {
    if (output >= 0) {
      return output;
    }

    std::cout << "Incorrect depth!" << std::endl;
  }
  std::runtime_error("Depth has not been passed!");
  return output;
}

int handle_new_vertices_count_input() {
  int output;
  while (std::cin >> output) {
    if (output >= 0) {
      return output;
    }
    std::cout << "Incorrect new_vertices_count!" << std::endl;
  }

  std::runtime_error("New_vertices_count has been passed!");

  return output;
}

void write_to_file(const std::string& graph_str, const std::string& file_name) {
  std::ofstream fout;
  fout.open(file_name);
  fout << graph_str;
  fout.close();
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
