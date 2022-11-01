#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include "graph.hpp"
#pragma once

namespace uni_course_cpp {

namespace printing {
std::string print_edge_color(const Graph::Edge::Color& color);
std::string print_graph(const Graph& graph);
std::string get_current_date_time();
std::string generation_started_string(int i);
std::string generation_finished_string(int i, std::string graph_description);
}  // namespace printing

}  // namespace uni_course_cpp
