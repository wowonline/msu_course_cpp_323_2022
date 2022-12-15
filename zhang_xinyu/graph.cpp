#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  void add_vertex();
  void add_edge(VertexId, VertexId);
  const std::vector<EdgeId>& connected_edge_ids(VertexId vertexId) const {
    return adjacency_list_.at(vertexId);
  }

  const auto& get_vertexes() const { return vertexes_; }
  const auto& get_edges() const { return edges_; }

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

   private:
    VertexId id_ = 0;
  };

  struct Edge {
   public:
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

 private:
  bool has_vertex(VertexId) const;
  std::vector<Vertex> vertexes_;
  std::vector<Edge> edges_;
  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;
  VertexId get_new_vertex_id() { return last_vertex_id_++; }
  EdgeId get_new_edge_id() { return last_edge_id_++; }
  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
};

namespace printing {
namespace json {

std::string print_graph(const Graph&);
std::string print_vertex(const Graph::Vertex&, const Graph&);
std::string print_edge(const Graph::Edge&);

std::string print_graph(const Graph& graph) {
  std::string str_json = "{\"vertices\":[";
  bool is_first_1 = true;
  for (const auto& vertex : graph.get_vertexes()) {
    if (is_first_1) {
      is_first_1 = false;
    } else {
      str_json += ",";
    }
    str_json += print_vertex(vertex, graph);
  }
  str_json += "],\"edges\":[";
  bool is_first_2 = true;
  for (const auto& edge : graph.get_edges()) {
    if (is_first_2) {
      is_first_2 = false;
    } else {
      str_json += ",";
    }
    str_json += print_edge(edge);
  }
  return str_json += "]}\n";
}

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  Graph::VertexId vertexId = vertex.id();
  std::string str_json = "{\"id\":";
  str_json += std::to_string(vertexId) + ",\"edge_ids\":[";
  bool is_first = true;
  const std::vector<Graph::EdgeId>& edge_to_this_vertex =
      graph.connected_edge_ids(vertexId);
  for (const auto& edgeId : edge_to_this_vertex) {
    if (is_first) {
      is_first = false;
    } else {
      str_json += ",";
    }
    str_json += std::to_string(edgeId);
  }

  return str_json + "]}";
}

std::string print_edge(const Graph::Edge& edge) {
  return "{\"id\":" + std::to_string(edge.id()) + ",\"vertex_ids\":[" +
         std::to_string(edge.from_vertex_id()) + "," +
         std::to_string(edge.to_vertex_id()) + "]}";
}
}  // namespace json
}  // namespace printing

bool Graph::has_vertex(VertexId id) const {
  return std::any_of(vertexes_.begin(), vertexes_.end(),
                     [id](const Vertex& vertex) { return vertex.id() == id; });
}

void Graph::add_vertex() {
  VertexId new_vertex_id = get_new_vertex_id();
  vertexes_.emplace_back(new_vertex_id);
}

void Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  assert(has_vertex(from_vertex_id));
  assert(has_vertex(to_vertex_id));
  const EdgeId new_edge_id = get_new_edge_id();
  edges_.emplace_back(new_edge_id, from_vertex_id, to_vertex_id);

  if (from_vertex_id != to_vertex_id) {
    adjacency_list_[from_vertex_id].emplace_back(new_edge_id);
  }
  adjacency_list_[to_vertex_id].emplace_back(new_edge_id);
}

void write_to_file(const std::string& text, const std::string& filename) {
  std::ofstream fout(filename);
  fout << text;
  fout.close();
}

constexpr int kVerticesCount = 14;

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

int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
