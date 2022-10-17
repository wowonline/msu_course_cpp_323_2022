#include <algorithm>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#pragma once

class Graph {
 public:
  using Depth = int;
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
    enum class Color { Grey, Green, Yellow, Red };

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
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
    Color color_ = Color::Grey;
  };

  bool is_connected(const VertexId&, const VertexId&) const;
  VertexId add_vertex();

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  Depth get_graph_depth() const { return max_depth_; }
  Depth get_vertex_depth(VertexId id) const {
    assert(has_vertex(id));
    return depth_of_vertices_.at(id);
  }

  const std::unordered_map<VertexId, Vertex>& vertices() const {
    return vertices_;
  }
  const std::vector<VertexId>& vertices_of_depth(const Depth& depth) const {
    return vertices_of_depth_[depth];
  }
  const std::unordered_map<EdgeId, Edge>& edges() const { return edges_; }
  const std::set<EdgeId>& connected_edge_ids(VertexId vertex_id) const {
    return connections_list_.at(vertex_id);
  }

  bool has_vertex(VertexId id) const {
    return vertices_.find(id) != vertices_.end();
  }
  bool has_edge(EdgeId id) const { return edges_.find(id) != edges_.end(); }

 private:
  EdgeId gen_new_edge_id() { return next_edge_id_++; }
  VertexId gen_new_vertex_id() { return next_vertex_id_++; }

  void set_vertex_depth(const VertexId& id, const Depth& depth);

  VertexId next_vertex_id_ = 0;
  EdgeId next_edge_id_ = 0;
  Depth max_depth_ = 0;
  std::unordered_map<VertexId, Depth> depth_of_vertices_;
  std::vector<std::vector<VertexId>> vertices_of_depth_;
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, std::set<EdgeId>> connections_list_;
};

class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth, int new_vertices_count)
        : depth_(depth), new_vertices_count_(new_vertices_count) {}

    Graph::Depth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(Params&& params) : params_(std::move(params)) {}

  Graph generate() const;

 private:
  void generate_grey_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;

  Params params_ = Params(0, 0);
};
