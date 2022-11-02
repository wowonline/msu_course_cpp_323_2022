#pragma once

#include "graph.hpp"

namespace uni_course_cpp {
namespace printing {
std::string print_graph(const Graph& graph);
std::string color_to_string(Graph::Edge::Color color);
// void write_to_file(const std::string& content, const std::string& file_name);
}  // namespace printing
}  // namespace uni_course_cpp
