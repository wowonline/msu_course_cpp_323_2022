#pragma once

struct Vertex {
 public:
  using Id = int;
  explicit Vertex(Id id) : id_(id) {}
  Id id() const { return id_; }

 private:
  Id id_ = 0;
};
