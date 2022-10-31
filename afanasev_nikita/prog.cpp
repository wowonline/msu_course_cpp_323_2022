#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

   private:
    VertexId id_ = 0;
  };

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

  VertexId add_vertex();

  EdgeId add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    assert(!is_connected(from_vertex_id, to_vertex_id));

    if (get_edges(to_vertex_id).empty()) {
      const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
      set_vertex_depth(to_vertex_id, from_vertex_depth + 1);
    }

    const auto color = determine_color(from_vertex_id, to_vertex_id);

    const auto edge_id = next_edge_id();
    adjacency_list_[from_vertex_id].push_back(edge_id);
    if (from_vertex_id != to_vertex_id) {
      adjacency_list_[to_vertex_id].push_back(edge_id);
    }

    edges_.try_emplace(edge_id, edge_id, from_vertex_id, to_vertex_id, color);
    return edge_id;
  }

  const std::vector<EdgeId>& get_edges(VertexId id) const {
    return adjacency_list_.at(id);
  }

  const std::unordered_map<VertexId, Vertex>& get_vertices() const {
    return vertices_;
  }

  const std::unordered_map<EdgeId, Edge>& get_graph_edges() const {
    return edges_;
  }

  Depth get_depth() const { return current_depth_; }

  Depth get_vertex_depth(VertexId vertex_id) const {
    return depths_.at(vertex_id);
  }

  const std::vector<VertexId>& get_vertices_at_depth(Depth depth) {
    return vertices_at_depth_[depth];
  }

  bool is_connected(VertexId from_vertex_id, VertexId to_vertex_id) const {
    assert(adjacency_list_.find(from_vertex_id) != adjacency_list_.end());
    assert(adjacency_list_.find(to_vertex_id) != adjacency_list_.end());
    if (from_vertex_id != to_vertex_id) {
      const auto& connected_from_edges_ids = adjacency_list_.at(from_vertex_id);
      const auto& connected_to_edges_ids = adjacency_list_.at(to_vertex_id);
      for (auto from_edge_id : connected_from_edges_ids) {
        for (auto to_edge_id : connected_to_edges_ids) {
          if (from_edge_id == to_edge_id) {
            return true;
          }
        }
      }
      return false;
    } else {
      const auto& edges_ids = adjacency_list_.at(from_vertex_id);
      for (auto edge_id : edges_ids) {
        if (edges_.at(edge_id).from_vertex_id() ==
            edges_.at(edge_id).to_vertex_id()) {
          return true;
        }
      }
      return false;
    }
  }

 private:
  VertexId current_vertex_id_ = 0;
  EdgeId current_edge_id_ = 0;
  Depth current_depth_ = 0;

  VertexId next_vertex_id() { return current_vertex_id_++; }

  EdgeId next_edge_id() { return current_edge_id_++; }

  bool has_vertex(VertexId vertex_id) const {
    return vertices_.find(vertex_id) != vertices_.end();
  }

  void set_vertex_depth(VertexId, Depth);

  Edge::Color determine_color(VertexId from_vertex_id,
                              VertexId to_vertex_id) const {
    const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
    const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
    if (from_vertex_id == to_vertex_id) {
      return Edge::Color::Green;
    }
    if (get_edges(to_vertex_id).size() == 0) {
      return Edge::Color::Grey;
    }
    if (to_vertex_depth - from_vertex_depth == 1 &&
        !is_connected(from_vertex_id, to_vertex_id)) {
      return Edge::Color::Yellow;
    }
    if (to_vertex_depth - from_vertex_depth == 2) {
      return Edge::Color::Red;
    }
    throw std::runtime_error("Failed to determine color");
  }

  std::unordered_map<VertexId, Vertex> vertices_;
  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
  std::unordered_map<VertexId, Depth> depths_;
  std::unordered_map<Depth, std::vector<VertexId>> vertices_at_depth_;
};

constexpr Graph::Depth kDefaultDepth = 1;
constexpr float kGreenEdgesProbability = 0.1;
constexpr float kRedEdgesProbability = 0.33;

Graph::VertexId Graph::add_vertex() {
  const auto vertex_id = next_vertex_id();
  vertices_.try_emplace(vertex_id, vertex_id);
  adjacency_list_[vertex_id];
  set_vertex_depth(vertex_id, kDefaultDepth);
  return vertex_id;
}

