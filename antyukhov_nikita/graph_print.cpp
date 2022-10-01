#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

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
    explicit Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id){};

    EdgeId id() const { return id_; };
    VertexId from_vertex_id() const { return from_vertex_id_; };
    VertexId to_vertex_id() const { return to_vertex_id_; };

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
  };
  void add_vertex() {
    VertexId new_id = get_new_vertex_id();
    Vertex new_vertex{new_id};
    vertices_vector_.push_back(new_vertex);

    vertices_edges_map_[new_id] = {};
  };

  void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    Edge new_edge1{get_new_edge_id(), from_vertex_id, to_vertex_id};
    this->edges_vector_.push_back(new_edge1);
    this->vertices_edges_map_[from_vertex_id].push_back(new_edge1);
    this->vertices_edges_map_[to_vertex_id].push_back(new_edge1);
  };

  std::vector<Edge> get_edges_vector() const { return this->edges_vector_; }

  std::vector<Vertex> get_vertices_vector() const {
    return this->vertices_vector_;
  }

  std::unordered_map<VertexId, std::vector<Edge>> get_edges_vertices_map()
      const {
    return this->vertices_edges_map_;
  }

 private:
  VertexId get_new_vertex_id() { return vertices_amount_++; };

  EdgeId get_new_edge_id() { return edges_amount_++; };

  std::vector<Vertex> vertices_vector_ = {};
  std::vector<Edge> edges_vector_ = {};
  std::unordered_map<VertexId, std::vector<Edge>> vertices_edges_map_ = {};
  VertexId vertices_amount_ = 0;
  EdgeId edges_amount_ = 0;
};

namespace printing {
namespace json {

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::stringstream json_string;
  std::unordered_map<Graph::VertexId, std::vector<Graph::Edge>> set =
      graph.get_edges_vertices_map();
  json_string << "\n\t\t{ \"id\":" << vertex.id() << ", \"edges_ids\": [";
  std::vector<Graph::Edge> edges_vertices_vector = set[vertex.id()];

  for (auto y : edges_vertices_vector) {
    json_string << y.id();
    if (y.id() != edges_vertices_vector[edges_vertices_vector.size() - 1].id())
      json_string << ", ";
  }

  json_string << "] }";

  return json_string.str();
}

std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  std::stringstream json_string;
  json_string << "\n\t\t{ \"id\": " << edge.id() << ", \"vertex_ids\": ["
              << edge.from_vertex_id() << ", " << edge.to_vertex_id() << "] }";

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
  const int kVerticesCount = 14;
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
