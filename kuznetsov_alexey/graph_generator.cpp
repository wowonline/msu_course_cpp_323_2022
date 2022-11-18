#include "graph_generator.hpp"

namespace uni_course_cpp {

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  if (params_.depth() < 0) {
    throw std::runtime_error("Graph depth < 0");
  }
  if (params_.depth() > 0) {
    graph.add_vertex();
    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
  }
  return graph;
}

double GraphGenerator::generate_random(double a, double b) const {
  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(a, b);
  return dis(gen);
}

double GraphGenerator::probaility_generate_grey_edge(
    Graph::Depth current_depth,
    Graph::Depth graph_depth) const {
  if (graph_depth == 0) {
    return 0;
  } else {
    return 1.0 - static_cast<double>((current_depth - 1)) / (graph_depth - 1);
  }
}

bool GraphGenerator::try_generate_grey_edge(Graph& graph,
                                            Graph::Depth graph_depth,
                                            Graph::Depth current_depth,
                                            Graph::VertexId vertex_id,
                                            Graph::VertexId& next_vertex_id)
    const {  // generate grey edge: (vertex_id, next_vertex_id)
  if (probaility_generate_grey_edge(current_depth, graph_depth) >
      generate_random(0, 1)) {
    graph.add_vertex();  // add vertex with id = vertex_id
    graph.add_edge(vertex_id, next_vertex_id);
    next_vertex_id++;
    return 1;
  } else {
    return 0;
  }
}

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  std::vector<Graph::VertexId> vertices_with_last_depth = {0};  // root = 0
  Graph::VertexId next_vertex_id = 1;
  for (Graph::Depth current_depth = 1; current_depth <= params_.depth();
       current_depth++) {
    std::vector<Graph::VertexId> new_vertices_with_current_depth;
    for (auto& vertex_id : vertices_with_last_depth) {
      for (int i = 0; i < params_.new_vertices_count(); ++i) {
        if (try_generate_grey_edge(graph, params_.depth(), current_depth,
                                   vertex_id, next_vertex_id)) {
          new_vertices_with_current_depth.emplace_back(next_vertex_id - 1);
        }
      }
    }
    vertices_with_last_depth = new_vertices_with_current_depth;
  }
}

void GraphGenerator::try_generate_yellow_edge(Graph& graph,
                                              Graph::Depth graph_depth,
                                              const Graph::Vertex& vertex_from,
                                              Graph::Vertex& vertex_to) const {
  Graph::Depth vertex_from_depth = vertex_from.depth();
  double probability_generate =
      static_cast<double>((vertex_from_depth - 1)) / (graph_depth - 1);
  if (generate_random(0, 1) > probability_generate) {
    if (graph.is_connected(vertex_from.id(), vertex_to.id())) {
      return;
    }
    graph.add_edge(vertex_from.id(), vertex_to.id());
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  for (auto& vertex_from : graph.get_vertices()) {
    Graph::Depth vertex_depth = vertex_from.depth();
    for (auto& vertex_to :
         graph.get_vertices_with_depth(vertex_from.depth() + 1)) {
      try_generate_yellow_edge(graph, params_.depth(), vertex_from, vertex_to);
    }
  }
}

void GraphGenerator::try_generate_red_edge(Graph& graph,
                                           const Graph::Vertex& vertex_from,
                                           Graph::Vertex& vertex_to) const {
  Graph::Depth vertex_from_depth = vertex_from.depth();
  if (generate_random(0, 1) < 0.33) {
    graph.add_edge(vertex_from.id(), vertex_to.id());
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (auto& vertex_from : graph.get_vertices()) {
    Graph::Depth vertex_depth = vertex_from.depth();
    for (auto& vertex_to :
         graph.get_vertices_with_depth(vertex_from.depth() + 2)) {
      try_generate_red_edge(graph, vertex_from, vertex_to);
    }
  }
}

void GraphGenerator::try_generate_green_edge(
    Graph& graph,
    const Graph::Vertex& vertex) const {
  if (generate_random(0, 1) < 0.1) {
    graph.add_edge(vertex.id(), vertex.id());
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (auto& vertex : graph.get_vertices()) {
    try_generate_green_edge(graph, vertex);
  }
}

}  // namespace uni_course_cpp
