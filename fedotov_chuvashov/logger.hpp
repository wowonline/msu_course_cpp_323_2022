#pragma once
#include <string>

namespace uni_course_cpp {
class Logger {
 public:
  void log(const std::string& string);
  static Logger& get_logger();
  Logger(const Logger&) = delete;
  void operator=(const Logger&) = delete;

 private:
  Logger() {}
};

std::string generation_started_string(int i);
std::string generation_finished_string(int i, std::string content);

}  // namespace uni_course_cpp
