#include "logger.hpp"
#include "config.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}
}  // namespace

namespace uni_course_cpp {
Logger& Logger::get_logger() {
  static Logger instance;
  return instance;
}

void Logger::log(const std::string& string) {
  const auto string_with_date_time = get_current_date_time() + " " + string;
  std::cout << string_with_date_time << std::endl;
  if (!log_file.is_open())
    log_file.open(config::kLogFilePath);
  log_file << string_with_date_time << std::endl;
}
}  // namespace uni_course_cpp
