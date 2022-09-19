#pragma once

#include <set>
#include <vector>

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
    Edge(EdgeId id, VertexId from, VertexId to)
        : id_(id), from_(from), to_(to) {}
    EdgeId id() const { return id_; }
    VertexId from() const { return from_; }
    VertexId to() const { return to_; }

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
    VertexId from_;
    VertexId to_;
  };

  void add_vertex() { vertices_.emplace_back(vertices_.size()); }

  void add_edge(VertexId from, VertexId to) {
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
