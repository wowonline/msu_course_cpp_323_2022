#include <algorithm>
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
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id) {}

    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
  };

  void add_vertex() {
    vertices_.emplace_back(Vertex(gen_new_vertex_id()));
    std::vector<EdgeId> EdgesInPull = {};
    VertexPullEdges_.emplace_back(EdgesInPull);
  }

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    auto edge_id = gen_new_edge_id();
    edges_.emplace_back(Edge(edge_id, from_vertex_id, to_vertex_id));
    VertexPullEdges_[from_vertex_id].emplace_back(edge_id);
    VertexPullEdges_[to_vertex_id].emplace_back(edge_id);
  }

  std::string vertex_with_edges_json(const Vertex& vertex) const {
    int id = vertex.id();
    assert(has_vertex(id));
    std::string vertex_json =
        "{\"id\":" + std::to_string(id) + ",\"edge_ids\":[";
    bool is_have = false;

    for (const auto& edge_id : VertexPullEdges_[id]) {
      is_have = true;
      vertex_json += std::to_string(edge_id) + ",";
    }

    if (is_have) {
      vertex_json.pop_back();
    }

    vertex_json += "]}";

    return vertex_json;
  }

  std::string edge_with_vertices_json(const Edge& edge) const {
    assert(has_edge(edge.id()));
    std::string edge_json = "{\"id\":" + std::to_string(edge.id()) +
                            ",\"vertex_ids\":[" +
                            std::to_string(edge.from_vertex_id()) + "," +
                            std::to_string(edge.to_vertex_id()) + "]}";

    return edge_json;
  }

  std::vector<Vertex> set_of_vertices() const { return vertices_; }
  std::vector<Edge> set_of_edges() const { return edges_; }

 private:
  bool has_vertex(VertexId id) const {
    return std::any_of(
        vertices_.begin(), vertices_.end(),
        [id](const Vertex& vertex) { return vertex.id() == id; });
  }

  bool has_edge(EdgeId id) const {
    return std::any_of(edges_.begin(), edges_.end(),
                       [id](const Edge& edge) { return edge.id() == id; });
  }

  EdgeId gen_new_edge_id() { return next_edge_id_++; }

  VertexId gen_new_vertex_id() { return next_vertex_id_++; }

  VertexId next_vertex_id_ = 0;
  EdgeId next_edge_id_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<EdgeId>> VertexPullEdges_;
};

namespace printing {
namespace json {

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  return graph.vertex_with_edges_json(vertex);
}

std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  return graph.edge_with_vertices_json(edge);
}

std::string print_graph(const Graph& graph) {
  std::string graph_json = "{\"vertices\":[";
  auto vertices = graph.set_of_vertices();

  if (!vertices.empty()) {
    std::for_each(vertices.begin(), vertices.end(),
                  [&](const Graph::Vertex& v) {
                    graph_json += print_vertex(v, graph) + ",";
                  });

    graph_json.pop_back();
  }

  graph_json += "],\"edges\":[";

  std::vector<Graph::Edge> edges = graph.set_of_edges();

  if (!edges.empty()) {
    std::for_each(edges.begin(), edges.end(), [&](const Graph::Edge& e) {
      graph_json += print_edge(e, graph) + ",";
    });
    graph_json.pop_back();
  }

  graph_json += "]}";

  return graph_json;
}
}  // namespace json
}  // namespace printing

Graph generate_graph(const int& kVerticesCount = 14) {
  auto graph = Graph();

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

  return graph;
}

void write_to_file(const std::string& str, const std::string& filename) {
  std::ofstream out(filename, std::ios::trunc);
  out << str << std::endl;
  out.close();
}

int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
