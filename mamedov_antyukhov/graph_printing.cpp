#include "graph_printing.hpp"
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include "graph.hpp"

namespace uni_course_cpp {
namespace printing {
std::string print_edge_color(Graph::Edge::Color color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Red:
      return "red";
    case Graph::Edge::Color::Yellow:
      return "yellow";
  }

  return "";
}

std::string print_graph(const Graph& graph) {
  std::stringstream data_graph;
  const auto& vertices_depth = graph.get_depth_levels();
  const auto& edges = graph.get_edges();
  data_graph << "{\n\tdepth:" << graph.depth() << ",\n";
  data_graph << "\tvertices: { amount: " << vertices_depth.size()
             << ", distribution: [";
  for (int i = 0; i < vertices_depth.size() - 1; ++i) {
    data_graph << vertices_depth[i].size() << ", ";
  }
  data_graph << vertices_depth[vertices_depth.size() - 1].size() << "]},\n";
  data_graph << "\tedges: {amount: " << edges.size() << ", distribution: {";

  std::vector<std::pair<Graph::Edge::Color, std::string>> colors = {
      {Graph::Edge::Color::Grey, "grey: "},
      {Graph::Edge::Color::Green, "green: "},
      {Graph::Edge::Color::Yellow, "yellow: "},
      {Graph::Edge::Color::Red, "red: "}};

  for (const auto& p : colors) {
    data_graph << p.second << graph.get_colored_edge_ids(p.first).size();

    if (p.first == Graph::Edge::Color::Red)
      data_graph << "}}\n}";
    else
      data_graph << ", ";
  }

  return data_graph.str();
}

}  // namespace printing
}  // namespace uni_course_cpp
