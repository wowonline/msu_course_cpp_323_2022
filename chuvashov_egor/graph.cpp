#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

constexpr int kVerticesCount = 14;

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;

  struct Edge {
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id) {}

    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }

   private:
    const EdgeId id_ = 0;
    const EdgeId from_vertex_id_ = 0;
    const EdgeId to_vertex_id_ = 0;
  };

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

   private:
    const VertexId id_ = 0;
  };

  void add_vertex() { vertices_.emplace_back(get_new_vertex_id()); }

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    assert(has_vertex(from_vertex_id));
    assert(has_vertex(to_vertex_id));
    int new_id = get_new_edge_id();
    edges_.emplace_back(new_id, from_vertex_id, to_vertex_id);
    connections_[from_vertex_id].push_back(new_id);
    connections_[to_vertex_id].push_back(new_id);
  }

  bool has_vertex(VertexId id) const {
    for (Vertex vertex : vertices_) {
      if (vertex.id() == id)
        return true;
    }
    return false;
  }

  std::vector<Vertex> get_vertex_vector() const { return vertices_; }
  std::vector<Edge> get_edges_vector() const { return edges_; }

  std::string get_vertex_edges_str(VertexId id) const {
    std::stringstream s;
    std::vector<EdgeId> edges = connections_.at(id);
    s << "[";
    for (std::vector<EdgeId>::iterator edge_it = edges.begin(); edge_it != edges.end(); ++edge_it) {
      s << *edge_it;
      if (edge_it != (--edges.end())) {
        s << ", ";
      }
    }
    s << "]";
    return s.str();
  }

  VertexId get_new_vertex_id() { return new_vertex_id_++; }
  EdgeId get_new_edge_id() { return new_edge_id_++; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::unordered_map<VertexId, std::vector<EdgeId>> connections_;

  VertexId new_vertex_id_ = 0;
  EdgeId new_edge_id_ = 0;
};

namespace printing {
namespace json {

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::stringstream json;
  json << "\n\t\t\t\"id\": " << vertex.id()
       << ",\n\t\t\t\"edge_ids\": " << graph.get_vertex_edges_str(vertex.id());
  return json.str();
}

std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  std::stringstream json;
  json << "\n\t\t\t\"id\": " << edge.id() << ",\n\t\t\t\"vertex_ids\": ["
       << edge.from_vertex_id() << ", " << edge.to_vertex_id() << "]\n";
  return json.str();
}

std::string print_graph(const Graph& graph) {
  std::stringstream json;
  json << "{\n\t\"vertices\": [\n\t\t";

  const std::vector<Graph::Vertex> vertex_vector = graph.get_vertex_vector();
  for (auto vertex_it = vertex_vector.begin(); vertex_it != vertex_vector.end();
       ++vertex_it) {
    json << "{" << print_vertex((*vertex_it), graph) << "\n\t\t}";
    if (vertex_it != (--vertex_vector.end())) {
      json << ", ";
    }
  }
  json << "\n\t], \n\t\"edges\": [\n\t\t";
  const std::vector<Graph::Edge> edge_vector = graph.get_edges_vector();
  for (auto edge_it = edge_vector.begin(); edge_it != edge_vector.end();
       ++edge_it) {
    json << "{" << print_edge((*edge_it), graph) << "\n\t\t}";
    if (edge_it != (--edge_vector.end())) {
      json << ", ";
    }
  }
  json << "\n\t]\n}";
  return json.str();
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

void write_to_file(const std::string& graph_json, const std::string& filename) {
  std::ofstream file;
  file.open(filename);
  file << graph_json;
  file.close();
}


int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
