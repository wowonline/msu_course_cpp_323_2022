#include "graph_printing.hpp"
#include <sstream>
#include <string>
#include <unordered_map>
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

  std::unordered_map<int, int> colors_amount = graph.get_colors_amount();
  data_graph << "grey: "
             << colors_amount[static_cast<int>(Graph::Edge::Color::Grey)]
             << ", ";
  data_graph << "green: "
             << colors_amount[static_cast<int>(Graph::Edge::Color::Green)]
             << ", ";
  data_graph << "yellow: "
             << colors_amount[static_cast<int>(Graph::Edge::Color::Yellow)]
             << ", ";
  data_graph << "red: "
             << colors_amount[static_cast<int>(Graph::Edge::Color::Red)]
             << "}}\n}";

  return data_graph.str();
}

}  // namespace printing
}  // namespace uni_course_cpp