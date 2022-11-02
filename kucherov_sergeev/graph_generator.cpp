#include "graph_generator.hpp"

Graph GraphGenerator::generate() const {
  auto graph = Graph();

  if (params_.depth() != 0) {
    graph.add_vertex();
    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
  }

  return graph;
}

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  for (Graph::Depth current_depth = 1; current_depth < params_.depth();
       current_depth++) {
    float new_vertex_probability =
        1.f - (current_depth - 1.f) / (params_.depth() - 1.f);

    for (const auto vertex_id : graph.get_depth_vertex_ids(current_depth)) {
      for (int attempt = 0; attempt < params_.new_vertices_count(); attempt++) {
        if (get_random_bool(new_vertex_probability)) {
          const auto new_vertex_id = graph.add_vertex();
          graph.add_edge(vertex_id, new_vertex_id);
        }
      }
    }
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (Graph::Depth current_depth = 1; current_depth <= params_.depth();
       current_depth++) {
    for (const auto vertex_id : graph.get_depth_vertex_ids(current_depth)) {
      if (get_random_bool(kEdgeGreenProbability)) {
        graph.add_edge(vertex_id, vertex_id);
      }
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  for (Graph::Depth current_depth = 2; current_depth < params_.depth();
       current_depth++) {
    float new_edge_probability = current_depth / (params_.depth() - 1.f);

    for (const auto vertex_id : graph.get_depth_vertex_ids(current_depth)) {
      if (get_random_bool(new_edge_probability)) {
        const auto& to_vertex_ids =
            get_unconnected_vertex_ids(graph, vertex_id);

        if (to_vertex_ids.size() != 0) {
          const auto to_vertex_id = get_random_vertex_id(to_vertex_ids);
          graph.add_edge(vertex_id, to_vertex_id);
        }
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  for (Graph::Depth current_depth = 1; current_depth < params_.depth() - 1;
       current_depth++) {
    const auto& to_vertex_ids = graph.get_depth_vertex_ids(current_depth + 2);

    if (to_vertex_ids.size() == 0) {
      break;
    }

    for (const auto vertex_id : graph.get_depth_vertex_ids(current_depth)) {
      if (get_random_bool(kEdgeRedProbability)) {
        const auto to_vertex_id = get_random_vertex_id(to_vertex_ids);
        graph.add_edge(vertex_id, to_vertex_id);
      }
    }
  }
}