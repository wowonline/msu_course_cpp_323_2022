#pragma once

#include <utility>

#include "vertex.hpp"

struct Edge {
 public:
  using Id = int;

  Edge(Id id, Vertex::Id from, Vertex::Id to) : id_(id), from_(from), to_(to) {
    if (from > to) {
      std::swap(from, to);
    }
  }
  Id id() const { return id_; }
  Vertex::Id from() const { return from_; }
  Vertex::Id to() const { return to_; }

  bool operator==(const Edge& rhs) const {
    return from_ == rhs.from_ && to_ == rhs.to_;
  }

  bool operator<(const Edge& rhs) const {
    return from() == rhs.from() ? to() < rhs.to() : from() < rhs.from();
  }

  bool operator>(const Edge& rhs) const { return rhs < *this; }

  bool operator>=(const Edge& rhs) const { return !operator<(rhs); }

  bool operator<=(const Edge& rhs) const { return !operator>(rhs); }

 private:
  Id id_;
  Vertex::Id from_;
  Vertex::Id to_;
};
