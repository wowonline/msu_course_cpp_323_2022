#include "graph.hpp"

namespace uni_course_cpp {

namespace {
static constexpr double kGreenEdgesProbability = 0.1;
static constexpr double kRedEdgesProbability = 0.33;
static constexpr int kGraphBaseDepth = 1;
static constexpr int kYellowEdgeDepth = 1;
static constexpr int kRedEdgeDepth = 2;
static constexpr int kYellowMaxEdgeDepth = 2;
bool check_probability(const double probability);

};  // namespace

class GraphGenerator {
 public:
  struct Params {
   public:
    Params(Graph::Depth depth, int new_vertices_count)
        : depth_(depth), new_vertices_count_(new_vertices_count){};

    Graph::Depth depth() const { return depth_; }
    int new_vertices_count() const { return new_vertices_count_; }

   private:
    Graph::Depth depth_ = 0;
    int new_vertices_count_ = 0;
  };

  explicit GraphGenerator(Params&& params) : params_(std::move(params)){};

  void generate_grey_edges(Graph& graph) const;
  void generate_green_edges(Graph& graph) const;
  void generate_yellow_edges(Graph& graph) const;
  void generate_red_edges(Graph& graph) const;

  Graph generate() const;

 private:
  Params params_ = Params(0, 0);
};

Graph::VertexId get_random_vertex_id(std::size_t size);

std::vector<Graph::VertexId> get_unconnected_vertex_ids(
    const std::vector<Graph::VertexId>& vertices,
    const Graph& graph,
    const Graph::VertexId vertex);

namespace constants {
static double green_edges_probability = 0.1;
static double red_edges_probability = 0.33;
};  // namespace constants
};  // namespace uni_course_cpp