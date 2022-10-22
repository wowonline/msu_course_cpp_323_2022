#pragma once

#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "edge.hpp"
#include "vertex.hpp"

class Graph {
 public:
  void add_vertex() {
    const auto vertex_id = generate_vertex_id();
    vertices_.emplace(vertex_id, vertex_id);
  }

  void add_edge(Vertex::Id from_vertex_id, Vertex::Id to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    assert(!has_edge(from_vertex_id, to_vertex_id));

    const auto edge_id = generate_edge_id();
    edges_.emplace(edge_id, Edge(edge_id, from_vertex_id, to_vertex_id));
    connetions_[from_vertex_id].insert(edge_id);
    connetions_[to_vertex_id].insert(edge_id);
  }

  bool has_vertex(Vertex::Id vertex_id) {
    return vertices_.find(vertex_id) != vertices_.end();
  }

  bool has_edge(Vertex::Id from_vertex_id, Vertex::Id to_vertex_id) {
    for (const auto& edge_id_1 : connetions_[from_vertex_id]) {
      for (const auto& edge_id_2 : connetions_[to_vertex_id]) {
        if (edge_id_1 == edge_id_2) {
          return true;
        }
      }
    }
    return false;
  }

 private:
  Vertex::Id generate_vertex_id() { return next_vertex_id_++; }
  Edge::Id generate_edge_id() { return next_edge_id_++; }

  Vertex::Id next_vertex_id_ = 0;
  Edge::Id next_edge_id_ = 0;

  std::unordered_map<Vertex::Id, Vertex> vertices_;
  std::unordered_map<Edge::Id, Edge> edges_;
  std::unordered_map<Vertex::Id, std::unordered_set<Edge::Id>> connetions_;
};
