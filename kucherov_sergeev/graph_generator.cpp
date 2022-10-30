#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

const int kVerticesCount = 14;
static constexpr float kEdgeGreenProbability = 0.1;
static constexpr float kEdgeRedProbability = 0.33;

bool get_random_bool(float true_probability) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::bernoulli_distribution bernoulli_distribution(true_probability);
  return bernoulli_distribution(generator);
}

void write_to_file(const std::string& graph_json,
                   const std::string& file_name) {
  std::ofstream json_file(file_name);

  json_file << graph_json;
}

int handle_depth_input() {
  const std::string init_message = "Type graph depth: ";
  const std::string err_format_message =
      "The depth must be a non-negative integer. Try again";

  int depth;
  int correct_input = false;

  std::cout << init_message << std::endl;

  while (correct_input == false) {
    if (std::cin >> depth && depth >= 0) {
      correct_input = true;
    } else if (std::cin.fail() || depth < 0) {
      std::cout << err_format_message << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      throw std::runtime_error("Failed to get depth from user");
    }
  }

  return depth;
}

int handle_new_vertices_count_input() {
  const std::string init_message = "Type new vertices count: ";
  const std::string err_format_message =
      "Vertices count must be a non-negative integer. Try again";
  int new_vertices_count;
  int correct_input = false;

  std::cout << init_message << std::endl;

  while (correct_input == false) {
    if (std::cin >> new_vertices_count && new_vertices_count >= 0) {
      correct_input = true;
    } else if (std::cin.fail() || new_vertices_count < 0) {
      std::cout << err_format_message << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      throw std::runtime_error("Failed to get new vertices count from user");
    }
  }

  return new_vertices_count;
}

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  struct Edge {
   public:
    enum class Color { Grey, Green, Yellow, Red };

    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id, Color color)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id),
          color_(color) {}

    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }
    Color color() const { return color_; }

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
    Color color_ = Color::Grey;
  };

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

   private:
    VertexId id_ = 0;
  };

  VertexId add_vertex();
  EdgeId add_edge(VertexId from_vertex_id, VertexId to_vertex_id);
  Depth get_depth() const;
  const std::vector<VertexId>& get_depth_vertex_ids(Depth depth) const;
  const std::vector<EdgeId>& get_connected_edge_ids(VertexId vertex_id) const;
  bool is_vertices_connected(VertexId first_vertex_id,
                             VertexId second_vertex_id) const;
  Depth get_vertex_depth(VertexId vertex_id) const;
  const std::vector<Vertex>& get_vertices() const;
  const std::vector<Edge>& get_edges() const;

 private:
  VertexId get_new_vertex_id();

  EdgeId get_new_edge_id();

  Edge::Color determine_edge_color(VertexId from_vertex_id,
                                   VertexId to_vertex_id) const;

  void set_vertex_depth(VertexId vertex_id, Depth depth);

  VertexId next_free_vertex_id_ = 0;
  EdgeId next_free_edge_id_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
  std::unordered_map<VertexId, Depth> vertex_depths_list_;
  std::vector<std::vector<VertexId>> depth_vertices_list_;

  const Depth default_vertex_depth_ = 1;
};

Graph::VertexId Graph::add_vertex() {
  const VertexId vertex_id = get_new_vertex_id();

  vertices_.emplace_back(vertex_id);
  set_vertex_depth(vertex_id, default_vertex_depth_);

  return vertex_id;
}

Graph::EdgeId Graph::add_edge(Graph::VertexId from_vertex_id,
                              Graph::VertexId to_vertex_id) {
  if (adjacency_list_.find(to_vertex_id) == adjacency_list_.end() ||
      adjacency_list_.at(to_vertex_id).empty()) {
    set_vertex_depth(to_vertex_id, get_vertex_depth(from_vertex_id) + 1);
  }

  const auto edge_id = get_new_edge_id();
  const auto edge_color = determine_edge_color(from_vertex_id, to_vertex_id);

  edges_.emplace_back(edge_id, from_vertex_id, to_vertex_id, edge_color);

  adjacency_list_[from_vertex_id].push_back(edge_id);
  if (to_vertex_id != from_vertex_id) {
    adjacency_list_[to_vertex_id].push_back(edge_id);
  }

  return edge_id;
}

