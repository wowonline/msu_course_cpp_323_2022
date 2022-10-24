#include "graph.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

Graph::VertexId Graph::add_vertex() {
  const VertexId new_vertex_id = get_new_vertex_id();
  Vertex new_vertex(new_vertex_id);

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

  auto first_vertex_depth = get_vertex_depth(first_vertex_id);
  auto second_vertex_depth = get_vertex_depth(second_vertex_id);

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
      set_new_vertex_depth(second_vertex_id, second_vertex_depth,
                           first_vertex_depth + 1);
    } else
      std::runtime_error("Failed to determine color");
  }

  connections_[first_vertex_id].push_back(new_edge_id);
  connections_[second_vertex_id].emplace_back(new_edge_id);

  edges_.insert({new_edge_id,
                 Edge(new_edge_id, first_vertex_id, second_vertex_id, color)});

  return new_edge_id;
};

Graph::Depth Graph::get_vertex_depth(VertexId vertex_id) const {
  for (auto it = vertices_depth_.begin(); it != vertices_depth_.end(); ++it) {
    if (std::find(it->second.begin(), it->second.end(), vertex_id) !=
        it->second.end())
      return it->first + 1;
  }
  return 0;
}

void Graph::set_new_vertex_depth(VertexId vertex_id,
                                 Depth old_vertex_depth,
                                 Depth new_vertex_depth) {
  --old_vertex_depth;
  vertices_depth_[old_vertex_depth].erase(
      std::find(vertices_depth_[old_vertex_depth].begin(),
                vertices_depth_[old_vertex_depth].end(), vertex_id));
  set_vertex_depth(vertex_id, new_vertex_depth);
}

void Graph::set_vertex_depth(VertexId vertex_id, Depth vertex_depth) {
  --vertex_depth;
  if (vertex_depth < vertices_depth_.size())
    vertices_depth_.at(vertex_depth).emplace_back(vertex_id);
  else
    vertices_depth_.insert({vertex_depth, std::vector<VertexId>{vertex_id}});
}

bool Graph::has_edge(VertexId first_vertex_id,
                     VertexId second_vertex_id) const {
  if (connections_.find(first_vertex_id) == connections_.end())
    return false;
  if (connections_.find(second_vertex_id) == connections_.end())
    return false;

  const auto first_vertex_edges = get_connected_edge_ids(first_vertex_id);
  const auto second_vertex_edges = get_connected_edge_ids(second_vertex_id);
  return std::find_first_of(
             first_vertex_edges.begin(), first_vertex_edges.end(),
             second_vertex_edges.begin(),
             second_vertex_edges.end()) != first_vertex_edges.end();
}

// GraphGenerator's interface

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
  double step = 1.0 / max_depth;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<Graph::VertexId> vertices = {graph.get_vertices().begin()->first};
  std::vector<Graph::VertexId> vertices_next;

  for (Graph::Depth temp_depth = 0; temp_depth < max_depth; ++temp_depth) {
    std::bernoulli_distribution d(probability);
    for (auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex) {
      for (int j = 0; j < new_vertices_count; ++j) {
        if (d(gen)) {
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
  std::random_device rd;
  std::mt19937 gen(rd());
  double probability = 0.1;
  std::bernoulli_distribution d(probability);
  const auto vertices = graph.get_vertices();
  for (auto vertex : vertices) {
    if (d(gen)) {
      graph.add_edge(vertex.first, vertex.first);
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  double probability = 1.0;
  double step = 1.0 / (params_.depth() - 1);
  std::random_device rd;
  std::mt19937 gen(rd());
  const auto vertices = graph.get_vertices_depth();

  auto it_temp = vertices.begin();
  auto it_next = it_temp;
  if (it_next != vertices.end())
    ++it_next;

  for (; it_next != vertices.end(); ++it_next, ++it_temp) {
    std::bernoulli_distribution d(probability);
    std::uniform_int_distribution<> distrib(0, it_next->second.size() - 1);
    for (auto vertex = it_temp->second.begin(); vertex != it_temp->second.end();
         ++vertex) {
      if (d(gen)) {
        int tries = 0;
        while (tries < it_next->second.size()) {
          ++tries;
          auto new_vertex_id = it_next->second[distrib(gen)];
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

void GraphGenerator::generate_red_edges(Graph& graph) const {
  double probability = 0.33;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  const auto vertices = graph.get_vertices_depth();

  auto it_temp = vertices.begin();
  auto it_next = it_temp;
  for (int i = 0; i < 2 && it_next != vertices.end(); ++i, ++it_next)
    ;

  for (; it_next != vertices.end(); ++it_next, ++it_temp) {
    std::uniform_int_distribution<> distrib(0, it_next->second.size() - 1);
    for (auto vertex = it_temp->second.begin(); vertex != it_temp->second.end();
         ++vertex) {
      if (d(gen)) {
        auto new_vertex_id = it_next->second[distrib(gen)];
        graph.add_edge(*vertex, new_vertex_id);
      }
    }
  }
}