void Graph::set_vertex_depth(Graph::VertexId vertex_id, Graph::Depth depth) {
  current_depth_ = std::max(current_depth_, depth);
  depths_[vertex_id] = depth;
  vertices_at_depth_[depth].push_back(vertex_id);
  if (depth != kDefaultDepth) {
    for (auto vertex_id_iterator = vertices_at_depth_[kDefaultDepth].cbegin();
         vertex_id_iterator != vertices_at_depth_[kDefaultDepth].cend();
         ++vertex_id_iterator) {
      if (*vertex_id_iterator == vertex_id) {
        vertices_at_depth_[kDefaultDepth].erase(vertex_id_iterator);
        break;
      }
    }
  }
}

std::vector<Graph::VertexId> get_unconnected_vertices_ids(
    Graph& graph,
    Graph::VertexId vertex_id,
    const std::vector<Graph::VertexId>& vertices_ids) {
  std::vector<Graph::VertexId> result;
  for (auto vertex_id_depth_greater : vertices_ids) {
    if (!graph.is_connected(vertex_id, vertex_id_depth_greater)) {
      result.push_back(vertex_id_depth_greater);
    }
  }
  return result;
}

std::vector<Graph::VertexId> get_vertices_ids(
    Graph& graph,
    const std::vector<Graph::VertexId>& vertices_ids) {
  std::vector<Graph::VertexId> result;
  for (auto vertex_id_depth_greater : vertices_ids) {
    result.push_back(vertex_id_depth_greater);
  }
  return result;
}

class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth, int new_vertices_count)
        : depth_(depth), new_vertices_count_(new_vertices_count) {}

    Graph::Depth get_depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(Params&& params) : params_(std::move(params)) {}

  Graph generate() const {
    auto graph = Graph();
    if (params_.get_depth() != 0) {
      graph.add_vertex();
    }
    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
    return graph;
  }

 private:
  void generate_grey_edges(Graph& graph) const {
    for (auto depth = 1; depth < params_.get_depth(); ++depth) {
      float probability =
          (params_.get_depth() - depth) / (params_.get_depth() - 1.f);
      std::random_device device;
      std::mt19937 generator(device());
      std::bernoulli_distribution distribution(probability);
      for (auto vertex_id : graph.get_vertices_at_depth(depth)) {
        for (size_t i = 0; i < params_.new_vertices_count(); ++i) {
          if (distribution(generator)) {
            graph.add_edge(vertex_id, graph.add_vertex());
          }
        }
      }
    }
  }
  void generate_green_edges(Graph& graph) const {
    std::random_device device;
    std::mt19937 generator(device());
    std::bernoulli_distribution distribution(kGreenEdgesProbability);
    for (const auto& [vertex_id, vertex] : graph.get_vertices()) {
      if (distribution(generator)) {
        graph.add_edge(vertex_id, vertex_id);
      }
    }
  }
  void generate_yellow_edges(Graph& graph) const {
    for (auto depth = 1; depth < params_.get_depth(); ++depth) {
      float probability = (depth - 1) / (params_.get_depth() - 2.f);
      std::random_device device_1, device_2;
      std::mt19937 generator_for_bernoulli_distribution(device_1()),
          generator_for_uniform_integer_distribution(device_2());
      std::bernoulli_distribution bernoulli_distribution(probability);
      for (auto vertex_id : graph.get_vertices_at_depth(depth)) {
        if (bernoulli_distribution(generator_for_bernoulli_distribution)) {
          if (!graph.get_vertices_at_depth(depth + 1).empty()) {
            std::vector<Graph::VertexId> unconnected_vertices_ids =
                get_unconnected_vertices_ids(
                    graph, vertex_id, graph.get_vertices_at_depth(depth + 1));
            std::uniform_int_distribution<> uniform_integer_distribution(
                0, unconnected_vertices_ids.size() - 1);
            graph.add_edge(
                vertex_id,
                unconnected_vertices_ids[uniform_integer_distribution(
                    generator_for_uniform_integer_distribution)]);
          }
        }
      }
    }
  }
  void generate_red_edges(Graph& graph) const {
    for (auto depth = 1; depth < params_.get_depth() - 1; ++depth) {
      std::random_device device_1, device_2;
      std::mt19937 generator_for_bernoulli_distribution(device_1()),
          generator_for_uniform_integer_distribution(device_2());
      std::bernoulli_distribution bernoulli_distribution(kRedEdgesProbability);
      for (auto vertex_id : graph.get_vertices_at_depth(depth)) {
        if (bernoulli_distribution(generator_for_bernoulli_distribution)) {
          if (!graph.get_vertices_at_depth(depth + 2).empty()) {
            std::vector<Graph::VertexId> vertices_ids =
                get_vertices_ids(graph, graph.get_vertices_at_depth(depth + 2));
            std::uniform_int_distribution<> uniform_integer_distribution(
                0, vertices_ids.size() - 1);
            graph.add_edge(vertex_id,
                           vertices_ids[uniform_integer_distribution(
                               generator_for_uniform_integer_distribution)]);
          }
        }
      }
    }
  }

  Params params_ = Params(0, 0);
};

