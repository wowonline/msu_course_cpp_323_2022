#include "graph_generator.hpp"
#include <algorithm>
#include <iostream>
#include <random>

bool check_probabilty(float probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution p(probability);

  return p(gen);
}

int get_random_number_between(int start, int end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(start, end);
  return distrib(gen);
}

void generate_new_vertices(Graph& graph,
                           Graph::VertexId root_id,
                           Graph::Depth depth,
                           int new_vertices_count) {
  if (depth <= 1) {
    return;
  }
  const float probabilty_of_grey_edge =
      1.0 - graph.get_vertex_depth(root_id) / (depth - 1.0);
  for (int i = 0; i < new_vertices_count; ++i) {
    if (check_probabilty(probabilty_of_grey_edge)) {
      const Graph::VertexId child_id = graph.add_vertex();
      graph.add_edge(root_id, child_id);
      generate_new_vertices(graph, child_id, depth, new_vertices_count);
    }
  }
}

void generate_green_edges(Graph& graph) {
  constexpr float probabilty_of_green_edge = 0.1;
  for (const auto& [vertex_id, _] : graph.vertices()) {
    if (check_probabilty(probabilty_of_green_edge)) {
      graph.add_edge(vertex_id, vertex_id);
    }
  }
}

void generate_red_edges(Graph& graph) {
  constexpr float probabilty_of_red_edge = 0.33;
  for (const auto& [vertex_id, _] : graph.vertices()) {
    if (check_probabilty(probabilty_of_red_edge)) {
      const Graph::Depth current_depth = graph.get_vertex_depth(vertex_id);
      if (current_depth + 2 <= graph.depth()) {
        const std::set<Graph::VertexId>& deeper_vertices =
            graph.vertices_with_depth(current_depth + 2);
        if (deeper_vertices.size() > 0) {
          const Graph::VertexId random_vertex_pos =
              get_random_number_between(0, deeper_vertices.size() - 1);
          graph.add_edge(vertex_id, *std::next(deeper_vertices.begin(),
                                               random_vertex_pos));
        }
      }
    }
  }
}

void generate_yellow_edges(Graph& graph) {
  for (const auto& [vertex_id, _] : graph.vertices()) {
    Graph::Depth current_depth = graph.get_vertex_depth(vertex_id);
    if (current_depth + 1 <= graph.depth()) {
      const float probabilty_of_yellow_edge =
          static_cast<float>(current_depth) / graph.depth();
      if (check_probabilty(probabilty_of_yellow_edge)) {
        const std::set<Graph::VertexId>& deeper_vertices =
            graph.vertices_with_depth(current_depth + 1);
        const std::set<Graph::VertexId>& children_vertices =
            graph.children_of_vertex(vertex_id);
        std::vector<Graph::VertexId> not_connected_vertices;
        std::set_difference(deeper_vertices.begin(), deeper_vertices.end(),
                            children_vertices.begin(), children_vertices.end(),
                            std::inserter(not_connected_vertices,
                                          not_connected_vertices.begin()));
        if (not_connected_vertices.size() > 0) {
          Graph::VertexId random_vertex_id =
              get_random_number_between(0, not_connected_vertices.size() - 1);
          graph.add_edge(vertex_id, not_connected_vertices[random_vertex_id]);
        }
      }
    }
  }
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  const Graph::VertexId root_id = graph.add_vertex();
  generate_new_vertices(graph, root_id, params_.depth(),
                        params_.new_vertices_count());
  generate_green_edges(graph);
  generate_red_edges(graph);
  generate_yellow_edges(graph);
  return graph;
}
