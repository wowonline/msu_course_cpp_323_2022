#include "graph_generator.hpp"

namespace uni_course_cpp {

namespace {

static constexpr double kProbabilityRed = 0.33;
static constexpr double kProbabilityGreen = 0.1;

bool check_probability(double chance) {
  std::mt19937 generator{std::random_device()()};
  std::bernoulli_distribution distribution(chance);
  return distribution(generator);
}

double probaility_generate_grey_edge(Graph::Depth current_depth,
                                     Graph::Depth graph_depth) {
  if (graph_depth == 0) {
    return 1.0;
  } else {
    return 1.0 - static_cast<double>((current_depth - Graph::kBaseDepth)) /
                     (graph_depth - Graph::kBaseDepth);
  }
}

Graph::VertexId get_random_vertex_id(int size) {
  std::random_device rand_device;
  std::mt19937 gen(rand_device());
  std::uniform_int_distribution<> distrib(0, size - 1);
  return distrib(gen);
}

std::vector<Graph::VertexId> get_unconnected_vertex_ids(
    const Graph& graph,
    Graph::VertexId vertex_from_id,
    const std::vector<Graph::VertexId>& vertex_ids) {
  std::vector<Graph::VertexId> not_connected_vertex_ids;
  for (const auto& vertex_to_id : vertex_ids) {
    if (!graph.is_connected(vertex_from_id, vertex_to_id)) {
      not_connected_vertex_ids.emplace_back(vertex_to_id);
    }
  }
  return not_connected_vertex_ids;
}

void try_generate_red_edge(Graph& graph,
                           Graph::VertexId vertex_from_id,
                           Graph::VertexId vertex_to_id) {
  const Graph::Depth vertex_from_depth = graph.vertex_depth(vertex_from_id);
  if (check_probability(kProbabilityRed)) {
    graph.add_edge(vertex_from_id, vertex_to_id);
  }
}

void generate_red_edges(Graph& graph) {
  for (const auto& vertex_from : graph.get_vertices()) {
    const auto vertex_from_id = vertex_from.id();
    const Graph::Depth vertex_depth = graph.vertex_depth(vertex_from_id);
    const auto& vertex_ids =
        graph.get_vertices_with_depth(vertex_depth + Graph::kDifferenceRedEdge);
    if (!vertex_ids.empty()) {
      const Graph::VertexId vertex_to_id =
          vertex_ids.at(get_random_vertex_id(vertex_ids.size()));
      try_generate_red_edge(graph, vertex_from_id, vertex_to_id);
    }
  }
}

void try_generate_green_edge(Graph& graph, Graph::VertexId vertex_id) {
  if (check_probability(kProbabilityGreen)) {
    graph.add_edge(vertex_id, vertex_id);
  }
}

void generate_green_edges(Graph& graph) {
  for (const auto& vertex : graph.get_vertices()) {
    try_generate_green_edge(graph, vertex.id());
  }
}

}  // namespace

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

void GraphGenerator::try_generate_grey_edge(Graph& graph,
                                            Graph::Depth current_depth,
                                            Graph::VertexId vertex_id) const {
  const auto probability =
      probaility_generate_grey_edge(current_depth, params_.depth());
  if (check_probability(probability)) {
    const Graph::VertexId next_vertex_id = graph.add_vertex();
    graph.add_edge(vertex_id, next_vertex_id);
  }
}

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  for (Graph::Depth current_depth = Graph::kBaseDepth;
       current_depth <= params_.depth(); current_depth++) {
    const auto vertices_with_last_depth =
        graph.get_vertices_with_depth(current_depth);
    if (graph.depth() != current_depth) {
      break;
    }
    for (const auto& vertex_id : vertices_with_last_depth) {
      for (int i = 0; i < params_.new_vertices_count(); ++i) {
        try_generate_grey_edge(graph, current_depth, vertex_id);
      }
    }
  }
}

void GraphGenerator::try_generate_yellow_edge(
    Graph& graph,
    Graph::VertexId vertex_from_id,
    Graph::VertexId vertex_to_id) const {
  const Graph::Depth vertex_from_depth = graph.vertex_depth(vertex_from_id);
  const double probability_generate =
      1.0 - static_cast<double>((vertex_from_depth - Graph::kBaseDepth)) /
                (params_.depth() - Graph::kBaseDepth);
  if (check_probability(probability_generate)) {
    graph.add_edge(vertex_from_id, vertex_to_id);
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {  //
  for (const auto& vertex_from : graph.get_vertices()) {
    const auto vertex_from_id = vertex_from.id();
    const Graph::Depth vertex_depth = graph.vertex_depth(vertex_from_id);
    const auto& vertex_ids = graph.get_vertices_with_depth(
        vertex_depth + Graph::kDifferenceYellowEdge);
    const auto not_connected_vertex_ids =
        get_unconnected_vertex_ids(graph, vertex_from_id, vertex_ids);
    if (!not_connected_vertex_ids.empty()) {
      const Graph::VertexId vertex_to_id = not_connected_vertex_ids.at(
          get_random_vertex_id(not_connected_vertex_ids.size()));
      try_generate_yellow_edge(graph, vertex_from_id, vertex_to_id);
    }
  }
}

}  // namespace uni_course_cpp
