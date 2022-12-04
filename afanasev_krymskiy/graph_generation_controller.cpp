#include "graph_generation_controller.hpp"
#include <atomic>
#include <cassert>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>

namespace uni_course_cpp {
void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::atomic<int> wait_for_jobs_count = graphs_count_;

  for (int index = 0; index < graphs_count_; ++index)
    jobs_.emplace_back([&gen_started_callback, &gen_finished_callback,
                        &wait_for_jobs_count, index, this]() {
      gen_started_callback(index);
      auto graph = graph_generator_.generate();
      gen_finished_callback(index, std::move(graph));
      --wait_for_jobs_count;
    });

  std::mutex mutex_for_jobs;
  const auto job_optional = [&mutex_for_jobs,
                             this]() -> std::optional<JobCallback> {
    const std::lock_guard lock(mutex_for_jobs);
    if (!jobs_.empty()) {
      const auto item = jobs_.back();
      jobs_.pop_back();
      return item;
    }
    return std::nullopt;
  };

  for (int i = 0; i < threads_count_; ++i)
    workers_.emplace_back(job_optional);

  for (auto& worker : workers_)
    worker.start();

  while (wait_for_jobs_count)
    ;

  for (auto& worker : workers_)
    worker.stop();
}

void GraphGenerationController::Worker::start() {
  assert(state_ == State::Idle);
  thread_ = std::thread([this]() {
    while (true) {
      if (state_ == State::ShouldTerminate) {
        return;
      }
      const auto job_optional = get_job_callback_();
      if (job_optional.has_value()) {
        state_ = State::Working;
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
  if (thread_.joinable())
    stop();
}
}  // namespace uni_course_cpp