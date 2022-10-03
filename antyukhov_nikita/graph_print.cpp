#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

constexpr int kVerticesCount = 14;

class Graph {
 public:
  using VertexId = int;
  using EdgeId = int;
  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id){};
    VertexId id() const { return id_; };

   private:
    VertexId id_ = 0;
  };

  struct Edge {
   public:
    explicit Edge(EdgeId id,
                  VertexId first_vertex_id,
                  VertexId second_vertex_id)
        : id_(id),
          first_vertex_id(first_vertex_id),
          second_vertex_id(second_vertex_id){};

    EdgeId id() const { return id_; };
    VertexId get_first_vertex_id() const { return first_vertex_id; };
    VertexId get_second_vertex_id() const { return second_vertex_id; };

   private:
    EdgeId id_ = 0;
    VertexId first_vertex_id = 0;
    VertexId second_vertex_id = 0;
  };
  void add_vertex() {
    const VertexId new_id = get_new_vertex_id();
    vertices_.emplace_back(new_id);

    connections_[new_id] = {};
  };

  void add_edge(VertexId first_vertex_id, VertexId second_vertex_id) {
    EdgeId new_edge_id = get_new_edge_id();
    Edge new_edge1{new_edge_id, first_vertex_id, second_vertex_id};
    edges_.push_back(new_edge1);
    connections_[first_vertex_id].push_back(new_edge_id);
    connections_[second_vertex_id].push_back(new_edge_id);
  };

  std::vector<Edge> get_edges_vector() const { return this->edges_; }

  std::vector<Vertex> get_vertices_vector() const { return vertices_; }

  std::unordered_map<VertexId, std::vector<EdgeId>> get_connectioins_() const {
    return connections_;
  }

 private:
  VertexId get_new_vertex_id() { return last_vertex_id_++; };

  EdgeId get_new_edge_id() { return last_edge_id_++; };

  std::vector<Vertex> vertices_ = {};
  std::vector<Edge> edges_ = {};
  std::unordered_map<VertexId, std::vector<EdgeId>> connections_ = {};
  VertexId last_vertex_id_ = 0;
  EdgeId last_edge_id_ = 0;
};

namespace printing {
namespace json {

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::stringstream json_string;
  std::unordered_map<Graph::VertexId, std::vector<Graph::EdgeId>> set =
      graph.get_connectioins_();
  json_string << "\n\t\t{ \"id\":" << vertex.id() << ", \"edges_ids\": [";
  std::vector<Graph::EdgeId> connections = set[vertex.id()];

  for (Graph::EdgeId y : connections) {
    json_string << y;
    if (y != connections[connections.size() - 1])
      json_string << ", ";
  }

  json_string << "] }";

  return json_string.str();
}

std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  std::stringstream json_string;
  json_string << "\n\t\t{ \"id\": " << edge.id() << ", \"vertex_ids\": ["
              << edge.get_first_vertex_id() << ", "
              << edge.get_second_vertex_id() << "] }";

  return json_string.str();
}

std::string print_graph(const Graph& graph) {
  std::stringstream json_string;
  json_string << "{\n\t\"vertices\": [";
  std::vector<Graph::Vertex> vertices = graph.get_vertices_vector();
  std::vector<Graph::Edge> edges = graph.get_edges_vector();
  for (auto x : vertices) {
    json_string << printing::json::print_vertex(x, graph);
    if (x.id() != vertices[vertices.size() - 1].id())
      json_string << ",";
  }

  json_string << "\n\t],\n\t\"edges\": [";
  for (auto x : edges) {
    json_string << print_edge(x, graph);
    if (x.id() != edges[edges.size() - 1].id())
      json_string << ",";
  }
  json_string << "\n\t]\n}";

  return json_string.str();
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

void write_to_file(std::string s, std::string file) {
  std::ofstream fout;
  fout.open(file);
  fout << s;
  fout.close();
}

int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
