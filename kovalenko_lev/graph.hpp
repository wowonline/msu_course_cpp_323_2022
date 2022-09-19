#pragma once

#include <map>
#include <set>
#include <vector>

#include "edge.hpp"
#include "vertex.hpp"

class Graph {
 public:
  void add_vertex() {
    auto vertex_id = generate_vertex_id();
    vertices_.emplace(vertex_id, vertex_id);
  }

  void add_edge(Vertex::Id from, Vertex::Id to) {
    auto edge_id = generate_edge_id();
    edges_.emplace(std::piecewise_construct, std::make_tuple(edge_id),
                   std::make_tuple(edge_id, from, to));
    connetions_[from].insert(edge_id);
    connetions_[to].insert(edge_id);
  }

 private:
  Vertex::Id generate_vertex_id() const {
    Vertex::Id ans = next_vertex_id_;
    next_vertex_id_ = Vertex::next_id(next_vertex_id_);
    return ans;
  }

  Edge::Id generate_edge_id() const {
    Edge::Id ans = next_edge_id_;
    next_edge_id_ = Edge::next_id(next_edge_id_);
    return ans;
  }

  mutable Vertex::Id next_vertex_id_ = 0;
  mutable Edge::Id next_edge_id_ = 0;

  std::map<Vertex::Id, Vertex> vertices_;
  std::map<Edge::Id, Edge> edges_;
  std::map<Vertex::Id, std::set<Edge::Id>> connetions_;
};
