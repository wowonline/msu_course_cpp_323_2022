#pragma once

#include <iostream>

namespace uni_course_cpp {
class Logger {
 public:
  static Logger& get_logger();
  void log(const std::string& string) const;
};
}  // namespace uni_course_cpp
