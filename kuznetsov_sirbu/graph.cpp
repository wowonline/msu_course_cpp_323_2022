#include "graph.hpp"
#include <algorithm>
#include <vector>
#include "graph_json_printing.hpp"

namespace uni_course_cpp {

Graph::VertexId Graph::add_vertex() {
  const VertexId new_vertex_id = get_new_vertex_id();
    vertices_.emplace_back(new_vertex_id);
    vertex_depths_[new_vertex_id] = kBaseDepth;
    adjacency_list_of_edges_[new_vertex_id] = {};
    depth_to_vertices_[kBaseDepth].emplace_back(new_vertex_id);
    return new_vertex_id;
}

void Graph::set_vertex_depth(Graph::VertexId vertex_id, Depth depth) {
  depth_to_vertices_[depth].emplace_back(vertex_id);
    const auto pos = std::find(depth_to_vertices_[kBaseDepth].begin(),
                               depth_to_vertices_[kBaseDepth].end(), vertex_id);
    if (pos != depth_to_vertices_[kBaseDepth].end()) {
      depth_to_vertices_[kBaseDepth].erase(pos);
    }
    vertex_depths_[vertex_id] = depth;
}

bool Graph::is_connected(Graph::VertexId from_vertex_id,
                         Graph::VertexId to_vertex_id) const {
  std::vector<EdgeId> edges_vector =
        adjacency_list_of_edges_.at(from_vertex_id);
    for (size_t i = 0; i < edges_vector.size(); i++) {
      if (edges_[edges_vector[i]].from_vertex_id() == to_vertex_id ||
          edges_[edges_vector[i]].to_vertex_id() == to_vertex_id) {
        return 1;
      }
    }

    return 0;
}

const std::vector<Graph::EdgeId>& Graph::connected_edges_ids(
    Graph::VertexId vertex_id) const {
  if (!has_vertex(vertex_id)) {
      static std::vector<EdgeId> empty_edges_list;
      return empty_edges_list;
    }
    return adjacency_list_of_edges_.at(vertex_id);
}

Graph::Edge::Color Graph::get_edge_color(Graph::VertexId from_vertex_id,
                                         Graph::VertexId to_vertex_id) const {
  const auto from_vertex_depth = vertex_depths_.at(from_vertex_id);
    const auto to_vertex_depth = vertex_depths_.at(to_vertex_id);

    if (from_vertex_id == to_vertex_id) {
      return Edge::Color::Green;
    }
    if (adjacency_list_of_edges_.at(to_vertex_id).size() == 0) {
      return Edge::Color::Grey;
    }
    if (to_vertex_depth - from_vertex_depth == 1 &&
        !is_connected(from_vertex_id, to_vertex_id)) {
      return Edge::Color::Yellow;
    }
    if (to_vertex_depth - from_vertex_depth == 2) {
      return Edge::Color::Red;
    }
    throw std::runtime_error("Failed to determine color");
}

Graph::EdgeId Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  const auto color = get_edge_color(from_vertex_id, to_vertex_id);

    EdgeId new_id = get_new_edge_id();
    if (color == Edge::Color::Grey) {
      set_vertex_depth(to_vertex_id, vertex_depth(from_vertex_id) + 1);
    }
    color_to_edges_[color].emplace_back(new_id);
    adjacency_list_of_edges_[from_vertex_id].emplace_back(new_id);
    adjacency_list_of_edges_[to_vertex_id].emplace_back(new_id);
    edges_.emplace_back(new_id, from_vertex_id, to_vertex_id, color);

    return new_id;
}

const std::vector<Graph::VertexId>& Graph::get_vertices_with_depth(Depth depth) const {
  if (depth_to_vertices_.find(depth) != depth_to_vertices_.end()) {
      return depth_to_vertices_.at(depth);
    } else {
      std::vector<VertexId> tmp;
      return std::move(tmp);
    }
}

std::vector<Graph::EdgeId> Graph::get_edges_with_color(Edge::Color color) const { // ???? 
  if (color_to_edges_.find(color) != color_to_edges_.end()) {
    return color_to_edges_.at(color);
  } else {
    return {};
  }
}

}  // namespace uni_course_cpp