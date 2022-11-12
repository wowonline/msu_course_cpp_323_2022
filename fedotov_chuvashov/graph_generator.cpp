#include "graph_generator.hpp"
#include <algorithm>
#include <atomic>
#include <deque>
#include <optional>
#include <random>
#include <thread>

namespace uni_course_cpp {

namespace {
bool check_probabilty(float probability) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution p(probability);

  return p(gen);
}

int get_random_vertex_id(const std::set<Graph::VertexId>& vertex_ids) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, vertex_ids.size() - 1);
  return *std::next(vertex_ids.begin(), distrib(gen));
}

std::set<Graph::VertexId> get_unconnected_vertex_ids(
    const Graph& graph,
    Graph::Depth current_depth,
    Graph::VertexId vertex_id) {
  const std::set<Graph::VertexId>& deeper_vertices =
      graph.vertices_at_depth(current_depth + kYellowDepthDifference);
  const std::set<Graph::VertexId> children_vertices =
      graph.children_of_vertex(vertex_id);
  std::set<Graph::VertexId> unconnected_vertices;
  std::set_difference(
      deeper_vertices.begin(), deeper_vertices.end(), children_vertices.begin(),
      children_vertices.end(),
      std::inserter(unconnected_vertices, unconnected_vertices.begin()));
  return unconnected_vertices;
}

void generate_green_edges(Graph& graph, std::mutex& mutex) {
  constexpr float probabilty_of_green_edge = 0.1;
  auto vertices = graph.vertices();
  std::for_each(vertices.begin(), vertices.end(),
                [&](const std::pair<Graph::VertexId, Graph::Vertex>& element) {
                  Graph::VertexId vertex_id = element.first;
                  if (check_probabilty(probabilty_of_green_edge)) {
                    mutex.lock();
                    graph.add_edge(vertex_id, vertex_id);
                    mutex.unlock();
                  }
                });
}

void generate_red_edges(Graph& graph, std::mutex& mutex) {
  constexpr float probabilty_of_red_edge = 0.33;
  for (Graph::Depth current_depth = 1;
       current_depth <= graph.depth() - kRedDepthDifference; ++current_depth) {
    auto vertices = graph.vertices_at_depth(current_depth);
    std::for_each(
        vertices.begin(), vertices.end(), [&](const Graph::VertexId vertex_id) {
          if (check_probabilty(probabilty_of_red_edge)) {
            const std::set<Graph::VertexId>& deeper_vertices =
                graph.vertices_at_depth(current_depth + kRedDepthDifference);
            if (deeper_vertices.size() > 0) {
              const Graph::VertexId random_vertex_id =
                  get_random_vertex_id(deeper_vertices);
              mutex.lock();
              graph.add_edge(vertex_id, random_vertex_id);
              mutex.unlock();
            }
          }
        });
  }
}

void generate_yellow_edges(Graph& graph, std::mutex& mutex) {
  for (Graph::Depth current_depth = 1;
       current_depth <= graph.depth() - kYellowDepthDifference;
       ++current_depth) {
    auto vertices = graph.vertices_at_depth(current_depth);
    std::for_each(
        vertices.begin(), vertices.end(), [&](const Graph::VertexId vertex_id) {
          const float probabilty_of_yellow_edge =
              static_cast<float>((current_depth - kYellowDepthDifference)) /
              (graph.depth() - kYellowDepthDifference - kGraphBaseDepth);
          if (check_probabilty(probabilty_of_yellow_edge)) {
            mutex.lock();
            std::set<Graph::VertexId> unconnected_vertices =
                get_unconnected_vertex_ids(graph, current_depth, vertex_id);
            if (unconnected_vertices.size() > 0) {
              const Graph::VertexId random_vertex_id =
                  get_random_vertex_id(unconnected_vertices);
              graph.add_edge(vertex_id, random_vertex_id);
            }
            mutex.unlock();
          }
        });
  }
}

}  // namespace

void GraphGenerator::generate_branch(Graph& graph,
                                     Graph::VertexId root_id,
                                     Graph::Depth depth,
                                     std::mutex& mutex) const {
  if (depth <= 0) {
    return;
  }
  mutex.lock();
  const float probabilty_of_grey_edge =
      1.0 - static_cast<float>(
                (graph.get_vertex_depth(root_id) - kGreyDepthDifference)) /
                (params_.depth() - kGreyDepthDifference);
  mutex.unlock();
  for (int i = 0; i < params_.new_vertices_count(); ++i) {
    if (check_probabilty(probabilty_of_grey_edge)) {
      mutex.lock();
      const Graph::VertexId child_id = graph.add_vertex();
      graph.add_edge(root_id, child_id);
      mutex.unlock();
      generate_branch(graph, child_id, depth - 1, mutex);
    }
  }
}

using JobCallback = std::function<void()>;
const int kMaxThreadsCount = std::thread::hardware_concurrency();

void GraphGenerator::generate_new_vertices(Graph& graph,
                                           Graph::VertexId root_id,
                                           Graph::Depth depth) const {
  std::deque<JobCallback> task_queue;
  std::mutex mutex;
  for (int i = 0; i < params_.new_vertices_count(); ++i) {
    const Graph::VertexId child_id = graph.add_vertex();
    graph.add_edge(root_id, child_id);
    task_queue.push_front(
        [&]() { generate_branch(graph, child_id, depth - 1, mutex); });
  }

  const int number_of_threads =
      std::min(params_.new_vertices_count(), kMaxThreadsCount);
  std::vector<std::thread> workers;
  std::mutex queue_mutex;
  workers.reserve(number_of_threads);
  for (int i = 0; i < number_of_threads; ++i) {
    workers.emplace_back([&]() {
      JobCallback task;
      while (true) {
        queue_mutex.lock();
        if (!task_queue.empty()) {
          task = task_queue.back();
          task_queue.pop_back();
        } else {
          queue_mutex.unlock();
          return;
        }
        queue_mutex.unlock();
        task();
      }
    });
  }
  for (auto& worker : workers) {
    worker.join();
  }
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  if (params_.depth() > 0) {
    const Graph::VertexId root_id = graph.add_vertex();
    generate_new_vertices(graph, root_id, params_.depth());
  }
  std::mutex mutex;
  auto green_thread =
      std::thread([&]() { generate_green_edges(graph, mutex); });
  auto red_thread = std::thread([&]() { generate_red_edges(graph, mutex); });
  auto yellow_thread =
      std::thread([&]() { generate_yellow_edges(graph, mutex); });
  green_thread.join();
  red_thread.join();
  yellow_thread.join();

  return graph;
}
}  // namespace uni_course_cpp
