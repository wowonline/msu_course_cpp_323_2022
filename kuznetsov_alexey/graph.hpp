#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

namespace uni_course_cpp {

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }
    Depth depth() const { return depth_; }
    void set_depth(Depth depth) { depth_ = depth; }
    inline bool operator==(const Vertex& vertex) {
      return this->id_ == vertex.id_;
    }

   private:
    Depth depth_ = 1;
    VertexId id_ = 0;
  };

  struct Edge {
    enum class Color { Grey, Green, Yellow, Red };

   public:
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id, Color color)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id),
          color_(color) {}
    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }
    Color color() const { return color_; }

   private:
    Color color_ = Color::Grey;
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
  };

  const std::vector<Edge>& get_edges() const { return edges_; }

  const std::vector<Vertex>& get_vertices() const { return vertices_; }

  void add_vertex();

  void set_vertex_depth(VertexId vertex_id, Depth depth);

  bool has_vertex(VertexId vertex_id) const {
    return adjacency_list_.count(vertex_id);
  }

  bool is_connected(VertexId from_vertex_id, VertexId to_vertex_id) const;

  const std::vector<EdgeId>& connected_edges_ids(VertexId vertex_id) const;

  std::vector<Edge> get_edges_with_color(Edge::Color color) const;

  Edge::Color get_edge_color(VertexId from_vertex_id,
                             VertexId to_vertex_id) const;

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id);

  std::vector<Vertex> get_vertices_with_depth(Depth depth) const;

  Depth depth() const { return graph_depth_; }

 private:
  VertexId get_new_vertex_id_() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id_() { return edge_id_counter_++; }

  Depth graph_depth_ = 0;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;
  std::unordered_map<Depth, std::vector<Vertex> > depth_to_vertices_;
  std::unordered_map<Edge::Color, std::vector<Edge> > color_to_edges_;
  std::unordered_map<VertexId, std::vector<EdgeId> > adjacency_list_of_edges_;
  std::unordered_map<VertexId, std::vector<VertexId> > adjacency_list_;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};

}  // namespace uni_course_cpp