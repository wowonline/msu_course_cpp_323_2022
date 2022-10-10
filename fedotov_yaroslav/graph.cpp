#include "graph.hpp"
#include <cassert>
#include <iostream>
#include "printing.hpp"

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id());
}

void Graph::add_edge(VertexId first_vertex_id, VertexId second_vertex_id) {
  assert(has_vertex(first_vertex_id));
  assert(has_vertex(second_vertex_id));
  edges_.emplace_back(get_new_edge_id(), first_vertex_id, second_vertex_id);
}

bool Graph::has_vertex(VertexId id) const {
  for (const auto& vertex : vertices_) {
    if (vertex.id() == id) {
      return true;
    }
  }
  return false;
}

Graph::VertexId Graph::get_new_vertex_id() {
  return last_vertex_id_++;
}

Graph::EdgeId Graph::get_new_edge_id() {
  return last_edge_id_++;
}

Graph::Vertex::Vertex(VertexId id) : id_(id) {}
Graph::VertexId Graph::Vertex::id() const {
  return id_;
}

Graph::Edge::Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
    : id_(id), from_vertex_id_(from_vertex_id), to_vertex_id_(to_vertex_id) {}

Graph::EdgeId Graph::Edge::id() const {
  return id_;
}
Graph::VertexId Graph::Edge::from_vertex_id() const {
  return from_vertex_id_;
}
Graph::VertexId Graph::Edge::to_vertex_id() const {
  return to_vertex_id_;
}

const std::vector<Graph::Vertex>& Graph::vertices() const {
  return vertices_;
}
const std::vector<Graph::Edge>& Graph::edges() const {
  return edges_;
}

constexpr int kVerticesCount = 14;

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

  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  printing::write_to_file(graph_json, "graph.json");
}
