#include <fstream>
#include <iostream>
#include <vector>

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  void add_vertex() { vertices_.emplace_back(Vertex(get_new_vertex_id())); };

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    edges_.emplace_back(Edge(get_new_edge_id(), from_vertex_id, to_vertex_id));
  };

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; };

   private:
    VertexId id_ = 0;
  };
  struct Edge {
   public:
    explicit Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id) {}
    EdgeId id() { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; };
    VertexId to_vertex_id() const { return to_vertex_id_; };

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
  };
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;

  VertexId get_new_vertex_id() { return last_vertex_id_++; }
  EdgeId get_new_edge_id() { return last_edge_id_++; }
};

namespace printing {
namespace json {

std::string print_graph(const Graph& graph);
std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph);
std::string print_edge(const Graph::Edge& edge, const Graph& graph);

}  // namespace json
}  // namespace printing

Graph generate_graph() {
  auto graph = Graph();
  constexpr int kVerticesCount = 14;

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

void write_to_file(const std::string& data, const std::string& file_path) {
  std::ofstream out;
  out.open(file_path, std::ios::out | std::ios::trunc);
  out << data;
  out.close();
}

int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
