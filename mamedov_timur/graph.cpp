#include "graph.hpp"
#include <cassert>

void Graph::add_vertex() {
  vertexes.emplace_back(get_new_vertex_id());
}

bool Graph::has_vertex(VertexId id) const {
  for (auto vertex : vertexes) {
    if (vertex.id() == id)
      return true;
  }
  return false;
}

void Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  assert(has_vertex(from_vertex_id));
  assert(has_vertex(to_vertex_id));
  edges.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
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

Graph::VertexId Graph::get_new_vertex_id() {
  return last_vertex_id_++;
}
Graph::EdgeId Graph::get_new_edge_id() {
  return last_edge_id_++;
}
