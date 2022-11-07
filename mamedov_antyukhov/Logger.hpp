#pragma once
#include <fstream>
#include <string>

namespace uni_course_cpp {
class Logger {
 public:
  static Logger& get_logger();
  void log(const std::string& string) const;

 private:
  Logger(){};
  std::ofstream log_file_;
};
}  // namespace uni_course_cpp