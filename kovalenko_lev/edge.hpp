#pragma once

#include <utility>

#include "vertex.hpp"

struct Edge {
 public:
  using Id = int;

  Edge(Id id, Vertex::Id from_vertex_id, Vertex::Id to_vertex_id)
      : id_(id), from_vertex_id_(from_vertex_id), to_vertex_id_(to_vertex_id) {}
  Edge(const Edge& other) = default;
  Id id() const { return id_; }
  Vertex::Id from_vertex_id() const { return from_vertex_id_; }
  Vertex::Id to_vertex_id() const { return to_vertex_id_; }

 private:
  Id id_;
  Vertex::Id from_vertex_id_;
  Vertex::Id to_vertex_id_;
};
