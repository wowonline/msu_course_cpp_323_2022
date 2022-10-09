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
    assert(from_vertex_id >= 0);
    assert(from_vertex_id < next_vertex_id_);
    assert(to_vertex_id >= 0);
    assert(to_vertex_id < next_vertex_id_);
    assert(!has_edge(from_vertex_id, to_vertex_id));

    const auto edge_id = generate_edge_id();
    edges_.insert(
        std::make_pair(edge_id, Edge(edge_id, from_vertex_id, to_vertex_id)));
    connetions_[from_vertex_id].insert(edge_id);
    connetions_[to_vertex_id].insert(edge_id);
  }

  bool has_edge(Vertex::Id from_vertex_id, Vertex::Id to_vertex_id) {
    const auto to_vertices = get_connected_vertices(from_vertex_id);
    return to_vertices.find(to_vertex_id) != to_vertices.end();
  }

  std::unordered_set<Vertex::Id> get_connected_vertices(
      Vertex::Id from_vertex_id) {
    std::unordered_set<Vertex::Id> result;
    if (connetions_.find(from_vertex_id) == connetions_.end()) {
      return result;
    }
    for (const auto& edge_id : connetions_[from_vertex_id]) {
      const Edge& edge = edges_.at(edge_id);
      if (edge.from_vertex_id() == from_vertex_id) {
        result.insert(edge.to_vertex_id());
      } else {
        result.insert(edge.from_vertex_id());
      }
    }
    return result;
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
