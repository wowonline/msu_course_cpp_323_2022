#pragma once
#include <fstream>
#include <string>
#include "config.hpp"

namespace uni_course_cpp {
class Logger {
 public:
  void log(const std::string& string);
  static Logger& get_logger();
  Logger(const Logger&) = delete;
  void operator=(const Logger&) = delete;
  Logger(Logger&& other) = delete;
  void operator=(Logger&& other) = delete;

 private:
  std::ofstream log_stream;
  Logger() { log_stream.open(config::kLogFilePath, std::ofstream::app); };
  ~Logger() { log_stream.close(); };
};
}  // namespace uni_course_cpp
