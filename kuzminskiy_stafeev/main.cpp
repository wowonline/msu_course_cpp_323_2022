#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_logger.hpp"
#include "graph_printing.hpp"

namespace fs = std::filesystem;

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string generation_started_string(int i) {
  return get_current_date_time() + " Graph " + std::to_string(i) +
         ", Generation Started";
}

std::string generation_finished_string(int i,
                                       const std::string& graph_description) {
  return get_current_date_time() + " Graph " + std::to_string(i) +
         ", Generation Finished {\n" + graph_description + "\n}";
}

void write_to_file(const std::string& str, const std::string& filename) {
  std::ofstream out(filename);
  out << str << std::endl;
}

bool is_number(std::string s) {
  for (int i = 0; i < s.length(); i++) {
    if (isdigit(s[i]) == false) {
      return false;
    }
  }

  return true;
}

int handle_depth_input() {
  std::string input;
  do {
    std::cout << "Input depth of graph: ";
    std::cin >> input;
    if (is_number(input) && std::stoi(input) >= 0) {
      return std::stoi(input);
    }

    std::cout << "Incorrect depth" << std::endl;
  } while (true);
}

int handle_new_vertices_count_input() {
  std::string input;
  do {
    std::cout << "Input new_vertices_count: ";
    std::cin >> input;
    if (is_number(input) && std::stoi(input) >= 0) {
      return std::stoi(input);
    }

    std::cout << "Incorrect new_vertices_count" << std::endl;
  } while (true);
}

int handle_graphs_count_input() {
  std::string input;
  do {
    std::cout << "Input graphs count: ";
    std::cin >> input;
    if (is_number(input) && std::stoi(input) >= 0) {
      return std::stoi(input);
    }

    std::cout << "Incorrect graphs count!" << std::endl;
    std::cout << "Input graphs count: ";
  } while (true);
}

void prepare_temp_directory() {
  if (!fs::exists(config::kTempDirectoryPath)) {
    if (!fs::create_directories(config::kTempDirectoryPath)) {
      throw std::runtime_error("Temp directory error!");
    }
  }
}

int main() {
  using namespace uni_course_cpp;

  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(std::move(params));
  Logger& logger = Logger::get_logger();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate();

    const auto graph_description = printing::print_graph(graph);
    logger.log(generation_finished_string(i, graph_description));

    const auto graph_json = printing::json::print_graph(graph);
    std::cout << graph_json << std::endl;
    write_to_file(graph_json, std::string(config::kTempDirectoryPath) +
                                  "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
