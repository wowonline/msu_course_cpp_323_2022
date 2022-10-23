#pragma once

#include <map>
#include <unordered_map>
#include <vector>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

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
    Edge(EdgeId id,
         VertexId first_vertex_id,
         VertexId second_vertex_id,
         Color color)
        : id_(id),
          first_vertex_id_(first_vertex_id),
          second_vertex_id_(second_vertex_id),
          color_(color) {}
    EdgeId id() const { return id_; }
    VertexId get_first_vertex_id() const { return first_vertex_id_; }
    VertexId get_second_vertex_id() const { return second_vertex_id_; }
    Color color() const { return color_; }

   private:
    EdgeId id_ = 0;
    VertexId first_vertex_id_ = 0;
    VertexId second_vertex_id_ = 0;
    Color color_ = Color::Grey;
  };

  VertexId add_vertex();

  EdgeId add_edge(VertexId first_vertex_id, VertexId second_vertex_id);

  bool has_edge(VertexId first_vertex_id, VertexId second_vertex_id) const;

  Depth depth() const { return vertices_depth_.size(); }

  const std::unordered_map<EdgeId, Edge>& get_edges() const { return edges_; }

  const std::unordered_map<VertexId, Vertex>& get_vertices() const {
    return vertices_;
  }

  const std::vector<EdgeId>& get_connected_edge_ids(VertexId vertex_id) const {
    return connections_.at(vertex_id);
  }

  const std::map<Depth, std::vector<VertexId>>& get_vertices_depth() const {
    return vertices_depth_;
  };

  Depth get_vertex_depth(VertexId vertex_id) const;

 private:
  VertexId get_new_vertex_id() { return last_vertex_id_++; }

  EdgeId get_new_edge_id() { return last_edge_id_++; };

  bool has_vertex_id(VertexId vertex_id) const {
    return vertices_.find(vertex_id) != vertices_.end();
  }

  std::unordered_map<VertexId, Vertex> vertices_ = {};
  std::unordered_map<EdgeId, Edge> edges_ = {};

  std::unordered_map<VertexId, std::vector<EdgeId>> connections_ = {};

  std::map<Depth, std::vector<VertexId>> vertices_depth_ = {};

  std::vector<int> vetices_dep_;
  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;
};

class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth, int new_vertices_count)
        : depth_(depth), new_vertices_count_(new_vertices_count){};

    Graph::Depth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(Params&& params) : params_(std::move(params)){};

  void generate_grey_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;

  Graph generate() const;

 private:
  Params params_ = Params(0, 0);
};