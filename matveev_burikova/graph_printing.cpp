#include "graph_printing.hpp"

namespace uni_course_cpp {
namespace printing {
std::string print_graph(const Graph& graph) {
  std::string graph_info_string;
  graph_info_string =
      "{\n  depth: " + std::to_string(graph.depth()) +
      ",\n  vertices: {amount: " + std::to_string(graph.get_vertices().size()) +
      ", distribution: [";
  for (int depth = 1; depth <= graph.depth(); ++depth) {
    graph_info_string +=
        std::to_string(graph.get_vertex_ids_on_depth(depth).size()) + ", ";
  }
  graph_info_string.erase(graph_info_string.length() - 2,
                          graph_info_string.length());
  graph_info_string +=
      "]},\n  edges: {amount: " + std::to_string(graph.get_edges().size()) +
      ", distribution: {";
  int color_distribution_counts[sizeof(Graph::Edge::Color)] = {0};
  for (const auto& [edge_id, edge] : graph.get_edges())
    color_distribution_counts[(int)edge.color()]++;
  for (int color_number = 0; color_number < sizeof(Graph::Edge::Color);
       color_number++) {
    graph_info_string +=
        printing::print_edge_color((Graph::Edge::Color)color_number) + ": " +
        std::to_string(color_distribution_counts[color_number]) + ", ";
  }
  graph_info_string.erase(graph_info_string.length() - 2,
                          graph_info_string.length());
  return graph_info_string + "}}\n}";
}

std::string print_edge_color(Graph::Edge::Color color) {
  switch (color) {
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    default:
      return "grey";
  }
}
}  // namespace printing
}  // namespace uni_course_cpp
