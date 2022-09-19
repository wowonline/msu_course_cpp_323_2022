#pragma once

#include <set>
#include <vector>

#include "vertex.hpp"

class Graph {
 public:
  using EdgeId = int;

  struct Edge {
   public:
    Edge(EdgeId id, Vertex::Id from, Vertex::Id to)
        : id_(id), from_(from), to_(to) {}
    EdgeId id() const { return id_; }
    Vertex::Id from() const { return from_; }
    Vertex::Id to() const { return to_; }

    bool operator==(const Edge& rhs) const {
      return (from_ == rhs.from_ && to_ == rhs.to_) ||
             (from_ == rhs.to_ && to_ == rhs.from_);
    }

    bool operator<(const Edge& rhs) const {
      return from() == rhs.from() ? to() < rhs.to() : from() < rhs.from();
    }

    bool operator>(const Edge& rhs) const { return rhs < *this; }

    bool operator>=(const Edge& rhs) const { return !operator<(rhs); }

    bool operator<=(const Edge& rhs) const { return !operator>(rhs); }

   private:
    EdgeId id_;
    Vertex::Id from_;
    Vertex::Id to_;
  };

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
  EdgeId edges_cnt_ = 0;

  std::set<Edge> edges_;
  std::vector<Vertex> vertices_;
};
