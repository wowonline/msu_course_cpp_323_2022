#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
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
void Logger::log(const std::string& string) {
  const auto log_string = get_current_date_time() + " " + string;
  std::cout << log_string << std::endl;
  log_file_ << string << std::endl;
}
}  // namespace uni_course_cpp
