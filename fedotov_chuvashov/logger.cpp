#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "config.hpp"

namespace uni_course_cpp {

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

std::string generation_started_string(int i) {
  std::stringstream start_string;
  start_string << get_current_date_time() << " Graph " << i
               << ", Generation Started\n";
  return start_string.str();
}

std::string generation_finished_string(int i, std::string content) {
  std::stringstream finish_string;
  finish_string << get_current_date_time() << " Graph " << i
                << ", Generation Finished " << content << "\n";
  return finish_string.str();
}

Logger& Logger::get_logger() {
  static Logger logger;
  return logger;
}

void Logger::log(const std::string& string) {
  std::ofstream log_file;
  log_file.open(config::kLogFilePath, std::ofstream::app);
  log_file << string;
  log_file.close();
}

}  // namespace uni_course_cpp
