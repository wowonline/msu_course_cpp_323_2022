#include "graph.hpp"

void Graph::add_vertex() {
  vertexes_.emplace_back(get_new_vertex_id());
}

void Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
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
