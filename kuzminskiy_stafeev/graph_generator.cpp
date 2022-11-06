#include "graph_generator.hpp"
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <random>
#include <thread>

namespace uni_course_cpp {

const int kMaxThreadsCount = std::thread::hardware_concurrency();

namespace {
std::vector<Graph::VertexId> get_unconnected_vertex_ids(
    const Graph& graph,
    Graph::VertexId vertex_id,
    const std::vector<Graph::VertexId>& vertex_ids_on_depth) {
  std::vector<Graph::VertexId> unconnected_vertices = {};
  for (const auto cur_vertex_id : vertex_ids_on_depth) {
    if (!graph.is_connected(vertex_id, cur_vertex_id)) {
      unconnected_vertices.emplace_back(cur_vertex_id);
    }
  }

  return unconnected_vertices;
}

int get_random_vertex(const std::vector<Graph::VertexId>& vertex_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, vertex_ids.size() - 1);
  return vertex_ids[dis(gen)];
}

bool check_probability(float prob) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution d(prob);
  return d(gen);
}

void generate_green_edges(Graph& graph) {
  static float constexpr prob = 0.1;
  const auto& vertices = graph.vertices();
  for (const auto& [vertex_id, vertex] : vertices) {
    if (check_probability(prob)) {
      graph.add_edge(vertex.id(), vertex.id());
    }
  }
}

void generate_yellow_edges(Graph& graph, Graph::Depth depth) {
  float step = 1.0 / (depth - 2);
  for (Graph::Depth cur_depth = 2; cur_depth <= depth - 1; cur_depth++) {
    float prob = (float)(step * (cur_depth - 1));

    for (const auto from_vertex_id : graph.vertices_of_depth(cur_depth)) {
      if (check_probability(prob)) {
        const auto unconnected_vertices_ids = get_unconnected_vertex_ids(
            graph, from_vertex_id, graph.vertices_of_depth(cur_depth + 1));
        if (!unconnected_vertices_ids.empty()) {
          const auto to_vertex_id = get_random_vertex(unconnected_vertices_ids);
          graph.add_edge(from_vertex_id, to_vertex_id);
        }
      }
    }
  }
}

void generate_red_edges(Graph& graph, Graph::Depth depth) {
  static float constexpr prob = (float)1 / 3;
  for (Graph::Depth cur_depth = 1; cur_depth <= depth - 2; cur_depth++) {
    const auto& next_vertices_depth = graph.vertices_of_depth(cur_depth + 2);

    for (const auto from_vertex_id : graph.vertices_of_depth(cur_depth)) {
      const auto to_vertex_id = get_random_vertex(next_vertices_depth);
      if (check_probability(prob)) {
        graph.add_edge(from_vertex_id, to_vertex_id);
      }
    }
  }
}

}  // namespace

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  if (params_.depth()) {
    graph.add_vertex();
    generate_grey_edges(graph);
    std::thread green_thread(generate_green_edges, std::ref(graph));
    std::thread yellow_thread(&generate_yellow_edges, std::ref(graph),
                              graph.get_graph_depth());
    std::thread red_thread(&generate_red_edges, std::ref(graph),
                           graph.get_graph_depth());

    green_thread.join();
    yellow_thread.join();
    red_thread.join();
  }

  return graph;
}

using JobCallBack = std::function<void()>;

bool has_job(std::mutex& jobs_mutex,
             std::atomic<bool>& should_terminate,
             const std::list<JobCallBack>& jobs) {
  jobs_mutex.lock();

  if (!jobs.empty()) {
    return true;
  } else {
    should_terminate = true;
    jobs_mutex.unlock();
    return false;
  }
}

JobCallBack get_job(std::mutex& jobs_mutex, std::list<JobCallBack>& jobs) {
  auto job = jobs.front();
  jobs.pop_front();
  jobs_mutex.unlock();
  return job;
}

void GraphGenerator::generate_grey_branch(
    Graph& graph,
    std::mutex& jobs_mutex,
    std::vector<Graph::VertexId> previous_vertex_ids,
    Graph::Depth cur_depth) const {
  const auto depth = params_.depth();
  std::vector<Graph::VertexId> cur_vertex_ids;

  if (depth < cur_depth) {
    return;
  } else if (cur_depth == 2) {
    jobs_mutex.lock();
    auto vertex_id = graph.add_vertex();
    graph.add_edge(0, vertex_id);
    jobs_mutex.unlock();
    cur_vertex_ids.emplace_back(vertex_id);
    generate_grey_branch(graph, jobs_mutex, cur_vertex_ids, cur_depth + 1);
    return;
  }

  float step = 1.0 / (depth - 1);
  float prob = (float)(1 - step * (cur_depth - 2));

  const auto new_vertices_count = params_.new_vertices_count();

  for (const auto from_vertex_id : previous_vertex_ids) {
    for (Graph::VertexId cur_vertex_count = 0;
         cur_vertex_count < new_vertices_count; cur_vertex_count++) {
      if (check_probability(prob)) {
        jobs_mutex.lock();
        auto vertex_id = graph.add_vertex();
        graph.add_edge(from_vertex_id, vertex_id);
        jobs_mutex.unlock();
        cur_vertex_ids.emplace_back(vertex_id);
      }
    }
  }

  generate_grey_branch(graph, jobs_mutex, cur_vertex_ids, cur_depth + 1);
}

void GraphGenerator::generate_grey_edges(Graph& graph) const {
  const auto depth = params_.depth();
  if (depth == 1) {
    return;
  }

  auto jobs = std::list<JobCallBack>();
  const auto new_vertices_count = params_.new_vertices_count();
  std::mutex graph_mutex;
  std::vector<Graph::VertexId> base_vertex_id = {0};

  for (int i = 0; i < new_vertices_count; i++) {
    jobs.push_back(
        [&]() { generate_grey_branch(graph, graph_mutex, base_vertex_id, 2); });
  }

  std::atomic<bool> should_terminate(false);
  std::mutex jobs_mutex;
  const auto worker = [&should_terminate, &jobs_mutex, &jobs]() {
    while (true) {
      if (should_terminate) {
        return;
      }

      const auto job_optional = [&jobs_mutex, &should_terminate,
                                 &jobs]() -> std::optional<JobCallBack> {
        if (has_job(jobs_mutex, should_terminate, jobs)) {
          return get_job(jobs_mutex, jobs);
        }

        return std::nullopt;
      }();

      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
      }
    }
  };

  const auto threads_count = std::min(kMaxThreadsCount, new_vertices_count);
  auto threads = std::vector<std::thread>();
  for (int i = 0; i < threads_count; i++) {
    threads.emplace_back(std::thread(worker));
  }

  for (auto& thread : threads) {
    thread.join();
  }
}

}  // namespace uni_course_cpp
