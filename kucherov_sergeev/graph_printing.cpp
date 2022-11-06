#include "graph_printing.hpp"
#include <map>

namespace uni_course_cpp {
namespace printing {
namespace {
std::vector<int> get_vertices_depth_distribution(const Graph& graph) {
  std::vector<int> vertices_depth_distribution = {};
  const auto depth = graph.get_depth();

  for (Graph::Depth i = 0; i < depth; i++) {
    vertices_depth_distribution.push_back(graph.get_depth_vertex_ids(i).size());
  }

  return vertices_depth_distribution;
}

std::unordered_map<Graph::Edge::Color, int> get_edges_color_distribution(
    const Graph& graph) {
  std::unordered_map<Graph::Edge::Color, int> edges_color_distribution{
      {Graph::Edge::Color::Green, 0},
      {Graph::Edge::Color::Red, 0},
      {Graph::Edge::Color::Yellow, 0},
      {Graph::Edge::Color::Grey, 0}};

  const auto& edges = graph.get_edges();

  for (const auto& [edge_id, edge] : edges) {
    edges_color_distribution[edge.color()] += 1;
  }

  return edges_color_distribution;
}
}  // namespace

std::string print_edge_color(Graph::Edge::Color color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    default:
      return "invalid color";
  }
}

std::string print_vertices_info(const Graph& graph) {
  const auto vertices_depth_distribution =
      get_vertices_depth_distribution(graph);
  std::string vertices_string =
      "vertices: {amount: " + std::to_string(graph.get_vertices().size()) +
      ", distribution: [";

  if (vertices_depth_distribution.size() != 0) {
    for (const auto vertices_count : vertices_depth_distribution) {
      vertices_string += std::to_string(vertices_count) + ", ";
    }
    vertices_string.pop_back();
    vertices_string.pop_back();
  }
  vertices_string += "]},";

  return vertices_string;
}

std::string print_edges_info(const Graph& graph) {
  const auto edges_color_distribution = get_edges_color_distribution(graph);
  std::string edges_string =
      "edges: {amount: " + std::to_string(graph.get_edges().size()) +
      ", distribution: {";
  edges_string +=
      "grey: " +
      std::to_string(edges_color_distribution.at(Graph::Edge::Color::Grey)) +
      ", ";
  edges_string +=
      "green: " +
      std::to_string(edges_color_distribution.at(Graph::Edge::Color::Green)) +
      ", ";
  edges_string +=
      "yellow: " +
      std::to_string(edges_color_distribution.at(Graph::Edge::Color::Yellow)) +
      ", ";
  edges_string +=
      "red: " +
      std::to_string(edges_color_distribution.at(Graph::Edge::Color::Red)) +
      "}}";

  return edges_string;
}

std::string print_graph(const Graph& graph) {
  std::string depth_string =
      "depth: " + std::to_string(graph.get_depth()) + ",";
  std::string vertices_string = print_vertices_info(graph);
  std::string edges_string = print_edges_info(graph);

  return "{\n\t" + depth_string + "\n\t" + vertices_string + "\n\t" +
         edges_string + "\n}";
}
}  // namespace printing
}  // namespace uni_course_cpp
