#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"

#include <atomic>
#include <cassert>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <thread>

namespace uni_course_cpp {
GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    GraphGenerator::Params&& graph_generator_params)
    : threads_count_(threads_count),
      graphs_count_(graphs_count),
      graph_generator_(std::move(graph_generator_params)) {}

using JobCallback = std::function<void()>;
JobCallback GraphGenerationController::get_job() {
  assert(!jobs_.empty());
  const auto job = jobs_.front();
  jobs_.pop_front();
  return job;
}
void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  for (int index = 0; index < graphs_count_; ++index) {
    jobs_.emplace_back(
        [this, &gen_started_callback, &gen_finished_callback, index]() {
          gen_started_callback(index);
          printf("{%d gen_started}\n", index);
          auto graph = graph_generator_.generate();
          gen_finished_callback(index, std::move(graph));
          printf("{%d gen_finished}\n", index);
        });
  }
  std::mutex jobs_mutex;
  std::atomic<int> waiting_jobs_count = graphs_count_;

  const Worker::GetJobCallback get_job_callback =
      [this, &jobs_mutex]() -> std::optional<JobCallback> {
    const std::lock_guard<std::mutex> guard(jobs_mutex);
    if (!jobs_.empty()) {
      return get_job();
    }
    return std::nullopt;
  };

  for (int i = 0; i < threads_count_; ++i)
    workers_.emplace_back(get_job_callback);
  for (auto& worker : workers_)
    worker.start(waiting_jobs_count);

  while (waiting_jobs_count > 0) {
  }
  for (auto& worker : workers_)
    worker.~Worker();
}
void GraphGenerationController::Worker::start(
    std::atomic<int>& waiting_jobs_count) {
  assert(state_ == State::Idle);
  state_ = State::Working;

  thread_ = std::thread([this, &waiting_jobs_count]() {
    while (true) {
      if (state_ == State::ShouldTerminate) {
        return;
      }
      const auto job_optional = get_job_callback_();
      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
        waiting_jobs_count = waiting_jobs_count - 1;
      }
    }
  });
}

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working);
  state_ = State::ShouldTerminate;
  thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  if (thread_.joinable())
    stop();
}
}  // namespace uni_course_cpp