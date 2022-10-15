#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

constexpr int kVerticesCount = 14;

class Graph {
public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  struct Vertex {
  public:
    Vertex() {}
    explicit Vertex(VertexId id, Depth depth) : id_(id), depth_(depth) {}
    VertexId id() const { return id_; }
    Depth depth() const { return depth_; }

  private:
    VertexId id_ = 0;
    Depth depth_;
  };

  struct Edge {
  public:
    enum class Color { Grey, Green, Yellow, Red };
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id, Color color)
        : id_(id), from_vertex_id_(from_vertex_id), to_vertex_id_(to_vertex_id),
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

  void add_vertex(Depth depth) {
    VertexId new_vertex_id = generate_vertex_id();
    adjacency_list_[new_vertex_id] = {};
    map_vertices_.emplace(new_vertex_id, Vertex(new_vertex_id, depth));
  }
  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    Edge::Color color;
    switch (map_vertices_[to_vertex_id].depth() -
            map_vertices_[from_vertex_id].depth()) {
    case 2:
      color = Edge::Color::Red;
      break;
    case 1:
      if (get_connected_edge_ids(to_vertex_id).empty())
        color = Edge::Color::Grey;
      else
        color = Edge::Color::Yellow;
      break;
    case 0:
      color = Edge::Color::Green;
      break;
    }
    EdgeId new_edge_id = generate_edge_id();
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    assert(!has_edge(from_vertex_id, to_vertex_id));
    vector_edges_.emplace_back(new_edge_id, from_vertex_id, to_vertex_id,
                               color);
    adjacency_list_[from_vertex_id].emplace_back(new_edge_id);
    adjacency_list_[to_vertex_id].emplace_back(new_edge_id);
  }

  const std::unordered_map<VertexId, Vertex> &get_map_vertices() const {
    return map_vertices_;
  }

  const std::vector<Edge> &get_vector_edges() const { return vector_edges_; }

  const std::vector<EdgeId> &get_connected_edge_ids(VertexId vertex_id) const {
    return adjacency_list_.at(vertex_id);
  }

  const std::vector<VertexId> &get_vertex_ids_on_depth(Depth depth) const {
    return vertices_on_depth_.at(depth);
  }

  void add_vertex_id_on_depth(Depth depth, VertexId vertex_id) {
    vertices_on_depth_[depth].emplace_back(vertex_id);
  }

  VertexId get_last_vertex_id() const { return last_vertex_id_; }

  bool has_vertex(VertexId id) const {
    if (map_vertices_.find(id) == map_vertices_.end())
      return false;
    return true;
  }

  bool has_edge(VertexId id_from, VertexId id_to) const {
    for (const auto &edge : vector_edges_) {
      if (edge.from_vertex_id() == id_from && edge.to_vertex_id() == id_to ||
          edge.from_vertex_id() == id_to && edge.to_vertex_id() == id_from)
        return true;
    }
    return false;
  }

private:
  std::vector<Edge> vector_edges_;
  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
  std::unordered_map<Depth, std::vector<VertexId>> vertices_on_depth_;
  std::unordered_map<VertexId, Vertex> map_vertices_;

  VertexId num_vertices_ = 0;
  VertexId last_vertex_id_ = -1;
  EdgeId num_edges_ = 0;

  VertexId generate_vertex_id() {
    last_vertex_id_++;
    return num_vertices_++;
  }
  EdgeId generate_edge_id() { return num_edges_++; }
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

  explicit GraphGenerator(Params &&params) : params_(std::move(params)) {}
  void generate_grey_edges(Graph &graph) const;
  void generate_green_edges(Graph &graph) const;
  void generate_yellow_edges(Graph &graph) const;
  void generate_red_edges(Graph &graph) const;
  Graph generate() const {
    auto graph = Graph();
    graph.add_vertex(1);
    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
    return graph;
  }

private:
  Params params_ = Params(0, 0);
};

void GraphGenerator::generate_grey_edges(Graph &graph) const {
  const Graph::Depth depth = params_.depth();
  const int new_vertices_count = params_.new_vertices_count();
  float step = 1.0 / (depth - 1);
  graph.add_vertex_id_on_depth(1, 0);
  for (Graph::Depth i = 0; i < depth; ++i)
    for (const auto &vertex_id : graph.get_vertex_ids_on_depth(i + 1))
      for (int j = 0; j < new_vertices_count; ++j) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution d(1 - i * step);
        if (d(gen)) {
          graph.add_vertex(i + 2);
          graph.add_edge(vertex_id, graph.get_last_vertex_id());
          graph.add_vertex_id_on_depth(i + 2, graph.get_last_vertex_id());
        }
      }
}

