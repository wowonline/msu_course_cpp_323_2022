#include "graph_printing.hpp"

namespace uni_course_cpp {

std::string printing::print_edge_color(const Graph::Edge::Color& color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    case Graph::Edge::Color::Green:
      return "green";
    default:
      return "No color";
  }
}

std::string printing::print_graph(const Graph& graph) {
  const auto depth = graph.get_graph_depth();

  const auto vertices = graph.vertices().size();
  const auto edges = graph.edges().size();

  std::string depths_s = "";

  for (int i = 1; i < depth + 1; i++) {
    depths_s += std::to_string(graph.vertices_of_depth(i).size());

    if (i != depth) {
      depths_s += ", ";
    }
  }

  std::string edges_s =
      "grey: " +
      std::to_string(graph.edges_of_color(Graph::Edge::Color::Grey)) + ", " +
      "green: " +
      std::to_string(graph.edges_of_color(Graph::Edge::Color::Green)) + ", " +
      "yellow: " +
      std::to_string(graph.edges_of_color(Graph::Edge::Color::Yellow)) + ", " +
      "red: " + std::to_string(graph.edges_of_color(Graph::Edge::Color::Red));

  return "  depth: " + std::to_string(depth) + ",\n" +
         "  vertices: {amount: " + std::to_string(vertices) +
         ", distribution: " + "[" + depths_s + "]},\n" +
         "  edges: {amount: " + std::to_string(edges) +
         ", distribution: " + "{" + edges_s + "}}";
}

std::string printing::get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string printing::generation_started_string(int i) {
  return get_current_date_time() + " Graph " + std::to_string(i) +
         ", Generation Started";
}

std::string printing::generation_finished_string(
    int i,
    std::string graph_description) {
  return get_current_date_time() + " Graph " + std::to_string(i) +
         ", Generation Finished {\n" + graph_description + "\n}";
}

}  // namespace uni_course_cpp
