#include <unordered_map>
#include <unordered_set>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;

  struct Vertex {
   public:
    const VertexId id = 0;
    explicit Vertex(const VertexId& _id) : id(_id) {}
  };

  struct Edge {
   public:
    const VertexId from_vertex_id = 0;
    const VertexId to_vertex_id = 0;
    const EdgeId id = 0;

    Edge(const EdgeId& _id,
         const VertexId& _from_vertex_id,
         const VertexId& _to_vertex_id)
        : id(_id),
          from_vertex_id(_from_vertex_id),
          to_vertex_id(_to_vertex_id) {}
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
  EdgeId nextEdgeId_ = 0;
  VertexId nextVertexId_ = 0;
  EdgeId generateEdgeId() { return nextEdgeId_++; }
  VertexId generateVertexId() { return nextVertexId_++; }

  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<VertexId, std::unordered_set<EdgeId>> connectivityList_;
};