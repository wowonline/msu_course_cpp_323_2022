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
  using Depth = int;

  struct Edge {
   public:
    enum class Color { Grey, Green, Yellow, Red };

    Edge(EdgeId id, VertexId from_vertex_id, VertexId to_vertex_id, Color color)
        : id_(id),
          from_vertex_id_(from_vertex_id),
          to_vertex_id_(to_vertex_id),
          color_(color) {}

    EdgeId id() const { return id_; }
    VertexId from_vertex_id() const { return from_vertex_id_; }
    VertexId to_vertex_id() const { return to_vertex_id_; }

   private:
    EdgeId id_ = 0;
    VertexId from_vertex_id_ = 0;
    VertexId to_vertex_id_ = 0;
    Color color_ = Color::Grey;
  };

  struct Vertex {
   public:
    explicit Vertex(VertexId id) : id_(id) {}
    VertexId id() const { return id_; }

   private:
    VertexId id_ = 0;
  };

  void add_vertex() {
    const VertexId new_vertex_id = get_new_vertex_id();

    vertices_.emplace_back(new_vertex_id);
    set_vertex_depth(new_vertex_id, 1);
  }

  void add_edge(const VertexId from_vertex_id, const VertexId to_vertex_id) {
    const auto edge_id = get_new_edge_id();
    const auto edge_color = determine_edge_color(from_vertex_id, to_vertex_id);

    edges_.emplace_back(edge_id, from_vertex_id, to_vertex_id, edge_color);

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

  Edge::Color determine_edge_color(const VertexId from_vertex_id,
                                   const VertexId to_vertex_id) const {
    const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
    const auto to_vertex_depth = get_vertex_depth(to_vertex_id);

    if (from_vertex_id == to_vertex_id) {
      return Edge::Color::Green;
    }
    if (get_connected_edge_ids(to_vertex_id).size() == 0) {
      return Edge::Color::Grey;
    }
    if (to_vertex_depth - from_vertex_depth == 1 &&
        !is_connected(from_vertex_id, to_vertex_id)) {
      return Edge::Color::Yellow;
    }
    if (to_vertex_depth - from_vertex_depth == 2) {
      return Edge::Color::Red;
    }

    throw std::runtime_error("Failed to determine color");
  }

  int is_connected(const VertexId first_vertex_id,
                   const VertexId second_vertex_id) const {
    return std::find_first_of(
               adjacency_list_.find(first_vertex_id)->second.begin(),
               adjacency_list_.find(first_vertex_id)->second.end(),
               adjacency_list_.find(second_vertex_id)->second.begin(),
               adjacency_list_.find(second_vertex_id)->second.end()) !=
           adjacency_list_.find(first_vertex_id)->second.end();
  }

  Depth get_vertex_depth(const VertexId vertex_id) const {
    return vertex_depths_list_.at(vertex_id);
  }

  void set_vertex_depth(const VertexId vertex_id, const Depth depth) {
    if (vertex_depths_list_.find(vertex_id) != vertex_depths_list_.end()) {
      const Depth previous_depth = get_vertex_depth(vertex_id);
      depth_vertices_list_[depth].erase(
          std::remove(depth_vertices_list_[depth].begin(),
                      depth_vertices_list_[depth].end(), vertex_id),
          depth_vertices_list_[depth].end());
    }

    depth_vertices_list_[depth].push_back(vertex_id);
    vertex_depths_list_[vertex_id] = depth;
  }

  VertexId next_free_vertex_id_ = 0;
  EdgeId next_free_edge_id_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
  std::unordered_map<VertexId, Depth> vertex_depths_list_;
  std::unordered_map<Depth, std::vector<VertexId>> depth_vertices_list_;
};

class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth, int new_vertices_count)
        : depth_(depth), new_vertices_count_(new_vertices_count) {}

    Graph::Depth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(Params&& params) : params_(std::move(params)) {}

  Graph generate() const {
    auto graph = Graph();
    graph.add_vertex();
    generate_grey_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);

    return graph;
  }

 private:
  void generate_grey_edges(Graph& graph) const {}

  void generate_green_edges(Graph& graph) const {}

  void generate_yellow_edges(Graph& graph) const {}

  void generate_red_edges(Graph& graph) const {}

  Params params_ = Params(0, 0);
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

int handle_new_vertices_count_input() {
  const std::string init_message = "Type new vertices count: ";
  const std::string err_format_message =
      "Vertices count must be a non-negative integer. Try again";
  const std::string err_fatal_message =
      "Occurred fatal error in handle_new_vertices_count_input";
  int new_vertices_count;
  int correct_input = false;

  std::cout << init_message << std::endl;

  while (correct_input == false) {
    if (std::cin >> new_vertices_count && new_vertices_count >= 0) {
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

  return new_vertices_count;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();

  auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(std::move(params));
  const auto graph = generator.generate();

  const auto graph_json = printing::json::print_graph(graph);
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
