#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int kVerticesCount = 14;

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;

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

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

   private:
    VertexId id_ = 0;
  };

  void add_vertex() { vertices_.emplace_back(get_new_vertex_id()); }

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
  }

  std::vector<EdgeId> get_vertex_edges_ids(const Graph::Vertex& vertex) const {
    std::vector<EdgeId> edge_ids;

    for (const auto& edge : edges_) {
      if (edge.from_vertex_id() == vertex.id() ||
          edge.to_vertex_id() == vertex.id()) {
        edge_ids.push_back(edge.id());
      }
    }

    return edge_ids;
  }

  const std::vector<Vertex>& get_vertices() const { return vertices_; }

  const std::vector<Edge>& get_edges() const { return edges_; }

 private:
  VertexId get_new_vertex_id() { return next_free_vertex_id_++; }

  EdgeId get_new_edge_id() { return next_free_edge_id_++; }

  VertexId next_free_vertex_id_ = 0;
  EdgeId next_free_edge_id_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};

namespace printing {
namespace json {

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::string vertex_json =
      "{\"id\":" + std::to_string(vertex.id()) + ",\"edge_ids\":[";
  const auto vertex_edge_ids = graph.get_vertex_edges_ids(vertex);

  for (const auto& edge_id : vertex_edge_ids) {
    vertex_json += std::to_string(edge_id) + ",";
  }

  vertex_json.pop_back();
  vertex_json += "]}";

  return vertex_json;
}

std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  std::string edge_json =
      "{\"id\":" + std::to_string(edge.id()) + ",\"vertex_ids\":[";

  edge_json += std::to_string(edge.from_vertex_id()) + "," +
               std::to_string(edge.to_vertex_id());
  edge_json += "]}";

  return edge_json;
}

std::string print_graph(const Graph& graph) {
  const auto& vertices = graph.get_vertices();
  const auto& edges = graph.get_edges();

  std::string graph_json = "{\n\t\"vertices\": [\n";

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
      graph_json += "\t\t" + print_edge(edge, graph) + ",\n";
    }
    graph_json.pop_back();
    graph_json.pop_back();
  }

  graph_json += "\n\t]\n}\n";

  return graph_json;
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

void write_to_file(const std::string& graph_json,
                   const std::string& file_name) {
  std::ofstream json_file(file_name);

  json_file << graph_json;

  json_file.close();
}

int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