namespace printing {
std::string print_edge_color(Graph::Edge::Color color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    case Graph::Edge::Color::Green:
      return "green";
  }
}
namespace json {
std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::string result = "{";

  result += "\"id\":";
  result += std::to_string(vertex.id());

  result += ",";

  result += "\"edge_ids\":[";
  const auto& edge_ids = graph.get_edges(vertex.id());
  for (const auto edge_id : edge_ids) {
    result += std::to_string(edge_id);
    result += ",";
  }
  if (!edge_ids.empty()) {
    result.pop_back();
  }
  result += "]";

  result += ",";

  result += "\"depth\": ";
  result += std::to_string(graph.get_vertex_depth(vertex.id()));

  result += "}";
  return result;
}
std::string print_edge(const Graph::Edge& edge) {
  std::string result = "{";

  result += "\"id\":";
  result += std::to_string(edge.id());

  result += ",";

  result += "\"vertex_ids\":[";
  result += std::to_string(edge.from_vertex_id());
  result += ",";
  result += std::to_string(edge.to_vertex_id());
  result += "]";

  result += ",";

  result += "\"color\": \"";
  result += print_edge_color(edge.color());
  result += "\"";

  result += "}";
  return result;
}
std::string print_graph(const Graph& graph) {
  std::string result = "{";

  result += "\"depth\": ";
  result += std::to_string(graph.get_depth());

  result += ",";

  result += "\"vertices\":[";
  const auto& vertices = graph.get_vertices();
  for (const auto& [vertex_id, vertex] : vertices) {
    result += print_vertex(vertex, graph);
    result += ",";
  }
  if (!vertices.empty()) {
    result.pop_back();
  }

  result += "]";

  result += ",";

  result += "\"edges\":[";
  const auto& edges = graph.get_graph_edges();
  for (const auto& [edge_id, edge] : edges) {
    result += print_edge(edge);
    result += ",";
  }
  if (!edges.empty()) {
    result.pop_back();
  }

  result += "]";

  result += "}\n";
  return result;
}
}  // namespace json
}  // namespace printing

void write_to_file(const std::string& string, const std::string& file_name) {
  std::ofstream file(file_name);
  file << string;
}

int handle_depth_input() {
  std::cout << "Enter depth:" << std::endl;
  std::string input_data;
  int depth;
  while (std::getline(std::cin, input_data)) {
    int flag = 1;
    for (auto symbol : input_data) {
      if (!std::isspace(symbol) && !std::isdigit(symbol) && symbol != '-') {
        std::cout << "Invalid graph depth! (should be integer value [0, "
                     "INT_MAX]). Re-enter the value:"
                  << std::endl;
        flag = 0;
        break;
      }
    }
    if (flag) {
      try {
        depth = std::stoi(input_data);
      } catch (...) {
        std::cout << "Invalid graph depth! (should be integer value [0, "
                     "INT_MAX]). Re-enter the value:"
                  << std::endl;
        continue;
      }
      if (depth < 0) {
        std::cout << "Depth cannot be negative! Re-enter the value:"
                  << std::endl;
      } else {
        break;
      }
    }
  }
  return depth;
}

int handle_new_vertices_count_input() {
  std::cout << "Enter new_vertices_count:" << std::endl;
  std::string input_data;
  int new_vertices_count;
  while (std::getline(std::cin, input_data)) {
    int flag = 1;
    for (auto symbol : input_data) {
      if (!std::isspace(symbol) && !std::isdigit(symbol) && symbol != '-') {
        std::cout << "Invalid new_vertices_count! (should be integer value [0, "
                     "INT_MAX]). Re-enter the value:"
                  << std::endl;
        flag = 0;
        break;
      }
    }
    if (flag) {
      try {
        new_vertices_count = std::stoi(input_data);
      } catch (...) {
        std::cout << "Invalid new_vertices_count! (should be integer value [0, "
                     "INT_MAX]). Re-enter the value:"
                  << std::endl;
        continue;
      }
      if (new_vertices_count < 0) {
        std::cout
            << "new_vertices_count cannot be negative! Re-enter the value:"
            << std::endl;
      } else {
        break;
      }
    }
  }
  return new_vertices_count;
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