void GraphGenerator::generate_green_edges(Graph &graph) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(0.1);
  for (auto &id_vertex_pair : graph.get_map_vertices())
    if (d(gen))
      graph.add_edge(id_vertex_pair.second.id(), id_vertex_pair.second.id());
}

void GraphGenerator::generate_yellow_edges(Graph &graph) const {
  const Graph::Depth depth = params_.depth();
  float step = 1.0 / (depth - 2);
  for (Graph::Depth i = 2; i < depth; ++i) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(step * (i - 1));
    const auto &vertex_ids_on_current_depth = graph.get_vertex_ids_on_depth(i);
    const auto &vertex_ids_on_following_depth =
        graph.get_vertex_ids_on_depth(i + 1);
    std::srand(std::time(0));
    for (const auto &from_vertex_id : vertex_ids_on_current_depth) {
      std::vector<Graph::VertexId> not_connected_vertex_ids =
          vertex_ids_on_following_depth;
      for (const Graph::EdgeId edge_id :
           graph.get_connected_edge_ids(from_vertex_id)) {
        Graph::VertexId connected_vertex_id;
        if (from_vertex_id ==
            graph.get_vector_edges()[edge_id].from_vertex_id())
          connected_vertex_id =
              graph.get_vector_edges()[edge_id].to_vertex_id();
        else
          connected_vertex_id =
              graph.get_vector_edges()[edge_id].from_vertex_id();
        auto iterator = not_connected_vertex_ids.begin();
        while (iterator != not_connected_vertex_ids.end()) {
          if (*iterator == connected_vertex_id) {
            not_connected_vertex_ids.erase(iterator);
            break;
          } else
            ++iterator;
        }
      }

      for (const Graph::EdgeId vertex_id : not_connected_vertex_ids) {
        auto new_to_vertex_number =
            std::rand() % not_connected_vertex_ids.size();
        auto to_vertex_id = not_connected_vertex_ids[new_to_vertex_number];
        if (d(gen))
          graph.add_edge(from_vertex_id, to_vertex_id);
        break;
      }
    }
  }
}

void GraphGenerator::generate_red_edges(Graph &graph) const {
  const Graph::Depth depth = params_.depth();
  static float constexpr probability = 1.0 / 3;
  for (Graph::Depth i = 1; i < depth - 1; ++i) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(probability);
    const auto &vertex_ids_on_current_depth = graph.get_vertex_ids_on_depth(i);
    const auto &vertex_ids_on_following_depth =
        graph.get_vertex_ids_on_depth(i + 2);

    for (const auto &from_vertex_id : vertex_ids_on_current_depth) {
      auto new_to_vertex_number =
          std::rand() % vertex_ids_on_following_depth.size();
      auto to_vertex_id = vertex_ids_on_following_depth[new_to_vertex_number];
      if (d(gen)) {
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
std::string print_vertex(const Graph::Vertex &vertex, const Graph &graph) {
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

std::string print_edge(const Graph::Edge &edge) {
  return "\"id\":" + std::to_string(edge.id()) + ",\"vertex_ids\":" + "[" +
         std::to_string(edge.from_vertex_id()) + "," +
         std::to_string(edge.to_vertex_id()) + "],\"color\":\"" +
         printing::color_to_string(edge.color()) + "\"";
}

std::string print_graph(const Graph &graph) {
  std::string graph_string = "{\n  \"vertices\": [\n";
  for (auto &id_vertex_pair : graph.get_map_vertices()) {
    graph_string +=
        "    {" + print_vertex(id_vertex_pair.second, graph) + "},\n";
  }
  graph_string.erase(graph_string.length() - 2, graph_string.length());
  graph_string += "\n  ],\n  \"edges\": [\n";
  for (const auto &edge : graph.get_vector_edges()) {
    graph_string += "    {" + print_edge(edge) + "},\n";
  }
  graph_string.erase(graph_string.length() - 2, graph_string.length());
  return graph_string + "\n  ]\n}\n";
}
} // namespace json
} // namespace printing

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

void write_to_file(const std::string &content, const std::string &file_name) {
  std::ofstream new_file(file_name);

  new_file << content;

  new_file.close();
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
