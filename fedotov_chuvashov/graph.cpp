#include "graph.hpp"
#include <cassert>
#include <iostream>
#include "printing.hpp"

Graph::VertexId Graph::add_vertex() {
  const VertexId new_id = get_new_vertex_id();
  vertices_.emplace(new_id, new_id);
  adjacency_list_.emplace(new_id, std::vector<EdgeId>{});
  children_of_vertex_.emplace(new_id, std::set<VertexId>{});
  set_vertex_depth(new_id, 0);
  return new_id;
}

Graph::EdgeId Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  assert(has_vertex(from_vertex_id));
  assert(has_vertex(to_vertex_id));
  const EdgeId new_id = get_new_edge_id();
  const Edge::Color edge_color = get_edge_color(from_vertex_id, to_vertex_id);
  if (edge_color == Edge::Color::Grey) {
    const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
    set_vertex_depth(to_vertex_id, from_vertex_depth + 1);
    vertices_at_depth_[from_vertex_depth + 1].insert(to_vertex_id);
  }
  edges_.try_emplace(new_id, new_id, from_vertex_id, to_vertex_id);
  adjacency_list_[from_vertex_id].emplace_back(new_id);
  children_of_vertex_[from_vertex_id].emplace(to_vertex_id);
  if (from_vertex_id != to_vertex_id) {
    adjacency_list_[to_vertex_id].emplace_back(new_id);
  }
  set_edge_color(new_id, edge_color);

  return new_id;
}

Graph::Edge::Color Graph::get_edge_color(EdgeId from_vertex_id,
                                         EdgeId to_vertex_id) const {
  const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
  const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
  if (from_vertex_id == to_vertex_id) {
    return Edge::Color::Green;
  }
  if (to_vertex_depth - from_vertex_depth == 1 &&
      !is_connected(from_vertex_id, to_vertex_id)) {
    return Edge::Color::Yellow;
  }
  if (to_vertex_depth - from_vertex_depth == 2) {
    return Edge::Color::Red;
  }
  return Edge::Color::Grey;
}
