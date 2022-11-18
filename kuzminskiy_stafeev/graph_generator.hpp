#pragma once
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>
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
  void generate_grey_edges(Graph& graph,
                           Graph::VertexId root_id,
                           std::mutex& graph_mutex) const;
  void generate_grey_branch(Graph& graph,
                            std::mutex& graph_mutex,
                            Graph::VertexId root_id,
                            Graph::Depth cur_depth) const;

  Params params_ = Params(0, 0);
};

class GraphGenerationController {
 public:
  using GenStartedCallback = std::function<void(int index)>;
  using GenFinishedCallback = std::function<void(int index, Graph&& graph)>;

  GraphGenerationController(int threads_count,
                            int graphs_count,
                            GraphGenerator::Params&& graph_generator_params);

  void generate(const GenStartedCallback& gen_started_callback,
                const GenFinishedCallback& gen_finished_callback);

 private:
  using JobCallback = std::function<void()>;
  using GetJobCallback = std::function<std::optional<JobCallback>()>;

  class Worker {
   public:
    explicit Worker(const GetJobCallback& get_job_callback)
        : thread_(), get_job_callback_(get_job_callback) {}

    ~Worker(void);
    void start();
    void stop();

   private:
    enum class State { Idle, Working, ShouldTerminate };

    std::thread thread_;
    GetJobCallback get_job_callback_;
    State state_ = State::Idle;
  };

  std::list<Worker*> workers_;
  std::list<JobCallback> jobs_;

  int threads_count_;
  int graphs_count_;
  GraphGenerator graph_generator_;
};

}  // namespace uni_course_cpp