Graph::Depth Graph::get_depth() const {
  return (depth_vertices_list_.size() - 1);
}

const std::vector<Graph::VertexId>& Graph::get_depth_vertex_ids(
    Graph::Depth depth) const {
  if (depth > get_depth()) {
    static const std::vector<VertexId> empty_result;
    return empty_result;
  }

  return depth_vertices_list_.at(depth);
}

const std::vector<Graph::EdgeId>& Graph::get_connected_edge_ids(
    Graph::VertexId vertex_id) const {
  if (adjacency_list_.find(vertex_id) == adjacency_list_.end()) {
    static const std::vector<Graph::EdgeId> empty_result;
    return empty_result;
  }

  return adjacency_list_.at(vertex_id);
}

bool Graph::is_vertices_connected(Graph::VertexId first_vertex_id,
                                  Graph::VertexId second_vertex_id) const {
  const auto& connected_edge_ids = get_connected_edge_ids(first_vertex_id);

  for (const auto& edge_id : connected_edge_ids) {
    const auto& current_edge = edges_[edge_id];
    if ((current_edge.from_vertex_id() ^ current_edge.to_vertex_id() ^
         first_vertex_id) == second_vertex_id) {
      return true;
    }
  }

  return false;
}

Graph::Depth Graph::get_vertex_depth(Graph::VertexId vertex_id) const {
  return vertex_depths_list_.at(vertex_id);
}

const std::vector<Graph::Vertex>& Graph::get_vertices() const {
  return vertices_;
}

const std::vector<Graph::Edge>& Graph::get_edges() const {
  return edges_;
}

Graph::VertexId Graph::get_new_vertex_id() {
  return next_free_vertex_id_++;
}

Graph::EdgeId Graph::get_new_edge_id() {
  return next_free_edge_id_++;
}

Graph::Edge::Color Graph::determine_edge_color(
    Graph::VertexId from_vertex_id,
    Graph::VertexId to_vertex_id) const {
  const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
  const auto to_vertex_depth = get_vertex_depth(to_vertex_id);

  if (from_vertex_id == to_vertex_id) {
    return Graph::Edge::Color::Green;
  }
  if (get_connected_edge_ids(to_vertex_id).size() == 0) {
    return Graph::Edge::Color::Grey;
  }
  if (to_vertex_depth - from_vertex_depth == 1 &&
      !is_vertices_connected(from_vertex_id, to_vertex_id)) {
    return Graph::Edge::Color::Yellow;
  }
  if (to_vertex_depth - from_vertex_depth == 2) {
    return Graph::Edge::Color::Red;
  }

  throw std::runtime_error("Failed to determine color");
}

void Graph::set_vertex_depth(Graph::VertexId vertex_id, Graph::Depth depth) {
  while (get_depth() < depth) {
    depth_vertices_list_.push_back({});
  }

  if (vertex_depths_list_.find(vertex_id) != vertex_depths_list_.end()) {
    const Depth previous_depth = get_vertex_depth(vertex_id);

    auto& previous_depth_vertices_list = depth_vertices_list_[previous_depth];
    previous_depth_vertices_list.erase(
        std::remove(previous_depth_vertices_list.begin(),
                    previous_depth_vertices_list.end(), vertex_id),
        previous_depth_vertices_list.end());
  }

  depth_vertices_list_[depth].push_back(vertex_id);
  vertex_depths_list_[vertex_id] = depth;
}

Graph::VertexId get_random_vertex_id(std::vector<Graph::VertexId> vertex_ids) {
  if (vertex_ids.empty()) {
    throw std::runtime_error("Can't pick random vertex id from empty list");
  }

  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> uniform_int_distribution(
      0, vertex_ids.size() - 1);

  return vertex_ids[uniform_int_distribution(generator)];
}

