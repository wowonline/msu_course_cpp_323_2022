#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  void add_vertex() { vertices_.emplace_back(Vertex(gen_new_vertex_id())); }

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    edges_.emplace_back(
        (Edge(gen_new_edge_id(), from_vertex_id, to_vertex_id)));
  }

  void show_graph() const {
    for (const auto& edge : edges_) {
      std::cout << edge.id() << " : " << edge.from_vertex_id() << " --> "
                << edge.to_vertex_id() << std::endl;
    }
  }

 private:
  bool has_vertex(VertexId id) const {
    return std::any_of(vertices_.begin(), vertices_.end(),
                       [id](const Vertex& v) { return v.id() == id; });
  }

  EdgeId gen_new_edge_id() { return next_edge_id_++; }

  VertexId gen_new_vertex_id() { return next_vertex_id_++; }

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

  VertexId next_vertex_id_ = 0;
  EdgeId next_edge_id_ = 0;
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
};

static constexpr int kVerticesCount = 14;

int main() {
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

  graph.show_graph();

  return 0;
}
