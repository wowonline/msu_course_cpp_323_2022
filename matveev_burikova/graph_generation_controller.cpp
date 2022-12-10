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
      graph_generator_(std::move(graph_generator_params)) {
  waiting_jobs_count_ = graphs_count_;
  std::mutex jobs_mutex;
  const Worker::GetJobCallback get_job_callback =
      [this, &jobs_mutex]() -> std::optional<JobCallback> {
    const std::lock_guard<std::mutex> guard(jobs_mutex);
    if (!jobs_.empty()) {
      --waiting_jobs_count_;
      return get_job();
    }
    return std::nullopt;
  };
  for (int i = 0; i < threads_count_; ++i)
    workers_.emplace_back(get_job_callback);
}

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
  std::mutex callback_mutex;

  for (int index = 0; index < graphs_count_; ++index) {
    jobs_.emplace_back([this, &gen_started_callback, &gen_finished_callback,
                        index, &callback_mutex]() {
      callback_mutex.lock();
      gen_started_callback(index);
      callback_mutex.unlock();

      auto graph = graph_generator_.generate();

      callback_mutex.lock();
      gen_finished_callback(index, std::move(graph));
      callback_mutex.unlock();
    });
  }

  for (auto& worker : workers_)
    worker.start();

  while (waiting_jobs_count_ > 0) {
  }
  for (auto& worker : workers_)
    worker.stop();
}
void GraphGenerationController::Worker::start() {
  assert(state_ == State::Idle);
  state_ = State::Working;

  thread_ = std::thread([this]() {
    while (true) {
      if (state_ == State::ShouldTerminate) {
        return;
      }
      const auto job_optional = get_job_callback_();
      if (job_optional.has_value()) {
        const auto& job = job_optional.value();
        job();
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
  assert(state_ != State::Idle);
  if (state_ == State::Working)
    stop();
}
}  // namespace uni_course_cpp
