#pragma once
#include "graph.hpp"

namespace uni_course_cpp {

class GraphGenerator {
 public:
  static constexpr double kProbabilityRed = 0.33;
  static constexpr double kProbabilityGreen = 0.1;
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
  Graph generate() const;

 private:
  bool check_probability(double chance) const;

  double probaility_generate_grey_edge(Graph::Depth current_depth,
                                       Graph::Depth graph_depth) const;

  Graph::VertexId get_random_vertex_id(int size) const;

  std::vector<Graph::VertexId> get_unconnected_vertex_ids(
      const Graph& graph,
      Graph::VertexId vertex_from_id,
      const std::vector<Graph::VertexId>& vertex_ids) const;

  void try_generate_grey_edge(Graph& graph,
                              Graph::Depth current_depth,
                              Graph::VertexId vertex_id) const;

  void generate_grey_edges(Graph& graph) const;

  void try_generate_yellow_edge(Graph& graph,
                                Graph::VertexId vertex_from_id,
                                Graph::VertexId vertex_to_id) const;

  void generate_yellow_edges(Graph& graph) const;

  void try_generate_red_edge(Graph& graph,
                             Graph::VertexId vertex_from_id,
                             Graph::VertexId vertex_to_id) const;

  void generate_red_edges(Graph& graph) const;

  void try_generate_green_edge(Graph& graph, Graph::VertexId vertex_id) const;

  void generate_green_edges(Graph& graph) const;

  Params params_ = Params(0, 0);
};

}  // namespace uni_course_cpp