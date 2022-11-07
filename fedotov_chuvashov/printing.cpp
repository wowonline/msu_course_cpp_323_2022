#include "printing.hpp"
#include <sstream>

namespace uni_course_cpp {
namespace printing {
std::string print_edge_color(Graph::Edge::Color color) {
  switch (color) {
    case Graph::Edge::Color::Grey:
      return "grey";
    case Graph::Edge::Color::Yellow:
      return "yellow";
    case Graph::Edge::Color::Red:
      return "red";
    case Graph::Edge::Color::Green:
      return "green";
  }
}

std::string print_graph(const Graph& graph) {
  std::ostringstream result;
  result << "{\n\tdepth: " << graph.depth()
         << ",\n\tvertices: {amount: " << graph.vertices().size()
         << ", distribution: [";
  for (int i = kGraphBaseDepth; i <= graph.depth(); ++i) {
    result << graph.vertices_at_depth(i).size();
    if (i != graph.depth())
      result << ", ";
  }
  result << "]},\n\tedges: {amount: " << graph.edges().size()
         << ", distribution: {grey: "
         << graph.get_color_amount(Graph::Edge::Color::Grey)
         << ", green: " << graph.get_color_amount(Graph::Edge::Color::Green)
         << ", yellow: " << graph.get_color_amount(Graph::Edge::Color::Yellow)
         << ", red: " << graph.get_color_amount(Graph::Edge::Color::Red)
         << "}}\n}";
  return result.str();
}
};  // namespace printing
}  // namespace uni_course_cpp
