#include "graph_generator.hpp"
#include <iostream>
#include <random>

namespace {
static constexpr double kGreenEdgesProbability = 0.1;
static constexpr double kRedEdgesProbability = 0.33;
static constexpr uni_course_cpp::Graph::Depth kGraphBaseDepth = 1;
static constexpr uni_course_cpp::Graph::Depth kYellowEdgeDepth = 1;
static constexpr uni_course_cpp::Graph::Depth kRedEdgeDepth = 2;
static constexpr uni_course_cpp::Graph::Depth kYellowMaxEdgeDepth = 2;

bool check_probability(const double probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  return d(gen);
}

};  // namespace

namespace uni_course_cpp {

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  if (!params_.depth()) {
    return graph;
  }
  graph.add_vertex();
  generate_grey_edges(graph);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);
  return graph;
}

Graph::VertexId get_random_vertex_id(int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, size);
  return distrib(gen);
}

std::vector<Graph::VertexId> get_unconnected_vertex_ids(
    const std::vector<Graph::VertexId>& vertices,
    const Graph& graph,
    const Graph::VertexId vertex) {
  std::vector<Graph::VertexId> result;
  for (const auto cur_vertex : vertices) {
    if (!graph.has_edge(vertex, cur_vertex)) {
      result.emplace_back(cur_vertex);
    }
  }
  return result;
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  const auto& vertices = graph.get_vertices();
  for (auto vertex : vertices) {
    if (check_probability(kGreenEdgesProbability)) {
      graph.add_edge(vertex.first, vertex.first);
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  const auto max_depth = graph.depth() - kYellowMaxEdgeDepth;

  for (Graph::Depth current_depth = kGraphBaseDepth; current_depth <= max_depth;
       ++current_depth) {
    for (const auto vertex : graph.get_vertex_ids_at_depth(current_depth)) {
      if (check_probability((1.0 / max_depth) * current_depth)) {
        const auto next_level_vertices = get_unconnected_vertex_ids(
            graph.get_vertex_ids_at_depth(current_depth + kYellowEdgeDepth),
            graph, vertex);
        if (!next_level_vertices.empty()) {
          graph.add_edge(vertex,
                         next_level_vertices[get_random_vertex_id(
                             next_level_vertices.size() - kYellowEdgeDepth)]);
        }
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  const auto max_depth = graph.depth() - kRedEdgeDepth;

  for (Graph::Depth current_depth = 0; current_depth < max_depth;
       ++current_depth) {
    for (const auto vertex : graph.get_vertex_ids_at_depth(current_depth)) {
      if (check_probability(kRedEdgesProbability)) {
        const auto next_level_vertices =
            graph.get_vertex_ids_at_depth(current_depth + kRedEdgeDepth);
        graph.add_edge(vertex,
                       next_level_vertices[get_random_vertex_id(
                           next_level_vertices.size() - kGraphBaseDepth)]);
      }
    }
  }
}

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  const auto max_depth = params_.depth() - kGraphBaseDepth;
  const auto new_vertices_count = params_.new_vertices_count();

  for (Graph::Depth current_depth = 0;
       current_depth < max_depth && current_depth < graph.depth();
       ++current_depth) {
    for (const auto vertex : graph.get_vertex_ids_at_depth(current_depth)) {
      for (int i = 0; i < new_vertices_count; ++i) {
        if (check_probability(1.0 - (1.0 / max_depth) * current_depth)) {
          graph.add_edge(vertex, graph.add_vertex());
        }
      }
    }
  }
}

}  // namespace uni_course_cpp
