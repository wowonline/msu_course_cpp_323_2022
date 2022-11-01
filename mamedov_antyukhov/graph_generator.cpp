#include "graph_generator.hpp"
#include <random>

bool check_probability(double probability){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  return d(gen);
}

Graph::VertexId get_random_vertex_id(std::size_t size){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, size);
  return distrib(gen);
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
  const auto max_depth = params_.depth() - 1;
  const auto new_vertices_count = params_.new_vertices_count();

  double probability = 1.0;
  const double step = 1.0 / max_depth;
  std::vector<Graph::VertexId> vertices = {graph.get_vertices().begin()->first};
  std::vector<Graph::VertexId> vertices_next;

  for (Graph::Depth current_depth = 0; current_depth < max_depth;
       ++current_depth) {
    for (auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex) {
      for (int i = 0; i < new_vertices_count; ++i) {
        if (check_probability(probability)) {
          const auto new_vertex_id = graph.add_vertex();
          graph.add_edge(*vertex, new_vertex_id);
          vertices_next.emplace_back(new_vertex_id);
        }
      }
    }
    probability -= step;
    vertices = vertices_next;
    vertices_next.clear();
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  const auto& vertices = graph.get_vertices();
  for (auto vertex : vertices) {
    if (check_probability(constants::green_edges_probability)) {
      graph.add_edge(vertex.first, vertex.first);
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  double probability = 1.0;
  const double step = 1.0 / (params_.depth() - 1);
  const auto& depth_levels = graph.get_depth_levels();

  if (!depth_levels.empty()) {
    for (auto it_next = depth_levels.begin(), it_temp = it_next++;
         it_next != depth_levels.end(); ++it_next, ++it_temp) {
      for (auto vertex = it_temp->begin(); vertex != it_temp->end(); ++vertex) {
        if (check_probability(probability)) {
          int tries = 0;
          while (tries < it_next->size()) {
            ++tries;
            auto new_vertex_id = (*it_next)[get_random_vertex_id(it_next->size() - 1)];
            if (!graph.has_edge(*vertex, new_vertex_id)) {
              graph.add_edge(*vertex, new_vertex_id);
              break;
            }
          }
        }
      }
      probability += step;
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  const auto& depth_levels = graph.get_depth_levels();

  auto it_temp = depth_levels.begin();
  auto it_next = it_temp;
  for (int i = 0; i < 2 && it_next != depth_levels.end(); ++i, ++it_next)
    ;

  for (; it_next != depth_levels.end(); ++it_next, ++it_temp) {
    for (auto vertex = it_temp->begin(); vertex != it_temp->end(); ++vertex) {
      if (check_probability(constants::red_edges_probability)) {
        auto new_vertex_id = (*it_next)[get_random_vertex_id(it_next->size() - 1)];
        graph.add_edge(*vertex, new_vertex_id);
      }
    }
  }
}