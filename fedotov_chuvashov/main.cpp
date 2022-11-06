#include <filesystem>
#include <fstream>
#include <iostream>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "logger.hpp"
#include "printing.hpp"
#include "printing_json.hpp"

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

int handle_graphs_count_input() {
  std::cout << "Enter graph count: ";
  int graph_count = 0;
  while (!(std::cin >> graph_count) || graph_count < 0) {
    std::cout << "Invalid value. Please, try again." << std::endl
              << "Enter graph count: ";
    std::cin.clear();
    std::cin.ignore(kInputSize, '\n');
  }
  return graph_count;
}

void prepare_temp_directory() {
  std::filesystem::create_directory(uni_course_cpp::config::kTempDirectoryPath);
}

}  // namespace

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  auto params =
      uni_course_cpp::GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = uni_course_cpp::GraphGenerator(std::move(params));
  auto& logger = uni_course_cpp::Logger::get_logger();

  for (int i = 0; i < graphs_count; ++i) {
    logger.log(uni_course_cpp::generation_started_string(i));
    const auto graph = generator.generate();

    const auto graph_description = uni_course_cpp::printing::print_graph(graph);
    logger.log(
        uni_course_cpp::generation_finished_string(i, graph_description));

    const auto graph_json = uni_course_cpp::json::print_graph(graph);
    write_to_file(graph_json, "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
