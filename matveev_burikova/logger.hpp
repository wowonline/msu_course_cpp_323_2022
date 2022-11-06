#pragma once

#include "config.hpp"

#include <fstream>

namespace {
std::string get_current_date_time();
std::ofstream log_file(uni_course_cpp::config::kLogFilePath,
                       std::ios_base::app);
}  // namespace

namespace uni_course_cpp {
class Logger {
 public:
  static Logger& get_logger();
  void log(const std::string& string) const;

 private:
  void log_to_file(const std::string& content) const;
};
}  // namespace uni_course_cpp
