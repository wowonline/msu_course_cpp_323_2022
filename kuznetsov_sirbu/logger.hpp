#pragma once
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include "config.hpp"

namespace uni_course_cpp {
class Logger {
 public:
  static Logger& get_instance();
  void log(const std::string& string);

  Logger(const Logger&) = delete;
  void operator=(const Logger&) = delete;
  Logger(Logger&& other) = delete;
  void operator=(Logger&& other) = delete;

 private:
  Logger() : log_stream_(config::kLogFilePath, std::ofstream::app) {}
  ~Logger() { log_stream_.close(); };

  std::ofstream log_stream_;
};
}  // namespace uni_course_cpp