#include "graph.hpp"


bool get_random_bool(float true_probability) {
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::bernoulli_distribution bernoulli_distribution(true_probability);
  return bernoulli_distribution(generator);
}

void write_to_file(const std::string& graph_json,
                   const std::string& file_name) {
  std::ofstream json_file(file_name);

  json_file << graph_json;
}

int handle_depth_input() {
  const std::string init_message = "Type graph depth: ";
  const std::string err_format_message =
      "The depth must be a non-negative integer. Try again";

  int depth;
  int correct_input = false;

  std::cout << init_message << std::endl;

  while (correct_input == false) {
    if (std::cin >> depth && depth >= 0) {
      correct_input = true;
    } else if (std::cin.fail() || depth < 0) {
      std::cout << err_format_message << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      throw std::runtime_error("Failed to get depth from user");
    }
  }

  return depth;
}

int handle_new_vertices_count_input() {
  const std::string init_message = "Type new vertices count: ";
  const std::string err_format_message =
      "Vertices count must be a non-negative integer. Try again";
  int new_vertices_count;
  int correct_input = false;

  std::cout << init_message << std::endl;

  while (correct_input == false) {
    if (std::cin >> new_vertices_count && new_vertices_count >= 0) {
      correct_input = true;
    } else if (std::cin.fail() || new_vertices_count < 0) {
      std::cout << err_format_message << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      throw std::runtime_error("Failed to get new vertices count from user");
    }
  }

  return new_vertices_count;
}



Graph::VertexId get_random_vertex_id(
    const std::vector<Graph::VertexId>& vertex_ids) {
  assert((vertex_ids.empty() == false) &&
         "Can't pick random vertex id from empty list");

  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> uniform_int_distribution(
      0, vertex_ids.size() - 1);

  return vertex_ids[uniform_int_distribution(generator)];
}

std::vector<Graph::VertexId> get_unconnected_vertex_ids(
    const Graph& graph,
    Graph::VertexId vertex_id) {
  std::vector<Graph::VertexId> unconnected_vertex_ids = {};
  for (const auto next_depth_vertex_id :
       graph.get_depth_vertex_ids(graph.get_vertex_depth(vertex_id) + 1)) {
    if (graph.is_vertices_connected(vertex_id, next_depth_vertex_id) == false) {
      unconnected_vertex_ids.push_back(next_depth_vertex_id);
    }
  }

  return unconnected_vertex_ids;
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
