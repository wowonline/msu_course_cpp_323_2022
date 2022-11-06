#pragma once

#include <fstream>
#include "config.hpp"

namespace uni_course_cpp {
class Logger {
 public:
  static Logger& get_logger();

  void log(const std::string& string);

 private:
  Logger(){};
  Logger(Logger const&){};
  void operator=(Logger const&){};
  ~Logger() = default;

  std::ofstream log_file_;
};
}  // namespace uni_course_cpp
