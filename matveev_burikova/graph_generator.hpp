#pragma once

#include "graph.hpp"

namespace uni_course_cpp {
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
  Graph generate() const;

 private:
  Params params_ = Params(0, 0);
};
}  // namespace uni_course_cpp

namespace {
using Graph = uni_course_cpp::Graph;
using GraphGenerator = uni_course_cpp::GraphGenerator;

const std::vector<Graph::VertexId> get_unconnected_vertex_ids(
    Graph graph,
    Graph::VertexId vertex_id,
    const std::vector<Graph::VertexId>& vertex_ids_on_depth);

bool check_probability(double probability);

Graph::VertexId get_random_vertex_id(
    const std::vector<Graph::VertexId>& vertex_ids_list);

void generate_grey_edges(Graph& graph, const GraphGenerator::Params params);

void generate_green_edges(Graph& graph);

void generate_yellow_edges(Graph& graph);

void generate_red_edges(Graph& graph);
}  // namespace
