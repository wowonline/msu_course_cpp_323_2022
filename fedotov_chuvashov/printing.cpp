#include "printing.hpp"
#include <sstream>

namespace uni_course_cpp {
namespace printing {
std::string print_edge_color(Graph::Edge::Color color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    case Graph::Edge::Color::Green:
      return "green";
  }
}

std::string print_graph(const Graph& graph) {
  std::ostringstream result;

  result << "{\n\tdepth: " << graph.depth()
         << ",\n\tvertices: {amount: " << graph.vertices().size()
         << ", distribution: [";
  for (int i = 1; i <= graph.depth(); ++i) {
    result << graph.vertices_at_depth(i).size();
    if (i != graph.depth())
      result << ", ";
  }
  result << "]},\n\tedges: {amount: " << graph.edges().size()
         << ", distribution: {grey: "
         << graph.get_color_amount(Graph::Edge::Color::Grey)
         << ", green: " << graph.get_color_amount(Graph::Edge::Color::Green)
         << ", yellow: " << graph.get_color_amount(Graph::Edge::Color::Yellow)
         << ", red: " << graph.get_color_amount(Graph::Edge::Color::Red)
         << "}}\n}";

  return result.str();
}

namespace json {
std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::ostringstream result;
  bool is_first = true;
  const auto& edges_ids = graph.connected_edge_ids(vertex.id());
  result << "\n{ \"id\": " << vertex.id()
         << ", \"depth\": " << graph.get_vertex_depth(vertex.id())
         << ", \"edge_ids\": [";
  for (const auto edge_id : edges_ids) {
    if (!is_first)
      result << ", " << edge_id;
    else {
      result << edge_id;
      is_first = false;
    }
  }
  result << "] }";
  return result.str();
}

std::string print_edge(const Graph::Edge& edge) {
  std::ostringstream result;
  result << "\n{\"id\": " << edge.id() << ", \"color\": \""
         << print_edge_color(edge.color()) << "\", \"vertex_ids\": ["
         << edge.from_vertex_id() << ", " << edge.to_vertex_id() << "]}";
  return result.str();
}

std::string print_graph(const Graph& graph) {
  std::ostringstream result;
  result << "{\"depth\": " << graph.depth() << ",\n \"vertices\": [";
  bool is_first = true;
  for (const auto& [_, vertex] : graph.vertices()) {
    if (!is_first) {
      result << ", ";
    } else {
      is_first = false;
    }
    result << print_vertex(vertex, graph);
  }

  result << "],\n\"edges\": [";
  is_first = true;
  for (const auto& [_, edge] : graph.edges()) {
    if (!is_first) {
      result << ", ";
    } else {
      is_first = false;
    }
    result << print_edge(edge);
  }
  result << "]}\n";
  return result.str();
}
};  // namespace json
};  // namespace printing
}  // namespace uni_course_cpp
