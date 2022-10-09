#pragma once

#include <utility>

#include "vertex.hpp"

struct Edge {
 public:
  using Id = int;

  Edge(Id id, Vertex::Id from_vertex_id, Vertex::Id to_vertex_id)
      : id_(id), from_vertex_id_(from_vertex_id), to_vertex_id_(to_vertex_id) {
    if (from_vertex_id > to_vertex_id) {
      std::swap(from_vertex_id, to_vertex_id);
    }
  }
  Id id() const { return id_; }
  Vertex::Id from_vertex_id() const { return from_vertex_id_; }
  Vertex::Id to_vertex_id() const { return to_vertex_id_; }

  bool operator==(const Edge& rhs) const {
    return from_vertex_id_ == rhs.from_vertex_id_ &&
           to_vertex_id_ == rhs.to_vertex_id_;
  }

  bool operator<(const Edge& rhs) const {
    return from_vertex_id() == rhs.from_vertex_id()
               ? to_vertex_id() < rhs.to_vertex_id()
               : from_vertex_id() < rhs.from_vertex_id();
  }

  bool operator>(const Edge& rhs) const { return rhs < *this; }

  bool operator>=(const Edge& rhs) const { return !operator<(rhs); }

  bool operator<=(const Edge& rhs) const { return !operator>(rhs); }

 private:
  Id id_;
  Vertex::Id from_vertex_id_;
  Vertex::Id to_vertex_id_;
};
