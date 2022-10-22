#include "graph.hpp"

Graph::VertexId Graph::add_vertex() {
  const auto vertex_id = gen_new_vertex_id();
  const auto& vertex = Vertex(vertex_id);
  vertices_.insert(std::make_pair(vertex_id, vertex));

  Graph::Depth base_depth = 0;
  if (vertices_of_depth_.empty()) {
    std::vector<VertexId> EmptyVertex_ = {};
    vertices_of_depth_.emplace_back(EmptyVertex_);
    vertices_of_depth_.emplace_back(EmptyVertex_);
    base_depth = 1;
  }

  vertices_of_depth_[base_depth].emplace_back(vertex_id);
  depth_of_vertices_[vertex_id] = base_depth;
  connections_list_[vertex_id] = {};

  return vertex_id;
}

bool Graph::is_connected(const Graph::VertexId from_vertex_id,
                         const Graph::VertexId to_vertex_id) const {
  const auto PullEdgesFrom = connections_list_.at(from_vertex_id);
  const auto PullEdgesTo = connections_list_.at(to_vertex_id);

  std::set<Graph::EdgeId> intersection;

  std::set_intersection(PullEdgesFrom.begin(), PullEdgesFrom.end(),
                        PullEdgesTo.begin(), PullEdgesTo.end(),
                        std::inserter(intersection, intersection.begin()));

  return !intersection.empty();
}

void Graph::set_vertex_depth(const VertexId id, const Depth depth) {
  const auto cur_depth = get_vertex_depth(id);
  const auto graph_depth = get_graph_depth();

  if (depth > graph_depth) {
    for (Graph::Depth i = graph_depth; i < depth; i++) {
        std::vector<VertexId> EmptyVertex = {};
      vertices_of_depth_.emplace_back(EmptyVertex);
    }
  }

  depth_of_vertices_[id] = depth;
  vertices_of_depth_[depth].emplace_back(id);
  vertices_of_depth_[cur_depth].erase(
      std::remove(vertices_of_depth_[cur_depth].begin(),
                  vertices_of_depth_[cur_depth].end(), id));
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert(has_vertex(from_vertex_id));
  assert(has_vertex(to_vertex_id));
  const auto edge_id = gen_new_edge_id();

  const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
  const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
  Edge::Color color = Edge::Color::Grey;
  if (from_vertex_id == to_vertex_id) {
    color = Edge::Color::Green;
  } else if (to_vertex_depth - from_vertex_depth < 0) {
    color = Edge::Color::Grey;
    set_vertex_depth(to_vertex_id, from_vertex_depth + 1);
  } else if (to_vertex_depth - from_vertex_depth == 1 &&
             !is_connected(to_vertex_id, from_vertex_id)) {
    color = Edge::Color::Yellow;
  } else if (to_vertex_depth - from_vertex_depth == 2) {
    color = Edge::Color::Red;
  } else {
    std::runtime_error("Can't define the color");
  }

  edges_.insert(std::make_pair(
      edge_id, Edge(edge_id, from_vertex_id, to_vertex_id, color)));
  if (from_vertex_id != to_vertex_id) {
    connections_list_[from_vertex_id].insert(edge_id);
  }

  connections_list_[to_vertex_id].insert(edge_id);
}

