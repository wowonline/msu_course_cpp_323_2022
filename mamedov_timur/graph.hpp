#pragma once

#include <unordered_map>
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
    Edge(EdgeId id, VertexId first_vertex_id_, VertexId second_vertex_id_);

    EdgeId id() const;
    VertexId get_first_vertex_id() const;
    VertexId get_second_vertex_id() const;

   private:
    EdgeId id_ = 0;
    VertexId first_vertex_id_ = 0;
    VertexId second_vertex_id_ = 0;
  };

  void add_vertex();

  void add_edge(VertexId first_vertex_id_, VertexId second_vertex_id_);

  const std::unordered_map<EdgeId, Edge>& get_edges() const;

  const std::unordered_map<VertexId, Vertex>& get_vertices() const;

  const std::vector<EdgeId>& get_edges_of_vertex(VertexId vertex_id) const;

 private:
  VertexId get_new_vertex_id();

  EdgeId get_new_edge_id();

  bool has_vertex_id(VertexId vertex_id) const;

  std::unordered_map<VertexId, Vertex> vertices_ = {};
  std::unordered_map<EdgeId, Edge> edges_ = {};

  std::unordered_map<VertexId, std::vector<EdgeId>> connections_ = {};
  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;
};
