#pragma once

// #include <queue>

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

    constexpr int kVerticesCount = 14;

    for (int i = 0; i < kVerticesCount; i++) {
      graph.add_vertex();
    }

    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(0, 3);
    graph.add_edge(1, 4);
    graph.add_edge(1, 5);
    graph.add_edge(1, 6);
    graph.add_edge(2, 7);
    graph.add_edge(2, 8);
    graph.add_edge(3, 9);
    graph.add_edge(4, 10);
    // graph.add_edge(5, 10);
    // graph.add_edge(6, 10);
    graph.add_edge(7, 11);
    // graph.add_edge(8, 11);
    graph.add_edge(9, 12);
    graph.add_edge(10, 13);
    // graph.add_edge(11, 13);
    // graph.add_edge(12, 13);

    return graph;

    /*
    graph.add_vertex();

    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
    return graph;
    */
  }

 private:
  Params params_ = Params(0, 0);

  void generate_grey_edges(Graph& graph) const { /* TODO */
    /*
      if (params_.depth() == 0) {
       return;
     }
     std::queue<std::reference_wrapper<const Graph::Vertex>> queue;
     queue.push(graph.vertices().at(0));
    */
  }
  void generate_green_edges(Graph& graph) const { /* TODO */
  }
  void generate_yellow_edges(Graph& graph) const { /* TODO */
  }
  void generate_red_edges(Graph& graph) const { /* TODO */
  }
};
