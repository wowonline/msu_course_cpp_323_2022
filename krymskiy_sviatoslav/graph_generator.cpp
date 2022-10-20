#include <iostream>
#include <vector>

class Graph {
    using VertexId = int;
    using EdgeId = int;
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
        Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id) :
                id_(id), from_vertex_id_(from_vertex_id), to_vertex_id_(to_vertex_id) {}

        EdgeId id() const { return id_; }
        VertexId from_vertex_id() const { return from_vertex_id_; }
        VertexId to_vertex_id() const { return to_vertex_id_; }

    private:
        EdgeId id_ = 0;
        VertexId from_vertex_id_ = 0;
        VertexId to_vertex_id_ = 0;
    };
    void add_vertex() {
        verticesAmount++;
    }
    void add_edge(VertexId from_vertex_id, VertexId to_vertex_id) {
        if (from_vertex_id >= verticesAmount || to_vertex_id >= verticesAmount) {
            std::cout << "Wrong vertexes!\n";
            return;
        }
        int graphSize = int(graphEdges.size());
        Edge edge(graphSize, from_vertex_id, to_vertex_id);
        graphEdges.push_back(edge);
    }

private:
    int verticesAmount = 0;
    std::vector<Edge> graphEdges;
};

int main() {
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

    return 0;
}