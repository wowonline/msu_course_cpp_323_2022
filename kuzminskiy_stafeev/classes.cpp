#include "classes.hpp"
#include "printing.hpp"

Graph::VertexId Graph::add_vertex() {
  const auto vertex_id = gen_new_vertex_id();
  const auto& vertex = Vertex(vertex_id);
  vertices_.insert(std::make_pair(vertex_id, vertex));

  Graph::Depth base_depth = 0;
  if (is_empty_) {
    std::vector<VertexId> EmptyVertex_ = {};
    vertices_of_depth_.emplace_back(EmptyVertex_);
    vertices_of_depth_.emplace_back(EmptyVertex_);
    max_depth_ = 1;
    base_depth = 1;
    is_empty_ = false;
  }

  vertices_of_depth_[base_depth].emplace_back(vertex_id);
  depth_of_vertices_[vertex_id] = base_depth;
  connections_list_[vertex_id] = {};

  return vertex_id;
}

bool Graph::is_connected(const Graph::VertexId& from_vertex_id,
                         const Graph::VertexId& to_vertex_id) const {
  const auto& PullEdgesFrom = connections_list_.at(from_vertex_id);
  const auto& PullEdgesTo = connections_list_.at(to_vertex_id);

  std::set<Graph::EdgeId> intersection;

  std::set_intersection(PullEdgesFrom.begin(), PullEdgesFrom.end(),
                        PullEdgesTo.begin(), PullEdgesTo.end(),
                        std::inserter(intersection, intersection.begin()));

  return !intersection.empty();
}

void Graph::set_vertex_depth(const VertexId& id, const Depth& depth) {
  auto cur_depth = get_vertex_depth(id);

  if (depth > max_depth_) {
    for (Graph::Depth i = max_depth_; i <= depth; i++) {
      std::vector<VertexId> EmptyVertex = {};
      vertices_of_depth_.emplace_back(EmptyVertex);
    }

    max_depth_ = depth;
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
  const auto& depth = params_.depth();
  const auto& new_vertices_count = params_.new_vertices_count();
  std::vector<Graph::VertexId> vertices_on_cur_depth = {(Graph::VertexId)0};
  std::random_device rd;
  std::mt19937 gen(rd());
  float step = 1.0 / (depth - 1);

  for (Graph::Depth i = 1; i <= depth; i++) {
    std::bernoulli_distribution d((float)(1 - step * (i - 1)));
    std::vector<Graph::VertexId> vertices_on_next_depth;
    for (auto from_vertex_id : vertices_on_cur_depth) {
      for (Graph::VertexId cur_vertex_count = 0;
           cur_vertex_count < new_vertices_count; cur_vertex_count++) {
        if (d(gen)) {
          auto vertex_id = graph.add_vertex();
          graph.add_edge(from_vertex_id, vertex_id);
          vertices_on_next_depth.emplace_back(vertex_id);
        }
      }
    }

    vertices_on_cur_depth.clear();
    vertices_on_cur_depth = vertices_on_next_depth;
  }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  static float constexpr prob = 0.1;
  std::random_device rd;
  std::mt19937 gen(rd());
  const auto& vertices = graph.vertices();
  std::bernoulli_distribution d(prob);
  for (const auto& [vertex_id, vertex] : vertices) {
    if (d(gen)) {
      graph.add_edge(vertex.id(), vertex.id());
    }
  }
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  auto depth = params_.depth();
  float step = 1.0 / (depth - 2);
  for (Graph::Depth i = 2; i <= depth - 1; i++) {
    std::bernoulli_distribution d((float)(step * (i - 1)));
    const auto cur_vertices_depth = graph.vertices_of_depth(i);
    const auto next_vertices_depth = graph.vertices_of_depth(i + 1);

    std::srand(std::time(0));
    for (const auto& from_vertex_id : cur_vertices_depth) {
      for (int j = 0; j < next_vertices_depth.size(); j++) {
        auto rand_pos = std::rand() % next_vertices_depth.size();
        auto to_vertex_id = next_vertices_depth[rand_pos];
        if (!graph.is_connected(from_vertex_id, to_vertex_id)) {
          if (d(gen)) {
            graph.add_edge(from_vertex_id, to_vertex_id);
          }
          break;
        }
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  const auto& depth = params_.depth();
  static float constexpr probability = (float)1 / 3;
  ;
  for (Graph::Depth i = 1; i <= depth - 2; i++) {
    std::bernoulli_distribution d(probability);
    const auto& cur_vertices_depth = graph.vertices_of_depth(i);
    const auto& next_vertices_depth = graph.vertices_of_depth(i + 2);

    for (const auto& from_vertex_id : cur_vertices_depth) {
      auto rand_pos = std::rand() % next_vertices_depth.size();
      auto to_vertex_id = next_vertices_depth[rand_pos];
      if (d(gen)) {
        graph.add_edge(from_vertex_id, to_vertex_id);
      }
    }
  }
}
