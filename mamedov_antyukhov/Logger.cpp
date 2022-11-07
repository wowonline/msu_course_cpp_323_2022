#include "Logger.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "config.hpp"

namespace uni_course_cpp {
std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S ");
  return date_time_string.str();
}

Logger& Logger::get_logger() {
  static Logger instance;
  std::ofstream log_file(config::kLogFilePath);
  log_file.close();
  return instance;
}

void Logger::log(const std::string& string) const {
  std::ofstream log_file(config::kLogFilePath, std::ios::app);
  std::string current_date_time = get_current_date_time();
  log_file << current_date_time << string;
  std::cout << current_date_time << string;
}
}  // namespace uni_course_cpp