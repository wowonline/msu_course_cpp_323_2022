#include "graph.hpp"

Graph::Vertex::Vertex(VertexId id) : id_(id) {}

Graph::VertexId Graph::Vertex::id() const {
  return id_;
}

Graph::Edge::Edge(EdgeId id,
                  VertexId first_vertex_id,
                  VertexId second_vertex_id,
                  Edge::Color color)
    : id_(id),
      first_vertex_id_(first_vertex_id),
      second_vertex_id_(second_vertex_id),
      color_(color) {}

Graph::EdgeId Graph::Edge::id() const {
  return id_;
}

Graph::VertexId Graph::Edge::get_first_vertex_id() const {
  return first_vertex_id_;
}
Graph::VertexId Graph::Edge::get_second_vertex_id() const {
  return second_vertex_id_;
}

Graph::Edge::Color Graph::Edge::color() const {
  return color_;
}

Graph::VertexId Graph::add_vertex() {
  const VertexId new_vertex_id = get_new_vertex_id();
  Vertex new_vertex(new_vertex_id);

  vertices_.insert({new_vertex_id, Vertex(new_vertex_id)});
  return new_vertex_id;
}

Graph::EdgeId Graph::add_edge(VertexId first_vertex_id_,
                              VertexId second_vertex_id_) {
  assert(has_vertex_id(first_vertex_id_));
  assert(has_vertex_id(second_vertex_id_));

  const EdgeId new_edge_id = get_new_edge_id();
  auto color = Graph::Edge::Color::Green;

  if (first_vertex_id_ != second_vertex_id_) {
    connections_[second_vertex_id_].emplace_back(new_edge_id);
    color = Graph::Edge::Color::Grey;
  }
  connections_[first_vertex_id_].push_back(new_edge_id);

  edges_.insert({new_edge_id, Edge(new_edge_id, first_vertex_id_,
                                   second_vertex_id_, color)});

  return new_edge_id;
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

const std::vector<std::vector<Graph::VertexId>>& Graph::get_vertices_depth()
    const {
  return vertices_depth_;
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

// GraphGenerator's interface

GraphGenerator::Params::Params(Graph::Depth depth, int new_vertices_count)
    : depth_(depth), new_vertices_count_(new_vertices_count) {}

Graph::Depth GraphGenerator::Params::depth() const {
  return depth_;
}

int GraphGenerator::Params::new_vertices_count() const {
  return new_vertices_count_;
}

GraphGenerator::GraphGenerator(Params&& params) : params_(std::move(params)) {}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.add_vertex();
  generate_grey_edges(graph);
  generate_green_edges(graph);
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