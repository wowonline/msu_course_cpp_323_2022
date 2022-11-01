#include "graph.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

Graph::VertexId Graph::add_vertex() {
  const VertexId new_vertex_id = get_new_vertex_id();
  vertices_.insert({new_vertex_id, Vertex(new_vertex_id)});
  return new_vertex_id;
}

Graph::EdgeId Graph::add_edge(VertexId first_vertex_id,
                              VertexId second_vertex_id) {
  assert(has_vertex_id(first_vertex_id));
  assert(has_vertex_id(second_vertex_id));

  const EdgeId new_edge_id = get_new_edge_id();
  auto color = Graph::Edge::Color::Grey;

  if (first_vertex_id == second_vertex_id) {
    color = Graph::Edge::Color::Green;
    connections_[first_vertex_id].push_back(new_edge_id);
    edges_.insert({new_edge_id, Edge(new_edge_id, first_vertex_id,
                                     second_vertex_id, color)});

    return new_edge_id;
  }

  const auto first_vertex_depth = get_vertex_depth_in_basis(first_vertex_id);
  const auto second_vertex_depth = get_vertex_depth_in_basis(second_vertex_id);

  if (!second_vertex_depth) {
    color = Graph::Edge::Color::Grey;
    if (!first_vertex_depth) {
      set_vertex_depth(first_vertex_id, 1);
      set_vertex_depth(second_vertex_id, 2);
    } else {
      set_vertex_depth(second_vertex_id, first_vertex_depth + 1);
    }
  } else {
    if (second_vertex_depth - first_vertex_depth == 1)
      color = Graph::Edge::Color::Yellow;
    else if (second_vertex_depth - first_vertex_depth == 2) {
      color = Graph::Edge::Color::Red;
      set_vertex_depth(second_vertex_id, first_vertex_depth + 1);
    } else
      std::runtime_error("Failed to determine color");
  }

  connections_[first_vertex_id].push_back(new_edge_id);
  connections_[second_vertex_id].emplace_back(new_edge_id);

  edges_.insert({new_edge_id,
                 Edge(new_edge_id, first_vertex_id, second_vertex_id, color)});

  return new_edge_id;
};

Graph::Depth Graph::get_vertex_depth_in_basis(VertexId vertex_id) const {
  if (vertices_depth_.find(vertex_id) != vertices_depth_.end()) {
    return vertices_depth_.at(vertex_id).first;
  }
  return 0;
}

Graph::Depth Graph::get_vertex_depth(VertexId vertex_id) const {
  assert(has_vertex_id(vertex_id));
  if (vertices_depth_.find(vertex_id) != vertices_depth_.end()) {
    return vertices_depth_.at(vertex_id).second;
  }
  return 0;
}

void Graph::set_vertex_depth(VertexId vertex_id, Depth new_vertex_depth) {
  const auto old_vertex_depth = get_vertex_depth(vertex_id);
  if (get_vertex_depth_in_basis(vertex_id)) {
    if (old_vertex_depth > new_vertex_depth) {
      vertices_depth_.at(vertex_id).second = new_vertex_depth;
      const auto& edge_ids = get_connected_edge_ids(vertex_id);
      const auto& edges = get_edges();
      for (auto edge : edge_ids) {
        set_vertex_depth(edges.at(edge).get_second_vertex_id(),
                         new_vertex_depth + 1);
      }
    }
  } else {
    vertices_depth_[vertex_id].first = new_vertex_depth;
    vertices_depth_[vertex_id].second = new_vertex_depth;
    --new_vertex_depth;
    if (new_vertex_depth < depth_levels_.size()) {
      depth_levels_.at(new_vertex_depth).emplace_back(vertex_id);
    } else {
      depth_levels_.insert(
          {new_vertex_depth, std::vector<VertexId>{vertex_id}});
    }
  }
}

bool Graph::has_edge(VertexId first_vertex_id,
                     VertexId second_vertex_id) const {
  if (connections_.find(first_vertex_id) == connections_.end())
    return false;
  if (connections_.find(second_vertex_id) == connections_.end())
    return false;

  const auto& first_vertex_edge_ids = get_connected_edge_ids(first_vertex_id);
  const auto& second_vertex_edge_ids = get_connected_edge_ids(second_vertex_id);
  return std::find_first_of(
             first_vertex_edge_ids.begin(), first_vertex_edge_ids.end(),
             second_vertex_edge_ids.begin(),
             second_vertex_edge_ids.end()) != first_vertex_edge_ids.end();
}
