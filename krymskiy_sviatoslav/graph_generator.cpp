#include <cassert>
#include <unordered_map>
#include <vector>

#include "graph_generator.hpp"

constexpr int kVerticesCount = 14;

Graph::VertexId Graph::get_new_vertex_id() {
  return current_vertex_id_++;
}

Graph::EdgeId Graph::get_new_edge_id() {
  return current_edge_id_++;
}

bool Graph::has_vertex(Graph::VertexId id) const {
  return vertices_.find(id) != vertices_.end();
}

void Graph::add_vertex() {
  const VertexId id = get_new_vertex_id();
  vertices_.emplace(id, id);
}

void Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  assert(has_vertex(from_vertex_id));
  assert(has_vertex(to_vertex_id));
  const EdgeId id = get_new_edge_id();
  edges_.emplace_back(id, from_vertex_id, to_vertex_id);
}

int main() {
  auto graph = Graph();
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
  graph.add_edge(5, 10);
  graph.add_edge(6, 10);
  graph.add_edge(7, 11);
  graph.add_edge(8, 11);
  graph.add_edge(9, 12);
  graph.add_edge(10, 13);
  graph.add_edge(11, 13);
  graph.add_edge(12, 13);

  return graph;
  return 0;
}
