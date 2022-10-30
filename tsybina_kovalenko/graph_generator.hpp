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

  explicit GraphGenerator(Params&& params)
      : params_(std::move(params)), generator_(std::random_device()()) {}

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
  Params params_ = Params(0, 0);

  mutable std::mt19937 generator_;

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
        std::bernoulli_distribution distribution(success_chance);
        for (int i = 0; i < params_.new_vertices_count(); ++i) {
          if (distribution(generator_)) {
            auto new_vertex_id = graph.add_vertex();
            graph.add_edge(vertex_id, new_vertex_id);
            queue.push(new_vertex_id);
          }
        }
      }
    }
  }
  void generate_green_edges(Graph& graph) const { /* TODO */
  }
  void generate_yellow_edges(Graph& graph) const { /* TODO */
  }
  void generate_red_edges(Graph& graph) const { /* TODO */
  }
};
