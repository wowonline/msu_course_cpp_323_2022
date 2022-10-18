#include "graph.hpp"
#include <cassert>

Graph::Vertex::Vertex(VertexId id) : id_(id) {}

Graph::VertexId Graph::Vertex::id() const {
  return id_;
}

Graph::Edge::Edge(EdgeId id,
                  VertexId first_vertex_id,
                  VertexId second_vertex_id)
    : id_(id),
      first_vertex_id_(first_vertex_id),
      second_vertex_id_(second_vertex_id) {}

Graph::EdgeId Graph::Edge::id() const {
  return id_;
}

Graph::VertexId Graph::Edge::get_first_vertex_id() const {
  return first_vertex_id_;
}
Graph::VertexId Graph::Edge::get_second_vertex_id() const {
  return second_vertex_id_;
}

void Graph::add_vertex() {
  const VertexId new_vertex_id = get_new_vertex_id();
  vertices_.insert({new_vertex_id, Vertex(new_vertex_id)});
};

void Graph::add_edge(VertexId first_vertex_id_, VertexId second_vertex_id_) {
  assert(has_vertex_id(first_vertex_id_));
  assert(has_vertex_id(second_vertex_id_));

  const EdgeId new_edge_id = get_new_edge_id();
  edges_.insert(
      {new_edge_id, Edge(new_edge_id, first_vertex_id_, second_vertex_id_)});

  if (first_vertex_id_ != second_vertex_id_) connections_[second_vertex_id_].emplace_back(new_edge_id);
  connections_[first_vertex_id_].push_back(new_edge_id);
};

const std::unordered_map<Graph::EdgeId, Graph::Edge>& Graph::get_edges() const {
  return edges_;
}

const std::unordered_map<Graph::VertexId, Graph::Vertex>& Graph::get_vertices()
    const {
  return vertices_;
}

const std::vector<Graph::EdgeId>& Graph::get_edges_of_vertex(
    Graph::VertexId vertex_id) const {
  return connections_.at(vertex_id);
}

bool Graph::has_vertex_id(VertexId vertex_id) const {
  return vertices_.find(vertex_id) != vertices_.end();
}

Graph::VertexId Graph::get_new_vertex_id() {
  return last_vertex_id_++;
};

Graph::EdgeId Graph::get_new_edge_id() {
  return last_edge_id_++;
};
