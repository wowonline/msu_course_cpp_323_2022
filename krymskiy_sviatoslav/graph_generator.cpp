#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>

#include "graph_generator.hpp"
#include "printing.hpp"

using VertexId = int;
using EdgeId = int;

struct Graph::Vertex {
public:
    Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

private:
    VertexId id_ = 0;
};

struct Graph::Edge {
public:
    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id)
            : id_(id), from_vertex_id_(from_vertex_id), to_vertex_id_(to_vertex_id) {}

    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }

private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
};

std::string printing::json::print_graph(const Graph& graph) {
    auto graphVertices = graph.get_vertices();
    auto graphEdges = graph.get_edges();

    std::string graphString = "{\n\t\"vertices\": [\n";
    bool first = true;
    for (auto vertex : graphVertices) {
        if (!first) {
            graphString += ",\n";
        }
        first = false;
        graphString += "\t\t" + print_vertex(vertex, graph);
    }

    graphString += "\n\t],\n\t\"edges\": [\n";
    first = true;
    for (auto edge : graphEdges) {
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

void Graph::add_vertex() {
    int verticesAmount = int(graphVertices.size());
    Vertex vertex(verticesAmount);
    graphVertices.push_back(vertex);
}

const std::vector<Graph::Vertex>& Graph::get_vertices() const {
    return graphVertices;
}

const std::vector<Graph::Edge>& Graph::get_edges() const {
    return graphEdges;
}

void Graph::add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
    int verticesAmount = int(graphVertices.size());
    if (from_vertex_id >= verticesAmount || to_vertex_id >= verticesAmount) {
        std::cout << "Wrong vertexes!\n";
        return;
    }
    int graphSize = int(graphEdges.size());
    Edge edge(graphSize, from_vertex_id, to_vertex_id);
    graphEdges.push_back(edge);
}

Graph generate_graph() {
    auto graph = Graph();
    int kVerticesCount = 14;
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