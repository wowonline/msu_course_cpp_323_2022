#pragma once

struct Vertex {
 public:
  using Id = int;
  explicit Vertex(Id id) : id_(id) {}
  Id id() const { return id_; }

  static Id next_id(Id current) { return current + 1; }

 private:
  Id id_ = 0;
};
