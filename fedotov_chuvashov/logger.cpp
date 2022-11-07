#include "logger.hpp"
#include <iostream>

namespace uni_course_cpp {

Logger& Logger::get_logger() {
  static Logger logger;
  return logger;
}

void Logger::log(const std::string& string) {
  log_stream << string;
  std::cout << string;
}
}  // namespace uni_course_cpp
