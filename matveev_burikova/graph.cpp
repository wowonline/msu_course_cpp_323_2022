#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_map>

constexpr int kVerticesCount = 14;

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }
    Depth depth() const { return depth_; }
    friend class Graph;

   private:
    VertexId id_;
    Depth depth_ = 1;
    void set_depth(Depth depth) { depth_ = depth; }
  };

  struct Edge {
   public:
    enum class Color { Grey, Green, Yellow, Red };
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id) {}
    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }
    Color color() const { return color_; }
    friend class Graph;

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
    Color color_ = Color::Grey;
    void set_color(Color color) { color_ = color; }
  };

  void add_vertex() {
    const VertexId new_vertex_id = generate_vertex_id();
    adjacency_list_[new_vertex_id] = {};
    vertices_.emplace(new_vertex_id, Vertex(new_vertex_id));
    add_vertex_id_on_depth(1, new_vertex_id);
  }

  Edge::Color get_new_edge_color(VertexId from_vertex_id,
                                 VertexId to_vertex_id) {
    if (from_vertex_id == to_vertex_id) {
      return Edge::Color::Green;
    }
    if (get_connected_edge_ids(to_vertex_id).size() == 0) {
      return Edge::Color::Grey;
    }
    if (get_vertex_depth(to_vertex_id) - get_vertex_depth(from_vertex_id) ==
            1 &&
        !has_edge(from_vertex_id, to_vertex_id)) {
      return Edge::Color::Yellow;
    }
    if (get_vertex_depth(to_vertex_id) - get_vertex_depth(from_vertex_id) ==
        2) {
      return Edge::Color::Red;
    }
    throw std::runtime_error("Failed to determine color");
  }

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    assert(!has_edge(from_vertex_id, to_vertex_id));
    const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
    if (get_connected_edge_ids(to_vertex_id).empty()) {
      const auto new_to_vertex_depth = from_vertex_depth + 1;
      remove_vertex_id_from_depth(get_vertex_depth(to_vertex_id), to_vertex_id);
      set_vertex_depth(to_vertex_id, new_to_vertex_depth);
      add_vertex_id_on_depth(new_to_vertex_depth, to_vertex_id);
    }
    const EdgeId new_edge_id = generate_edge_id();
    Edge::Color color = get_new_edge_color(from_vertex_id, to_vertex_id);
    edges_.emplace(new_edge_id,
                   Edge(new_edge_id, from_vertex_id, to_vertex_id));
    edges_.at(get_last_edge_id()).set_color(color);
    adjacency_list_[from_vertex_id].emplace_back(new_edge_id);
    if (from_vertex_id != to_vertex_id) {
      adjacency_list_[to_vertex_id].emplace_back(new_edge_id);
    }
  }

  Depth depth() const { return vertices_on_depth_.size(); }

  Depth get_vertex_depth(VertexId vertex_id) {
    return vertices_.at(vertex_id).depth();
  }

  const std::unordered_map<VertexId, Vertex>& get_vertices() const {
    return vertices_;
  }

  const std::unordered_map<EdgeId, Edge>& get_edges() const { return edges_; }

  const std::vector<EdgeId>& get_connected_edge_ids(VertexId vertex_id) const {
    return adjacency_list_.at(vertex_id);
  }

  const std::vector<VertexId>& get_vertex_ids_on_depth(Depth depth) const {
    return vertices_on_depth_.at(depth);
  }

  bool has_vertex(VertexId vertex_id) const {
    return vertices_.find(vertex_id) != vertices_.end();
  }
  bool has_edge(VertexId from_vertex_id, VertexId to_vertex_id) const {
    for (const auto& [edge_id, edge] : edges_) {
      if ((edge.from_vertex_id() == from_vertex_id &&
           edge.to_vertex_id() == to_vertex_id) ||
          (edge.from_vertex_id() == to_vertex_id &&
           edge.to_vertex_id() == from_vertex_id))
        return true;
    }
    return false;
  }
  VertexId get_last_vertex_id() const { return last_vertex_id_ - 1; }

 private:
  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
  std::unordered_map<Depth, std::vector<VertexId>> vertices_on_depth_;

  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;

  EdgeId get_last_edge_id() const { return last_edge_id_ - 1; }

  VertexId generate_vertex_id() { return last_vertex_id_++; }
  EdgeId generate_edge_id() { return last_edge_id_++; }

  void add_vertex_id_on_depth(Depth depth, VertexId vertex_id) {
    vertices_on_depth_[depth].emplace_back(vertex_id);
  }

  void set_vertex_depth(VertexId vertex_id, Depth new_depth) {
    vertices_.at(vertex_id).set_depth(new_depth);
  }

  void set_edge_color(EdgeId edge_id, Edge::Color new_color) {
    edges_.at(edge_id).set_color(new_color);
  }

  void remove_vertex_id_from_depth(Depth depth, VertexId vertex_id) {
    auto iterator = find(vertices_on_depth_[depth].begin(),
                         vertices_on_depth_[depth].end(), vertex_id);
    assert(iterator != vertices_on_depth_[depth].end());
    vertices_on_depth_[depth].erase(iterator);
  }
};

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
  void generate_grey_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;
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
  Params params_ = Params(0, 0);
};
const std::vector<Graph::VertexId> get_not_connected_vertex_ids(
    Graph graph,
    Graph::VertexId vertex_id,
    Graph::Depth with_depth) {
  std::vector<Graph::VertexId> not_connected_vertex_ids =
      graph.get_vertex_ids_on_depth(with_depth);
  for (const Graph::EdgeId edge_id : graph.get_connected_edge_ids(vertex_id)) {
    Graph::VertexId connected_vertex_id;
    if (vertex_id == graph.get_edges().at(edge_id).from_vertex_id())
      connected_vertex_id = graph.get_edges().at(edge_id).to_vertex_id();
    else
      connected_vertex_id = graph.get_edges().at(edge_id).from_vertex_id();
    auto iterator = not_connected_vertex_ids.begin();
    while (iterator != not_connected_vertex_ids.end()) {
      if (*iterator == connected_vertex_id) {
        not_connected_vertex_ids.erase(iterator);
        break;
      } else
        ++iterator;
    }
  }
  return not_connected_vertex_ids;
}

