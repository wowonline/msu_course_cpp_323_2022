#include <unordered_map>
#include <vector>

#define kVerticesCount 13
class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;

  struct Edge {
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id) {}

    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }

   private:
    const EdgeId id_ = 0;
    const EdgeId from_vertex_id_ = 0;
    const EdgeId to_vertex_id_ = 0;
  };

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

   private:
    const VertexId id_ = 0;
  };

  void add_vertex() { vertices_.emplace_back(get_new_vertex_id()); }

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    connections_[from_vertex_id].push_back(get_new_edge_id());
    connections_[to_vertex_id].push_back(get_new_edge_id());
  }

  bool has_vertex(VertexId id) const { return id < vertices_.size(); }

  VertexId get_new_vertex_id() { return new_vertex_id_++; }
  VertexId get_new_edge_id() { return new_edge_id_++; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  VertexId new_vertex_id_ = 0;
  EdgeId new_edge_id_ = 0;
  std::unordered_map<VertexId, std::vector<EdgeId>> connections_;
};

int main() {
  auto graph = Graph();

  for (int i = 0; i <= kVerticesCount; i++) {
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

  return 0;
}
