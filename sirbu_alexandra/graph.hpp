#include <unordered_map>
#include <unordered_set>

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

  VertexId add_vertex() {
    const VertexId newVertexId = generateVertexId();
    vertices_.emplace(newVertexId, newVertexId);
    connectivityList_.emplace(newVertexId, std::unordered_set<EdgeId>());

    return newVertexId;
  }

  EdgeId add_edge(const VertexId& from_vertex_id,
                  const VertexId& to_vertex_id) {
    const EdgeId newEdgeId = generateEdgeId();

    edges_.emplace(newEdgeId, Edge(newEdgeId, from_vertex_id, to_vertex_id));
    connectivityList_.at(from_vertex_id).insert(newEdgeId);

    if (from_vertex_id != to_vertex_id) {
      connectivityList_.at(to_vertex_id).insert(newEdgeId);
    }

    return newEdgeId;
  }

 private:
  EdgeId next_edge_id = 0;
  VertexId next_vertex_id = 0;
  EdgeId generateEdgeId() { return next_edge_id++; }
  VertexId generateVertexId() { return next_vertex_id++; }

  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<VertexId, std::unordered_set<EdgeId>> connectivityList_;
};
