#pragma once

#include <queue>
#include <random>

#include "graph.hpp"

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

  Graph generate() const {
    auto graph = Graph();
    graph.add_vertex();

    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);

    return graph;
  }

 private:
  static constexpr float kGreenEdgeGenerationChance = 0.1f;
  static constexpr float kRedEdgeGenerationChance = 0.33f;
  static constexpr int kRedEdgeDepth = 2;
  static constexpr int kYellowEdgeDepth = 1;
  Params params_ = Params(0, 0);

  mutable std::mt19937 generator_{std::random_device()()};

  bool check_probability(float chance) const {
    std::bernoulli_distribution distribution(chance);
    return distribution(generator_);
  }

  void generate_grey_edges(Graph& graph) const {
    if (params_.depth() <= 1) {
      return;
    }
    std::queue<Graph::VertexId> queue;
    queue.push(0);
    while (!queue.empty()) {
      const auto vertex_id = queue.front();
      queue.pop();
      const auto depth = graph.depth_of(vertex_id);
      if (depth < params_.depth()) {
        const float success_chance =
            1.f - static_cast<float>(depth - 1) / (params_.depth() - 1);
        for (int i = 0; i < params_.new_vertices_count(); ++i) {
          if (check_probability(success_chance)) {
            const auto new_vertex_id = graph.add_vertex();
            graph.add_edge(vertex_id, new_vertex_id);
            queue.push(new_vertex_id);
          }
        }
      }
    }
  }

  void generate_green_edges(Graph& graph) const {
    for (const auto& vertex : graph.vertices()) {
      if (check_probability(kGreenEdgeGenerationChance)) {
        graph.add_edge(vertex.id(), vertex.id());
      }
    }
  }

  Graph::VertexId select_random_vertex(
      const std::vector<Graph::VertexId>& probable_vertices) const {
    std::uniform_int_distribution<> distribution(0,
                                                 probable_vertices.size() - 1);
    return probable_vertices.at(distribution(generator_));
  }

  void generate_yellow_edges(Graph& graph) const {
    for (const auto& vertex : graph.vertices()) {
      const auto vertex_depth = graph.depth_of(vertex.id());
      if (vertex_depth != 1 &&
          vertex_depth <= params_.depth() - kYellowEdgeDepth) {
        const float success_chance =
            static_cast<float>(vertex_depth - 1) / (params_.depth() - 2);
        if (check_probability(success_chance)) {
          add_yellow_edge(graph, vertex.id());
        }
      }
    }
  }

  void add_yellow_edge(Graph& graph, Graph::VertexId from_vertex_id) const {
    std::vector<Graph::VertexId> probable_vertices;
    for (const Graph::Vertex& vertex : graph.vertices()) {
      if (graph.depth_of(vertex.id()) ==
              graph.depth_of(from_vertex_id) + kYellowEdgeDepth &&
          !graph.is_connected(from_vertex_id, vertex.id())) {
        probable_vertices.push_back(vertex.id());
      }
    }
    if (probable_vertices.size() > 0) {
      graph.add_edge(from_vertex_id, select_random_vertex(probable_vertices));
    }
  }

  void generate_red_edges(Graph& graph) const {
    for (const auto& vertex : graph.vertices()) {
      const auto vertex_depth = graph.depth_of(vertex.id());
      if (vertex_depth <= params_.depth() - kRedEdgeDepth) {
        if (check_probability(kRedEdgeGenerationChance)) {
          add_red_edge(graph, vertex.id());
        }
      }
    }
  }

  void add_red_edge(Graph& graph, Graph::VertexId from_vertex_id) const {
    std::vector<Graph::VertexId> probable_vertices;
    for (const Graph::Vertex& vertex : graph.vertices()) {
      if (graph.depth_of(vertex.id()) ==
          graph.depth_of(from_vertex_id) + kRedEdgeDepth) {
        probable_vertices.push_back(vertex.id());
      }
    }
    if (probable_vertices.size() > 0) {
      graph.add_edge(from_vertex_id, select_random_vertex(probable_vertices));
    }
  }
};
