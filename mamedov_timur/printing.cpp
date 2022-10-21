#include "printing.hpp"
#include <sstream>
#include <string>
#include <unordered_map>
#include "graph.hpp"

namespace printing {
std::string print_edge_color(const Graph::Edge::Color& color) {
  std::unordered_map<Graph::Edge::Color, std::string> table = {
      {Graph::Edge::Color::Grey, "grey"},
      {Graph::Edge::Color::Green, "green"},
      {Graph::Edge::Color::Red, "red"},
      {Graph::Edge::Color::Yellow, "yellow"}};
  return table[color];
}
namespace json {
std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::stringstream json_string;
  json_string << "\n\t\t{ \"id\":" << vertex.id() << ", \"edges_ids\": [";
  const auto connections = graph.get_edges_of_vertex(vertex.id());

  auto it = connections.begin(), prev_it = it;
  ++it;

  for (; it != connections.end(); ++it, ++prev_it) {
    json_string << *prev_it << ", ";
  }
  json_string << *prev_it;

  json_string << "] }";

  return json_string.str();
}

std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  std::stringstream json_string;
  json_string << "\n\t\t{ \"id\": " << edge.id() << ", \"vertex_ids\": ["
              << edge.get_first_vertex_id() << ", "
              << edge.get_second_vertex_id() << "], \"color\": \""
              << print_edge_color(edge.color()) << "\" }";

  return json_string.str();
}

std::string print_graph(const Graph& graph) {
  std::stringstream json_string;
  json_string << "{\n\t\"vertices\": [";
  const auto vertices = graph.get_vertices();
  const auto edges = graph.get_edges();

  auto it_vertices = vertices.begin(), prev_it_vertices = it_vertices;
  ++it_vertices;

  for (; it_vertices != vertices.end(); ++it_vertices, ++prev_it_vertices) {
    json_string << print_vertex(prev_it_vertices->second, graph) << ",";
  }
  json_string << print_vertex(prev_it_vertices->second, graph);

  json_string << "\n\t],\n\t\"edges\": [";

  auto it_edges = edges.begin(), prev_it_edges = it_edges;
  ++it_edges;

  for (; it_edges != edges.end(); ++it_edges, ++prev_it_edges) {
    json_string << print_edge(prev_it_edges->second, graph) << ",";
  }
  json_string << print_edge(prev_it_edges->second, graph) << "\n\t]\n}";

  return json_string.str();
}
}  // namespace json
}  // namespace printing
