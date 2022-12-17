#include "printing.hpp"

namespace uni_course_cpp {
namespace printing {

std::string print_graph(const IGraph& graph) {
  std::ostringstream result;
  result << "{\n\tdepth: " << graph.depth() << ",\n";
  result << "\tvertices: "
         << "{amount: " << graph.get_vertices().size() << ", "
         << "distribution: [";
  for (Depth depth = 1; depth <= graph.depth(); ++depth) {
    result << graph.get_vertices_with_depth(depth).size();
    if (depth < graph.depth()) {
      result << ", ";
    }
  }

  result << "]},\n\tedges: {amount: " << graph.get_edges().size();
  result << ", distribution: {grey: "
         << graph.get_edges_with_color(Color::Grey).size();
  result << ", green: " << graph.get_edges_with_color(Color::Green).size();
  result << ", yellow: " << graph.get_edges_with_color(Color::Yellow).size();
  result << ", red: " << graph.get_edges_with_color(Color::Red).size()
         << "}}\n}";

  return result.str();
}

}  // namespace printing
}  // namespace uni_course_cpp