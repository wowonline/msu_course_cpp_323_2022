#include "graph_generator.hpp"
#include <random>
#include <unordered_map>
#include <vector>
#include "graph.hpp"

namespace {
static constexpr float kGreenEdgesProbability = 0.1;
static constexpr float kRedEdgesProbability = 0.33;
bool check_probability(double probability) {
  std::random_device device;
  std::mt19937 generator(device());
  std::bernoulli_distribution distribution(probability);
  return distribution(generator);
}

uni_course_cpp::Graph::VertexId get_random_vertex_id(
    const std::vector<uni_course_cpp::Graph::VertexId>& list) {
  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<> distribution(0, list.size() - 1);
  return list[distribution(generator)];
}

std::vector<uni_course_cpp::Graph::VertexId> get_unconnected_vertices_ids(
    const uni_course_cpp::Graph& graph,
    uni_course_cpp::Graph::VertexId vertex_id,
    const std::vector<uni_course_cpp::Graph::VertexId>& vertices_ids) {
  std::vector<uni_course_cpp::Graph::VertexId> result;
  for (const auto vertex_id_depth_greater : vertices_ids) {
    if (!graph.has_edge(vertex_id, vertex_id_depth_greater)) {
      result.push_back(vertex_id_depth_greater);
    }
  }
  return result;
}
}  // namespace

namespace uni_course_cpp {
Graph GraphGenerator::generate() const {
  auto graph = Graph();
  if (params_.get_depth() != 0) {
    graph.add_vertex();
  }
  generate_grey_edges(graph);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);
  return graph;
}

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  for (auto depth = kDefaultDepth; depth < params_.get_depth(); ++depth) {
    if (depth > graph.depth())
      break;
    const float probability =
        (params_.get_depth() - depth) / (params_.get_depth() - 1.f);
    for (const auto vertex_id : graph.vertices_at_depth(depth)) {
      for (int i = 0; i < params_.new_vertices_count(); ++i) {
        if (check_probability(probability)) {
          graph.add_edge(vertex_id, graph.add_vertex());
        }
      }
    }
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (const auto& [vertex_id, vertex] : graph.vertices()) {
    if (check_probability(kGreenEdgesProbability)) {
      graph.add_edge(vertex_id, vertex_id);
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  for (auto depth = kDefaultDepth; depth < graph.depth(); ++depth) {
    const float probability = (depth - 1) / (params_.get_depth() - 2.f);
    for (const auto vertex_id : graph.vertices_at_depth(depth)) {
      if (check_probability(probability)) {
        const auto unconnected_vertices_ids = get_unconnected_vertices_ids(
            graph, vertex_id,
            graph.vertices_at_depth(depth + kYellowEdgeDepth));
        if (!unconnected_vertices_ids.empty()) {
          graph.add_edge(vertex_id,
                         get_random_vertex_id(unconnected_vertices_ids));
        }
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (auto depth = kDefaultDepth; depth < graph.depth() - 1; ++depth) {
    for (const auto vertex_id : graph.vertices_at_depth(depth)) {
      if (check_probability(kRedEdgesProbability) &&
          !graph.vertices_at_depth(depth + 2).empty()) {
        graph.add_edge(vertex_id, get_random_vertex_id(graph.vertices_at_depth(
                                      depth + kRedEdgeDepth)));
      }
    }
  }
}
}  // namespace uni_course_cpp
