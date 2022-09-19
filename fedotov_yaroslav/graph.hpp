#pragma once
#include <vector>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;

  struct Vertex {
   public:
    explicit Vertex(VertexId id);
    VertexId id() const;

   private:
    VertexId id_ = 0;
  };
  struct Edge {
   public:
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id);
    EdgeId id() const;
    VertexId from_vertex_id() const;
    VertexId to_vertex_id() const;

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
  };

  void add_vertex();
  void add_edge(VertexId first_vertex_id, VertexId second_vertex_id);
  const std::vector<Vertex>& vertices() const;
  const std::vector<Edge>& edges() const;

 private:
  bool has_vertex(VertexId id) const;
  VertexId get_new_vertex_id();
  EdgeId get_new_edge_id();

  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;
};