const bool check_probability(const double probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(probability);
  return d(gen);
}

const int get_random_number(const int minimum, const int maximum) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(minimum, maximum);
  return distrib(gen);
}

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  const Graph::Depth depth = params_.depth();
  const int new_vertices_count = params_.new_vertices_count();
  float step = 1.0 / (depth - 1);
  for (Graph::Depth i = 0; i < depth; ++i)
    for (const auto vertex_id : graph.get_vertex_ids_on_depth(i + 1))
      for (int j = 0; j < new_vertices_count; ++j) {
        if (check_probability(1 - i * step)) {
          graph.add_vertex();
          graph.add_edge(vertex_id, graph.get_last_vertex_id());
        }
      }
}

void GraphGenerator::generate_green_edges(Graph& graph) const {
  for (const auto& [vertex_id, vertex] : graph.get_vertices())
    if (check_probability(0.1))
      graph.add_edge(vertex.id(), vertex.id());
}

void GraphGenerator::generate_yellow_edges(Graph& graph) const {
  const Graph::Depth depth = params_.depth();
  float step = 1.0 / (depth - 2);
  for (Graph::Depth i = 2; i < depth; ++i) {
    const auto& vertex_ids_on_current_depth = graph.get_vertex_ids_on_depth(i);
    for (const auto& from_vertex_id : vertex_ids_on_current_depth) {
      const std::vector<Graph::VertexId> not_connected_vertex_ids =
          get_not_connected_vertex_ids(graph, from_vertex_id, i + 1);

      for (const Graph::EdgeId vertex_id : not_connected_vertex_ids) {
        auto new_to_vertex_number =
            get_random_number(0, not_connected_vertex_ids.size() - 1);
        auto to_vertex_id = not_connected_vertex_ids[new_to_vertex_number];
        if (check_probability(step * (i - 1)))
          graph.add_edge(from_vertex_id, to_vertex_id);
        break;
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph& graph) const {
  const Graph::Depth depth = params_.depth();
  static float constexpr probability = 1.0 / 3;
  for (Graph::Depth i = 1; i < depth - 1; ++i) {
    const auto& vertex_ids_on_current_depth = graph.get_vertex_ids_on_depth(i);
    const auto& vertex_ids_on_following_depth =
        graph.get_vertex_ids_on_depth(i + 2);

    for (const auto& from_vertex_id : vertex_ids_on_current_depth) {
      auto new_to_vertex_number =
          get_random_number(0, vertex_ids_on_following_depth.size() - 1);
      auto to_vertex_id = vertex_ids_on_following_depth[new_to_vertex_number];
      if (check_probability(probability)) {
        graph.add_edge(from_vertex_id, to_vertex_id);
      }
    }
  }
}

namespace printing {
std::string color_to_string(Graph::Edge::Color color) {
  switch (color) {
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    default:
      return "grey";
  }
}
namespace json {
std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::string edges_ids_string = "[";
  for (const Graph::EdgeId edge_id :
       graph.get_connected_edge_ids(vertex.id())) {
    edges_ids_string += std::to_string(edge_id) + ',';
  }
  edges_ids_string.erase(edges_ids_string.length() - 1,
                         edges_ids_string.length());
  return "\"id\":" + std::to_string(vertex.id()) +
         ",\"edge_ids\":" + edges_ids_string + "]" +
         ",\"depth\":" + std::to_string(vertex.depth());
}

std::string print_edge(const Graph::Edge& edge) {
  return "\"id\":" + std::to_string(edge.id()) + ",\"vertex_ids\":" + "[" +
         std::to_string(edge.from_vertex_id()) + "," +
         std::to_string(edge.to_vertex_id()) + "],\"color\":\"" +
         printing::color_to_string(edge.color()) + "\"";
}

std::string print_graph(const Graph& graph) {
  std::string graph_string =
      "{\n  \"depth\": " + std::to_string(graph.depth()) +
      ",\n  \"vertices\": [\n";
  for (const auto& [vertex_id, vertex] : graph.get_vertices()) {
    graph_string += "    {" + print_vertex(vertex, graph) + "},\n";
  }
  graph_string.erase(graph_string.length() - 2, graph_string.length());
  graph_string += "\n  ],\n  \"edges\": [\n";
  for (const auto& [edge_id, edge] : graph.get_edges()) {
    graph_string += "    {" + print_edge(edge) + "},\n";
  }
  graph_string.erase(graph_string.length() - 2, graph_string.length());
  return graph_string + "\n  ]\n}\n";
}
}  // namespace json
}  // namespace printing

const int handle_depth_input() {
  int depth_tmp;
  while (true) {
    std::cout << "Enter depth: ";
    std::cin >> depth_tmp;
    std::cout << std::endl;
    if (depth_tmp < 0)
      std::cout << "Incorrect data. Enter depth one more time: ";
    else
      break;
  }
  return depth_tmp;
}

void write_to_file(const std::string& content, const std::string& file_name) {
  std::ofstream new_file(file_name);

  new_file << content;
}

const int handle_new_vertices_count_input() {
  int new_vertices_count_tmp;
  while (true) {
    std::cout << "Enter new vertices count: ";
    std::cin >> new_vertices_count_tmp;
    std::cout << std::endl;
    if (new_vertices_count_tmp < 0)
      std::cout << "Incorrect data. Enter new vertices count one more time: ";
    else
      break;
  }
  return new_vertices_count_tmp;
}

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
