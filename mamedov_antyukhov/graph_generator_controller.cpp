#include "graph_generator_controller.hpp"
#include <atomic>

namespace uni_course_cpp {

void GraphGenerationController::Worker::start() {
  thread_ = std::thread([this]() {
    while (state_ != State::ShouldTerminate) {
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
  state_ = State::ShouldTerminate;
  thread_.join();
}

void GraphGenerationController::generate(
    const GenStartedCallback& gen_started_callback,
    const GenFinishedCallback& gen_finished_callback) {
  std::atomic<int> graphs_created_num = graphs_count_;
  for (int i = 0; i < graphs_count_; ++i) {
    jobs_.push([&gen_started_callback, &gen_finished_callback, this, i,
                &graphs_created_num]() {
      gen_started_callback(i);
      auto graph = graph_generator_.generate();
      gen_finished_callback(i, std::move(graph));
      --graphs_created_num;
    });
  }

  std::mutex jobs_mutex;
  const auto get_job_callback =
      [&jobs_mutex,
       this]() -> std::optional<GraphGenerationController::JobCallback> {
    const std::lock_guard<std::mutex> jobs_lock(jobs_mutex);
    if (!jobs_.empty()) {
      auto job = jobs_.front();
      jobs_.pop();
      return job;
    }

    return std::nullopt;
  };

  for (int i = 0; i < graphs_count_; ++i) {
    workers_.emplace_back(get_job_callback);
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (graphs_created_num) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}
}  // namespace uni_course_cpp