#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  void add_vertex();
  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id);

  auto get_vertexes_() const { return vertexes_; }
  auto get_edges_() const { return edges_; }

 public:
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
  std::vector<Vertex> vertexes_;
  std::vector<Edge> edges_;

  std::vector<Vertex>::iterator vertex_it;
  std::vector<Edge>::iterator edge_it;

  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;
  VertexId get_new_vertex_id() { return last_vertex_id_++; }
  EdgeId get_new_edge_id() { return last_edge_id_++; }
};

namespace printing {
namespace json {

std::string print_graph(const Graph&);
std::string print_vertex(const Graph::Vertex&, const Graph&);
std::string print_edge(const Graph::Edge&, const Graph&);

std::string print_graph(const Graph& graph) {
  std::string str_json = "{\"vertices\":[";
  bool is_first_1 = true;
  for (auto& vertex : graph.get_vertexes_()) {
    if (is_first_1) {
      is_first_1 = false;
    } else {
      str_json += ",";
    }
    str_json += print_vertex(vertex, graph);
  }
  str_json += "],\"edges\":[";
  bool is_first_2 = true;
  for (auto& edge : graph.get_edges_()) {
    if (is_first_2) {
      is_first_2 = false;
    } else {
      str_json += ",";
    }
    str_json += print_edge(edge, graph);
  }
  return str_json += "]}\n";
}

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::string str_json = "{\"id\":";
  str_json += std::to_string(vertex.id()) + ",\"edge_ids\":[";
  bool is_first = true;
  for (auto& edge : graph.get_edges_()) {
    if (edge.to_vertex_id() == vertex.id() ||
        edge.from_vertex_id() == vertex.id()) {
      if (is_first) {
        is_first = false;
      } else {
        str_json += ",";
      }
      str_json += std::to_string(edge.id());
    }
  }
  return str_json + "]}";
}

std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  return "{\"id\":" + std::to_string(edge.id()) + ",\"vertex_ids\":[" +
         std::to_string(edge.from_vertex_id()) + "," +
         std::to_string(edge.to_vertex_id()) + "]}";
}
}  // namespace json
}  // namespace printing

void Graph::add_vertex() {
  vertexes_.emplace_back(get_new_vertex_id());
}

void Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
}

void write_to_file(const std::string& text, const std::string& filename) {
  std::ofstream fout(filename);
  fout << text;
  fout.close();
}

Graph generate_graph(const int& kVerticesCount = 14) {
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
