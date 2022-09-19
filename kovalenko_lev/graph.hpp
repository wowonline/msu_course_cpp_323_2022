#pragma once

#include <set>
#include <vector>

#include "edge.hpp"
#include "vertex.hpp"

class Graph {
 public:
  void add_vertex() { vertices_.emplace_back(vertices_.size()); }

  void add_edge(Vertex::Id from, Vertex::Id to) {
    if (from < 0 || from >= vertices_.size() || to < 0 ||
        to >= vertices_.size() ||
        edges_.find(Edge(0, from, to)) != edges_.end()) {
      return;
    }
    edges_.emplace(edges_cnt_, from, to);
    edges_.emplace(edges_cnt_, to, from);
    ++edges_cnt_;
  }

 private:
  int edges_cnt_ = 0;

  std::set<Edge> edges_;
  std::vector<Vertex> vertices_;
};
