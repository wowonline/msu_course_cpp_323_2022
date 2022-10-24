#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_printing.hpp"

void write_to_file(const std::string& str, const std::string& filename) {
  std::ofstream out(filename);
  out << str << std::endl;
}

int handle_depth_input() {
  int input;
  std::cout << "Input depth of graph: ";
  while (std::cin >> input) {
    if (input >= 0) {
      return input;
    }

    std::cout << "Incorrect depth" << std::endl;
    std::cout << "Input depth of graph: ";
  }

  throw std::runtime_error("Bad input depth!");

  return input;
}

int handle_new_vertices_count_input() {
  int input;
  std::cout << "Input new_vertices_count: ";
  while (std::cin >> input) {
    if (input >= 0) {
      return input;
    }

    std::cout << "Incorrect new_vertices_count" << std::endl;
    std::cout << "Input new_vertices_count: ";
  }

  throw std::runtime_error("Bad input new_vertices_count!");

  return input;
}

int main() {
  int depth;
  int new_vertices_count;
  try {
    depth = handle_depth_input();
    new_vertices_count = handle_new_vertices_count_input();
  } catch (const std::bad_exception& e) {
    std::cerr << "Caught " << e.what() << std::endl;
    return 1;
  }

  auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(std::move(params));
  const auto graph = generator.generate();

  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
