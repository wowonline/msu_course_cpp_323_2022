#include "printing.hpp"
#include <sstream>
#include <string>
#include "graph.hpp"

namespace printing {
namespace json {
std::string print_edge(const Graph::Edge& edge, const Graph& graph) {
  std::ostringstream buf;
  buf << "{\n      \"id\" : " << edge.id() << ",\n      \"vertex_ids\" : [ "
      << edge.from_vertex_id() << ", " << edge.to_vertex_id() << " ]\n    }";
  return buf.str();
}

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::ostringstream buf;
  buf << "{\n      \"id\" : " << vertex.id() << ",\n      \"edge_ids\" : ";
  bool first_edge = true;
  for (const auto& edge : graph.edges_) {
    if (vertex.id() == edge.from_vertex_id() ||
        vertex.id() == edge.to_vertex_id()) {
      if (first_edge) {
        buf << "[ " << edge.id();
        first_edge = false;
      } else {
        buf << ", " << edge.id();
      }
    }
  }
  if (first_edge) {
    buf << "[ ]\n    }";
  } else {
    buf << " ]\n    }";
  }
  return buf.str();
}
std::string print_graph(const Graph& graph) {
  std::ostringstream buf;
  buf << "{\n  \"vertices\" : ";
  bool first_vertex = true;
  for (const auto& vertex : graph.vertexes_) {
    if (first_vertex) {
      buf << "[ \n    " << print_vertex(vertex, graph);
      first_vertex = false;
    } else {
      buf << ", " << print_vertex(vertex, graph);
    }
  }
  if (first_vertex) {
    buf << "[\n  ],";
  } else {
    buf << "\n  ],";
  }

  buf << "\n  \"edges\" : ";
  bool first_edge = true;
  for (const auto& edge : graph.edges_) {
    if (first_edge) {
      buf << "[ \n    " << print_edge(edge, graph);
      first_edge = false;
    } else {
      buf << ", " << print_edge(edge, graph);
    }
  }
  if (first_edge) {
    buf << "[\n  ]\n}";
  } else {
    buf << "\n  ]\n}";
  }
  return buf.str();
}
}  // namespace json
}  // namespace printing
