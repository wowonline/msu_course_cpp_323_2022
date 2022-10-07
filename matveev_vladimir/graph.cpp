#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

constexpr int kVerticesCount = 14;

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  using Depth = int;

  void add_vertex() { vector_vertices_.emplace_back(generate_vertex_id()); }
  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    assert(!has_edge(from_vertex_id, to_vertex_id));
    vector_edges_.emplace_back(generate_edge_id(), from_vertex_id,
                               to_vertex_id);
  }

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
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id) {}
    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
  };

  Depth get_vertex_depth(VertexId vertex_id) { return -1; }

  // const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
  // set_vertex_depth(to_vertex_id, from_vertex_depth + 1);

  std::vector<Edge> get_edges(VertexId vertex_id) {
    std::vector<Edge> vec = std::vector<Edge>();
    return vec;
  }

  Edge::Color set_edge_color(VertexId from_vertex_id, VertexId to_vertex_id) {
    const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
    const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
    if (from_vertex_id == to_vertex_id) {
      return Edge::Color::Green;
    }
    if (get_edges(to_vertex_id).size() == 0) {
      return Edge::Color::Grey;
    }
    if (to_vertex_depth - from_vertex_depth == 1 &&
        !has_edge(from_vertex_id, to_vertex_id)) {
      return Edge::Color::Yellow;
    }
    if (to_vertex_depth - from_vertex_depth == 2) {
      return Edge::Color::Red;
    }
    throw std::runtime_error("Failed to determine color");
  }

  std::vector<Vertex> get_vector_vertices() const { return vector_vertices_; }

  std::vector<Edge> get_vector_edges() const { return vector_edges_; }

 private:
  std::vector<Vertex> vector_vertices_;
  std::vector<Edge> vector_edges_;

  VertexId num_vertices_ = 0;
  EdgeId num_edges_ = 0;

  VertexId generate_vertex_id() { return num_vertices_++; }
  EdgeId generate_edge_id() { return num_edges_++; }

  bool has_vertex(VertexId id) const {
    for (const auto& vertex : vector_vertices_)
      if (vertex.id() == id)
        return true;
    return false;
  }
  bool has_edge(VertexId id_from, VertexId id_to) const {
    for (const auto& edge : vector_edges_) {
      if (edge.from_vertex_id() == id_from && edge.to_vertex_id() == id_to ||
          edge.from_vertex_id() == id_to && edge.to_vertex_id() == id_from)
        return true;
    }
    return false;
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

  Graph generate() const {
    auto graph = Graph();
    graph.add_vertex();
    /*generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);*/
    return graph;
  }

 private:
  Params params_ = Params(0, 0);
};

namespace printing {
std::string print_edge_color(const Graph::Edge::Color& color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "Grey";
    case Graph::Edge::Color::Green:
      return "Green";
    case Graph::Edge::Color::Yellow:
      return "Yellow";
    case Graph::Edge::Color::Red:
      return "Red";
    default:
      return "";
  }
}
namespace json {
std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::string edges_ids_string = "[";
  for (const auto& edge : graph.get_vector_edges()) {
    if (edge.from_vertex_id() == vertex.id() ||
        edge.to_vertex_id() == vertex.id())
      edges_ids_string += std::to_string(edge.id()) + ',';
  }
  edges_ids_string.erase(edges_ids_string.length() - 1,
                         edges_ids_string.length());
  return "\"id\":" + std::to_string(vertex.id()) +
         ",\"edge_ids\":" + edges_ids_string + "]";
}

std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  return "\"id\":" + std::to_string(edge.id()) + ",\"vertex_ids\":" + "[" +
         std::to_string(edge.from_vertex_id()) + "," +
         std::to_string(edge.to_vertex_id()) + "]";
}

std::string print_graph(const Graph& graph) {
  std::string graph_string = "{\n  \"vertices\": [\n";
  for (const auto& vertex : graph.get_vector_vertices()) {
    graph_string += "    {" + print_vertex(vertex, graph) + "},\n";
  }
  graph_string.erase(graph_string.length() - 2, graph_string.length());
  graph_string += "\n  ],\n  \"edges\": [\n";
  for (const auto& edge : graph.get_vector_edges()) {
    graph_string += "    {" + print_edge(edge, graph) + "},\n";
  }
  graph_string.erase(graph_string.length() - 2, graph_string.length());
  return graph_string + "\n  ]\n}";
}
}  // namespace json
}  // namespace printing

Graph generate_graph() {
  auto graph = Graph();

  for (int i = 0; i < kVerticesCount; i++) {
    graph.add_vertex();
  }

  graph.add_edge(0, 1);
  graph.add_edge(0, 2);
  graph.add_edge(0, 3);
  graph.add_edge(1, 4);
  graph.add_edge(1, 5);
  graph.add_edge(1, 6);
  graph.add_edge(2, 7);
  graph.add_edge(2, 8);
  graph.add_edge(3, 9);
  graph.add_edge(4, 10);
  graph.add_edge(5, 10);
  graph.add_edge(6, 10);
  graph.add_edge(7, 11);
  graph.add_edge(8, 11);
  graph.add_edge(9, 12);
  graph.add_edge(10, 13);
  graph.add_edge(11, 13);
  graph.add_edge(12, 13);

  return graph;
}

void write_to_file(const std::string content, const std::string file_name) {
  std::ofstream new_file(file_name);

  new_file << content;

  new_file.close();
}

int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;

  write_to_file(graph_json, "graph.json");

  return 0;
}
