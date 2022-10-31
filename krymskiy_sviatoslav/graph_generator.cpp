#include <iostream>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "graph_generator.hpp"
#include "printing.hpp"

using VertexId = int;
using EdgeId = int;

constexpr int kVerticesCount = 14;

Graph::VertexId Graph::get_new_vertex_id() {
  return current_vertex_id_++;
}

Graph::EdgeId Graph::get_new_edge_id() {
  return current_edge_id_++;
}

bool Graph::has_vertex(Graph::VertexId id) const {
  return vertices_.find(id) != vertices_.end();
}

void Graph::add_vertex() {
  const VertexId id = get_new_vertex_id();
  vertices_.emplace(id, id);
}

void Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
  assert(has_vertex(from_vertex_id));
  assert(has_vertex(to_vertex_id));
  const EdgeId id = get_new_edge_id();
  edges_.emplace_back(id, from_vertex_id, to_vertex_id);
}

std::string printing::json::print_graph(const Graph& graph) {
  auto vertices = graph.get_vertices();
  auto edges = graph.get_edges();

  std::string graphString = "{\n\t\"vertices\": [\n";
  bool first = true;

  for (auto& vertex: vertices) {
    if (!first) {
      graphString += ",\n";
    }
    first = false;
    graphString += "\t\t" + print_vertex(vertex.second, graph);
  }

  graphString += "\n\t],\n\t\"edges\": [\n";
  first = true;
  for (auto edge : edges) {
    if (!first) {
      graphString += ",\n";
    }
    first = false;
    graphString += "\t\t" + print_edge(edge, graph);
  }
  graphString += "\n\t]\n}";
  return graphString;
}

std::string printing::json::print_vertex(const Graph::Vertex& vertex,
                                         const Graph& graph) {
  auto graphEdges = graph.get_edges();
  Graph::VertexId vertexId = vertex.id();
  std::string vertexString =
      "{ \"id\": " + std::to_string(vertexId) + ", \"edge_ids\": [";
  bool first = true;
  for (auto edge : graphEdges) {
    if (edge.from_vertex_id() == vertexId || edge.to_vertex_id() == vertexId) {
      if (!first) {
        vertexString += ", ";
      }
      first = false;
      vertexString += std::to_string(edge.id());
      ;
    }
  }
  vertexString += "] }";
  return vertexString;
}

std::string printing::json::print_edge(const Graph::Edge& edge,
                                       const Graph& graph) {
  std::string edgeString = "{ \"id\": " + std::to_string(edge.id()) +
                           ", \"vertex_ids\": [" +
                           std::to_string(edge.to_vertex_id()) + ", " +
                           std::to_string(edge.from_vertex_id()) + "] }";
  return edgeString;
}

const std::unordered_map<Graph::VertexId, Graph::Vertex>& Graph::get_vertices() const {
  return vertices_;
}

const std::vector<Graph::Edge>& Graph::get_edges() const {
  return edges_;
}

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

void write_to_file(const std::string& graph_json, const std::string& fileName) {
  std::ofstream output;
  output.open(fileName);
  output << graph_json;
  output.close();
}

int main() {
  const auto graph = generate_graph();
  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");
  return 0;
}
