#include <fstream>
#include <iostream>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "printing.hpp"

namespace {

void write_to_file(const std::string& output_string,
                   const std::string& file_name) {
  std::ofstream output(file_name);
  if (output.is_open()) {
    output << output_string;
  } else {
    std::cout << "Unable to open file";
  }
}

constexpr int kInputSize = 256;

int handle_depth_input() {
  std::cout << "Enter depth: ";
  int depth = 0;
  while (!(std::cin >> depth) || depth < 0) {
    std::cout << "Invalid value. Please, try again." << std::endl
              << "Enter depth: ";
    std::cin.clear();
    std::cin.ignore(kInputSize, '\n');
  }
  return depth;
}

int handle_new_vertices_count_input() {
  std::cout << "Enter new vertices count: ";
  int new_vertices_count = 0;
  while (!(std::cin >> new_vertices_count) || new_vertices_count < 0) {
    std::cout << "Invalid value. Please, try again." << std::endl
              << "Enter new vertices count: ";
    std::cin.clear();
    std::cin.ignore(kInputSize, '\n');
  }
  return new_vertices_count;
}
}  // namespace

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  auto params =
      uni_course_cpp::GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = uni_course_cpp::GraphGenerator(std::move(params));
  const auto graph = generator.generate();

  const auto graph_json = uni_course_cpp::printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}

