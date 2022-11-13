#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Graph {
public:
  using VertexId = int;
  using EdgeId = int;

  struct Vertex {
  public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

  private:
    VertexId id_ = 0;
  };

  struct Edge {
  public:
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
        : id_(id), from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id) {}
    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }

  private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
  };

  const std::vector<Edge> &get_edges() const { return edges; }

  const std::vector<Vertex> &get_vertices() const { return vertices; }

  void add_vertex() { vertices.push_back(Vertex(vertices.size())); }

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    assert(count_vertices_ids_in_graph({from_vertex_id, to_vertex_id}) >= 2);
    edges.push_back(Edge(edges.size(), from_vertex_id, to_vertex_id));
  }

private:
  int count_vertices_ids_in_graph(const std::vector<VertexId> &v_ids) {
    int cnt = 0;
    for (auto &v_id : v_ids) {
      for (auto &u : vertices) {
        if (u.id() == v_id)
          cnt++;
      }
    }
    return cnt;
  }

  std::vector<Vertex> vertices;
  std::vector<Edge> edges;
};

namespace printing {
namespace json {

std::string print_graph(const Graph &graph);
std::string print_vertex(const Graph::Vertex &vertex, const Graph &graph);
std::string print_edge(const Graph::Edge &edge);

std::string print_vertex(const Graph::Vertex &vertex, const Graph &graph) {
  std::string res = "{\"id\":";
  res += std::to_string(vertex.id()) + ",";

  std::vector<Graph::EdgeId> edge_ids;
  for (const auto &edge : graph.get_edges()) {
    if (edge.to_vertex_id() == vertex.id() ||
        edge.from_vertex_id() == vertex.id()) {
      edge_ids.push_back(edge.id());
    }
  }

  res += "\"edge_ids\":[";
  for (const auto &id : edge_ids) {
    res += std::to_string(id) + ",";
  }
  if (edge_ids.size())
    res.pop_back();
  res += "]}";
  return res;
}

std::string print_edge(const Graph::Edge &edge) {
  return "{\"id\":" + std::to_string(edge.id()) + ",\"vertex_ids\":" + "[" +
         std::to_string(edge.from_vertex_id()) + "," +
         std::to_string(edge.to_vertex_id()) + "]}";
}

std::string print_graph(const Graph &graph) {
  std::string res = "{\"vertices\":[";
  std::vector<std::string> vertex_strings;
  for (const auto &vertex : graph.get_vertices()) {
    vertex_strings.push_back(print_vertex(vertex, graph));
  }
  for (auto &s : vertex_strings) {
    res += s + ",";
  }
  if (vertex_strings.size())
    res.pop_back();
  res += "],\"edges\":[";

  std::vector<std::string> edge_strings;
  for (const auto &edge : graph.get_edges()) {
    edge_strings.push_back(print_edge(edge));
  }
  for (auto &s : edge_strings) {
    res += s + ",";
  }
  if (edge_strings.size())
    res.pop_back();
  res += "]}";
  return res;
}
} // namespace json
} // namespace printing

int main() {
  auto graph = Graph();

  constexpr int kVerticesCount = 14;

  for (int i = 0; i < kVerticesCount; i++) {
    graph.add_vertex();
  }

  graph.add_edge(0, 1);
  graph.add_edge(0, 2);
  graph.add_edge(0, 3);
  graph.add_edge(1, 4);
  graph.add_edge(1, 5);
  graph.add_edge(1, 6);
  graph.add_edge(2, 7);
  graph.add_edge(2, 8);
  graph.add_edge(3, 9);
  graph.add_edge(4, 10);
  graph.add_edge(5, 10);
  graph.add_edge(6, 10);
  graph.add_edge(7, 11);
  graph.add_edge(8, 11);
  graph.add_edge(9, 12);
  graph.add_edge(10, 13);
  graph.add_edge(11, 13);
  graph.add_edge(12, 13);

  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;

  std::ofstream file("graph.json");
  file << graph_json;
  return 0;
}