#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
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
    const auto edge_id = get_new_edge_id();

    edges_.emplace_back(edge_id, from_vertex_id, to_vertex_id);

    adjacency_list_[from_vertex_id].push_back(edge_id);
    if (to_vertex_id != from_vertex_id) {
      adjacency_list_[to_vertex_id].push_back(edge_id);
    }
  }

  const std::vector<EdgeId>& get_connected_edge_ids(VertexId vertex_id) const {
    if (adjacency_list_.find(vertex_id) == adjacency_list_.end()) {
      static const std::vector<EdgeId> empty_result;
      return empty_result;
    }

    return adjacency_list_.at(vertex_id);
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
  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
};

namespace printing {
namespace json {

std::string print_vertex(const Graph::Vertex& vertex, const Graph& graph) {
  std::string vertex_json =
      "{\"id\":" + std::to_string(vertex.id()) + ",\"edge_ids\":[";
  const auto& connected_edge_ids = graph.get_connected_edge_ids(vertex.id());

  if (connected_edge_ids.size() != 0) {
    for (const auto edge_id : connected_edge_ids) {
      vertex_json += std::to_string(edge_id) + ",";
    }

    vertex_json.pop_back();
  }

  vertex_json += "]}";

  return vertex_json;
}

std::string print_edge(const Graph::Edge& edge) {
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
      graph_json += "\t\t" + print_edge(edge) + ",\n";
    }
    graph_json.pop_back();
    graph_json.pop_back();
  }

  graph_json += "\n\t]\n}\n";

  return graph_json;
}
}  // namespace json
}  // namespace printing

void write_to_file(const std::string& graph_json,
                   const std::string& file_name) {
  std::ofstream json_file(file_name);

  json_file << graph_json;

  json_file.close();
}

int handle_depth_input() {
  const std::string init_message = "Type graph depth: ";
  const std::string err_format_message =
      "The depth must be a non-negative integer. Try again";
  const std::string err_fatal_message =
      "Occurred fatal error in handle_depth_input";
  int depth;
  int correct_input = false;

  std::cout << init_message << std::endl;

  while (correct_input == false) {
    if (std::cin >> depth && depth >= 0) {
      correct_input = true;
    } else if (std::cin.fail()) {
      std::cout << err_format_message << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      std::cerr << err_fatal_message << std::endl;
      exit(1);
    }
  }

  return depth;
}

int main() {
  return 0;
}
