#pragma once

#include <cassert>
#include <random>
#include <unordered_map>
#include <vector>
#include <algorithm>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  struct Vertex {
   public:
    explicit Vertex(VertexId id);
    VertexId id() const;

   private:
    VertexId id_ = 0;
  };

  struct Edge {
   public:
    enum class Color { Grey, Green, Yellow, Red };
    Edge(EdgeId id,
         VertexId first_vertex_id,
         VertexId second_vertex_id,
         Edge::Color color);

    EdgeId id() const;
    VertexId get_first_vertex_id() const;
    VertexId get_second_vertex_id() const;
    Color color() const;

   private:
    EdgeId id_ = 0;
    VertexId first_vertex_id_ = 0;
    VertexId second_vertex_id_ = 0;
    Color color_ = Color::Grey;
  };

  VertexId add_vertex();

  EdgeId add_edge(VertexId first_vertex_id, VertexId second_vertex_id);

  const std::unordered_map<EdgeId, Edge>& get_edges() const;

  const std::unordered_map<VertexId, Vertex>& get_vertices() const;

  const std::vector<EdgeId>& get_edges_of_vertex(VertexId vertex_id) const;

  const std::unordered_map<Depth, std::vector<VertexId>>& get_vertices_depth()
      const;

 private:
  VertexId get_new_vertex_id();

  EdgeId get_new_edge_id();

  bool has_vertex_id(VertexId vertex_id) const;

  bool has_edge(VertexId first_vertex_id, VertexId second_vertex_id) const;

  std::unordered_map<VertexId, Vertex> vertices_ = {};
  std::unordered_map<EdgeId, Edge> edges_ = {};

  std::unordered_map<VertexId, std::vector<EdgeId>> connections_ = {};

  std::unordered_map<Depth, std::vector<VertexId>> vertices_depth_ = {};

  std::vector<int> vetices_dep_;
  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;
};

class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth, int new_vertices_count);

    Graph::Depth depth() const;
    int new_vertices_count() const;

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(Params&& params);

  void generate_grey_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;

  Graph generate() const;

 private:
  Params params_ = Params(0, 0);
};