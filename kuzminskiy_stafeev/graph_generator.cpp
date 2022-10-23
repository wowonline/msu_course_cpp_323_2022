#include "graph_generator.hpp"
#include <random>

int random(int a, int b) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(a, b);
  return dis(gen);
}

bool check_probability(float prob) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(prob);
  return d(gen);
}

std::vector<Graph::VertexId> unconnected_vertices_ids_on_depth(
    const Graph& graph,
    const Graph::VertexId vertex_id,
    const Graph::Depth depth) {
  std::vector<Graph::VertexId> unconnected_vertices = {};
  for (const auto cur_vertex_id : graph.vertices_of_depth(depth)) {
    if (!graph.is_connected(vertex_id, cur_vertex_id)) {
      unconnected_vertices.emplace_back(cur_vertex_id);
    }
  }
  return unconnected_vertices;
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.add_vertex();
  generate_grey_edges(graph);
  generate_green_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);

  return graph;
}

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  const auto depth = params_.depth();
  const auto& new_vertices_count = params_.new_vertices_count();
  float step = 1.0 / (depth - 1);

  for (Graph::Depth cur_depth = 1; cur_depth <= depth; cur_depth++) {
    float prob = (float)(1 - step * (cur_depth - 1));
    for (const auto from_vertex_id : graph.vertices_of_depth(cur_depth)) {
      for (Graph::VertexId cur_vertex_count = 0;
           cur_vertex_count < new_vertices_count; cur_vertex_count++) {
        if (check_probability(prob)) {
          auto vertex_id = graph.add_vertex();
          graph.add_edge(from_vertex_id, vertex_id);
        }
      }
    }
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  static float constexpr prob = 0.1;
  const auto& vertices = graph.vertices();
  for (const auto& [vertex_id, vertex] : vertices) {
    if (check_probability(prob)) {
      graph.add_edge(vertex.id(), vertex.id());
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  const auto depth = params_.depth();
  float step = 1.0 / (depth - 2);
  for (Graph::Depth cur_depth = 2; cur_depth <= depth - 1; cur_depth++) {
    float prob = (float)(step * (cur_depth - 1));

    for (const auto from_vertex_id : graph.vertices_of_depth(cur_depth)) {
      if (check_probability(prob)) {
        const auto& unconnected_vertices_ids =
            unconnected_vertices_ids_on_depth(graph, from_vertex_id,
                                              cur_depth + 1);
        if (!unconnected_vertices_ids.empty()) {
          const auto rand_pos = random(0, unconnected_vertices_ids.size() - 1);
          const auto to_vertex_id = unconnected_vertices_ids[rand_pos];
          graph.add_edge(from_vertex_id, to_vertex_id);
        }
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  const auto depth = params_.depth();
  static float constexpr prob = (float)1 / 3;
  for (Graph::Depth cur_depth = 1; cur_depth <= depth - 2; cur_depth++) {
    const auto& next_vertices_depth = graph.vertices_of_depth(cur_depth + 2);

    for (const auto& from_vertex_id : graph.vertices_of_depth(cur_depth)) {
      const auto rand_pos = random(0, next_vertices_depth.size() - 1);
      const auto to_vertex_id = next_vertices_depth[rand_pos];
      if (check_probability(prob)) {
        graph.add_edge(from_vertex_id, to_vertex_id);
      }
    }
  }
}
