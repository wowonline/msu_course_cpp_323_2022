#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "i_vertex.hpp"
#include "i_edge.hpp"

namespace uni_course_cpp {
using Depth = int;
class IGraph {
public:
    virtual ~IGraph() {};

    virtual VertexId add_vertex() = 0;
    virtual EdgeId add_edge(VertexId from_vertex_id, VertexId to_vertex_id) = 0;
    virtual has_vertex(VertexId vertex_id) const = 0;
    virtual const std::vector<Vertex>& get_vertices() const = 0;
    virtual const std::vector<Edge>& get_edges() const = 0;
    virtual const std::vector<EdgeId>& connected_edges_ids(VertexId vertex_id) const = 0;
    virtual void set_vertex_depth(VertexId vertex_id, Depth depth) = 0;
    virtual bool is_connected(VertexId from_vertex_id, VertexId to_vertex_id) const = 0;
    virtual Edge::Color get_edge_color(VertexId from_vertex_id,VertexId to_vertex_id) const = 0;
    virtual Depth vertex_depth(VertexId vertex_id) const = 0;
    virtual const Vertex& get_vertex_with_id(VertexId vertex_id) const = 0;
    virtual const std::vector<VertexId>& get_vertices_with_depth(Depth depth) const = 0;
    virtual const std::vector<Graph::EdgeId>& get_edges_with_color(Edge::Color color) const;
    virtual Depth depth() const = 0;

};
}