#pragma once

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  void add_vertex() {
    VertexId new_vertex_id = get_new_vertex_id();
    vertices_.emplace_back(new_vertex_id);
    connections_[new_vertex_id] = {};
    vertex_depths_[new_vertex_id] = 1;
  }

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    assert(from_vertex_id < vertices_.size());
    assert(to_vertex_id < vertices_.size());
    // The first edge added to the vertex must connect it with the main
    // component. In other words, at any moment graph must be connected except
    // for isolated vertices.
    assert(from_vertex_id == 0 || to_vertex_id == 0 ||
           !(vertex_depths_[from_vertex_id] == 1 &&
             vertex_depths_[to_vertex_id] == 1));

    const auto edge_id = get_new_edge_id();
    edges_.emplace_back(Edge(edge_id, from_vertex_id, to_vertex_id));

    connections_[from_vertex_id].push_back(edge_id);
    if (to_vertex_id != from_vertex_id) {
      connections_[to_vertex_id].push_back(edge_id);
    }

    update_vertex_depth(from_vertex_id,
                        to_vertex_id);  // TODO run only in case of grey edge
  }

  const auto& vertices() const { return vertices_; }

  const auto& edges() const { return edges_; }

  const auto& connections() const { return connections_; }

  const auto& vertex_depths() const { return vertex_depths_; }

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

   private:
    VertexId id_ = 0;
  };

  struct Edge {
   public:
    explicit Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id) {}
    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; };
    VertexId to_vertex_id() const { return to_vertex_id_; };

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
  };

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::unordered_map<VertexId, std::vector<EdgeId>> connections_;
  std::unordered_map<VertexId, Depth> vertex_depths_;

  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;

  VertexId get_new_vertex_id() { return last_vertex_id_++; }
  EdgeId get_new_edge_id() { return last_edge_id_++; }

  void update_vertex_depth(VertexId from_vertex_id, VertexId to_vertex_id) {
    if (from_vertex_id == 0 || to_vertex_id == 0) {
      auto non_root_id = std::max(from_vertex_id, to_vertex_id);
      if (non_root_id != 0) {
        vertex_depths_[non_root_id] = 2;
      }
    } else if (vertex_depths_[from_vertex_id] == 1) {
      vertex_depths_[from_vertex_id] = vertex_depths_[to_vertex_id] + 1;
    } else if (vertex_depths_[to_vertex_id] == 1) {
      vertex_depths_[to_vertex_id] = vertex_depths_[from_vertex_id] + 1;
    }
  }
};