std::vector<Graph::VertexId> get_unconnected_vertex_ids(
    const Graph& graph,
    Graph::VertexId vertex_id) {
  std::vector<Graph::VertexId> unconnected_vertex_ids = {};
  for (const auto next_depth_vertex_id :
       graph.get_depth_vertex_ids(graph.get_vertex_depth(vertex_id) + 1)) {
    if (graph.is_vertices_connected(vertex_id, next_depth_vertex_id) == false) {
      unconnected_vertex_ids.push_back(next_depth_vertex_id);
    }
  }

  return unconnected_vertex_ids;
}

class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth, int new_vertices_count)
        : depth_(depth), new_vertices_count_(new_vertices_count) {}

    Graph::Depth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(Params&& params) : params_(std::move(params)) {}

  Graph generate() const {
    auto graph = Graph();
    graph.add_vertex();
    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);

    return graph;
  }

 private:
  void generate_grey_edges(Graph& graph) const {
    for (Graph::Depth current_depth = 1; current_depth < params_.depth();
         current_depth++) {
      float new_vertex_probability =
          1.f - (current_depth - 1.f) / (params_.depth() - 1.f);

      for (const auto vertex_id : graph.get_depth_vertex_ids(current_depth)) {
        for (int attempt = 0; attempt < params_.new_vertices_count();
             attempt++) {
          if (get_random_bool(new_vertex_probability)) {
            const auto new_vertex_id = graph.add_vertex();
            graph.add_edge(vertex_id, new_vertex_id);
          }
        }
      }
    }
  }

  void generate_green_edges(Graph& graph) const {
    for (Graph::Depth current_depth = 1; current_depth <= params_.depth();
         current_depth++) {
      for (const auto vertex_id : graph.get_depth_vertex_ids(current_depth)) {
        if (get_random_bool(kEdgeGreenProbability)) {
          graph.add_edge(vertex_id, vertex_id);
        }
      }
    }
  }

  void generate_yellow_edges(Graph& graph) const {
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

  void generate_red_edges(Graph& graph) const {
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

  Params params_ = Params(0, 0);
};

namespace printing {
namespace json {

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::string vertex_json =
      "{\"id\":" + std::to_string(vertex.id()) + ",\"edge_ids\":[";
  const auto& connected_edge_ids = graph.get_connected_edge_ids(vertex.id());

  if (connected_edge_ids.size() != 0) {
    for (const auto edge_id : connected_edge_ids) {
      vertex_json += std::to_string(edge_id) + ",";
    }

    vertex_json.pop_back();
  }

  vertex_json +=
      "],\"depth\":" + std::to_string(graph.get_vertex_depth(vertex.id()));

  vertex_json += "}";

  return vertex_json;
}

std::string print_edge_color(const Graph::Edge::Color color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    default:
      return "invalid color";
  }
}

std::string print_edge(const Graph::Edge& edge) {
  std::string edge_json =
      "{\"id\":" + std::to_string(edge.id()) + ",\"vertex_ids\":[";

  edge_json += std::to_string(edge.from_vertex_id()) + "," +
               std::to_string(edge.to_vertex_id());
  edge_json += "],\"color\":\"" + print_edge_color(edge.color()) + "\"";
  edge_json += "}";

  return edge_json;
}

std::string print_graph(const Graph& graph) {
  const auto& vertices = graph.get_vertices();
  const auto& edges = graph.get_edges();

  std::string graph_json =
      "{\n\t\"depth\":" + std::to_string(graph.get_depth()) + ",";

  graph_json += "\n\t\"vertices\": [\n";
  if (vertices.size() != 0) {
    for (const auto& vertex : vertices) {
      graph_json += "\t\t" + print_vertex(vertex, graph) + ",\n";
    }
    graph_json.pop_back();
    graph_json.pop_back();
  }

  graph_json += "\n\t],\n\t\"edges\":[\n";

  if (edges.size() != 0) {
    for (const auto& edge : edges) {
      graph_json += "\t\t" + print_edge(edge) + ",\n";
    }
    graph_json.pop_back();
    graph_json.pop_back();
  }

  graph_json += "\n\t]\n}\n";

  return graph_json;
}
}  // namespace json
}  // namespace printing

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();

  auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(std::move(params));
  const auto graph = generator.generate();

  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
