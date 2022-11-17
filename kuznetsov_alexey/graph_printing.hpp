#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "graph.hpp"
#include "graph_generator.hpp"

 namespace printing {
 namespace json {

 std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph);

 std::string print_edge(const Graph::Edge& edge);

 std::string print_graph(const Graph& graph);

 }  // namespace json
 }
